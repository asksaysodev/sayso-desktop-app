/**
 * Redaction at the serialization boundary (SAYSO-460).
 *
 * WHY THIS EXISTS
 * Any axios failure used to write the full request `config` — including
 * `headers.Authorization` — into the production log file. `setupLogging()` in
 * main.ts serializes every object argument with JSON.stringify, and
 * `AxiosError.prototype.toJSON()` returns `config` with headers intact. Supabase
 * access tokens live an hour, so a log collected from a user inside that window
 * hands over a live credential for their account.
 *
 * The fix is deliberately at the *serialization boundary*, not at the ~53
 * individual `console.<level>(…, err)` call sites: a new call site must not be
 * able to reintroduce the leak. The `console.warn` in `loadBootSession`
 * ("Silent auth succeeded but profile fetch failed") is deliberately left
 * logging a raw AxiosError — it is the regression test for this module.
 *
 * TWO LAYERS, BECAUSE THE LEAK HAS TWO SHAPES
 *  (a) STRUCTURAL — key-based redaction while walking an object graph.
 *      Covers JSON.stringify of an AxiosError.
 *  (b) STRING — regex scrub of already-flattened text. A JSON.stringify
 *      replacer can never reach this: Sentry's console integration builds its
 *      breadcrumb message with `util.format(...args)`, which prints
 *      `config: { headers: { Authorization: 'Bearer …' } }` as plain text.
 * Layer (b) also catches `console.warn(`… ${err}`)` and `'Bearer ' + token`.
 * Where both layers can reach, both run. The overlap is deliberate.
 *
 * WHY A RUNTIME FILE LIVES IN electron/shared/
 * The other files here are marked TYPES ONLY, but the invariant that comment
 * protects is the second half of it: "must never import electron or node APIs —
 * nothing from it may survive into the bundle". Types-only was the sufficient
 * condition chosen to guarantee that, not the requirement. tsconfig.electron.json
 * pins rootDir to electron/, so electron → src imports are a hard TS6059; src →
 * electron already works and is the established direction (src/types/update.ts).
 * A security control duplicated across two processes silently drifts, so the
 * convention narrows rather than the module splitting in two:
 *
 *   electron/shared/* must be IMPORT-FREE and SIDE-EFFECT-FREE.
 *
 * Two consequences, both load-bearing:
 *  1. ES2020-SAFE SYNTAX ONLY. This file is typechecked by two programs with
 *     different libs — tsconfig.electron.json (ES2022) and tsconfig.json
 *     (ES2020). `Object.hasOwn`, `.at()` and `.replaceAll()` compile clean in the
 *     first and FAIL the second. Use Object.prototype.hasOwnProperty.call and
 *     .replace(/…/g, …).
 *  2. NO @sentry/* IMPORTS. The Sentry helpers are identity-typed so each config
 *     passes its own SDK's types through, without this file depending on a
 *     package that exists in only one of the two module graphs.
 *
 * NEVER MUTATES ITS INPUT. Redacting an AxiosError's `config.headers` in place
 * would corrupt an object the retry path still holds a reference to, and Sentry's
 * console breadcrumb carries the caller's *live* object references in
 * `data.arguments`. Every function here returns a copy.
 *
 * NOT USED: axios 1.19 honours an opt-in `config.redact` array in
 * AxiosError.toJSON. It is per-call-site, which is exactly what this module
 * exists to avoid, and it would silently miss the next bare-axios call someone
 * adds in main.
 */

export const REDACTED = '[REDACTED]';
export const REDACTED_JWT = '[REDACTED_JWT]';
const CIRCULAR = '[Circular]';

/**
 * Keys whose VALUE is a credential. Compared after normalization, by exact
 * membership — never substring. Substring matching would also eat
 * `tokenExpiresAt`, `hasApiKey` and `cookieConsent`, which is how a redactor
 * makes logs useless and gets reverted.
 *
 * Normalization folds case, hyphens, underscores and spaces, so one entry
 * covers `x-api-key`, `X_API_KEY` and `X Api Key` alike.
 */
const SENSITIVE_KEYS = new Set([
  // The headers named in the ticket
  'authorization',
  'proxyauthorization',
  'wwwauthenticate',
  'cookie',
  'setcookie',
  'xapikey',
  'apikey',
  // Token-bearing body/field names
  'accesstoken',
  'refreshtoken',
  'idtoken',
  'sessiontoken',
  'bearertoken',
  'xauthtoken',
  'xaccesstoken',
  // Generic secrets
  'password',
  'secret',
  'clientsecret',
  'privatekey',
  'anonkey',
  'tokenencryptionkey',
]);

function normalizeKey(key: string): string {
  return key.toLowerCase().replace(/[-_\s]/g, '');
}

export function isSensitiveKey(key: string): boolean {
  return SENSITIVE_KEYS.has(normalizeKey(key));
}

/**
 * Cap on the length of a single scrubbed string, applied AFTER scrubbing.
 *
 * The order matters and is the whole point: truncating first can cut a bare JWT
 * mid-payload, leaving `<header>.<partial payload>` with only one dot, which
 * JWT_PATTERN (two dots) no longer matches — so the fragment goes to disk. It is
 * not a usable credential without its signature, but the payload still base64s
 * back to `{"sub":…,"phone":…}`, which is exactly the PII this module exists to
 * keep out of a support log.
 *
 * Scrubbing first costs ~5 ms on a 5 MB string (measured), and a log argument
 * that large would already be the bigger problem. Correctness wins.
 */
const MAX_SCRUB_LENGTH = 8192;

/**
 * Layer (b). Order matters: the key/value form runs first because it catches
 * non-Bearer credentials (Basic auth, cookies, raw API keys) that the later,
 * shape-based rules cannot see.
 *
 * Matches both `"Authorization": "…"` (JSON.stringify output) and
 * `Authorization: '…'` (util.format output).
 */
const KEY_VALUE_PATTERN =
  /(["']?(?:authorization|proxy-authorization|www-authenticate|cookie|set-cookie|x-api-key|api[-_]?key|access[-_]?token|refresh[-_]?token|id[-_]?token|session[-_]?token|x-auth-token|password|secret|client[-_]?secret|private[-_]?key|anon[-_]?key)["']?\s*[:=]\s*)(?:"[^"]*"|'[^']*'|[^,\n}\]]+)/gi;

/** `Bearer <token>` anywhere, whatever the surrounding syntax. */
const BEARER_PATTERN = /\bbearer\s+[A-Za-z0-9._~+/=-]{8,}/gi;

/**
 * Bare JWT catch-all — the backstop for a token interpolated into a message with
 * no key next to it. `eyJ` is base64 of `{"`. The third segment may be empty
 * (alg=none). base64url charset only, so it cannot run past a closing quote.
 * This also covers the Supabase anon key, which is JWT-shaped; that key is
 * publishable, so over-redacting it is an accepted trade.
 *
 * Deliberately NOT anchored with `\b`: a word character immediately before `eyJ`
 * suppresses the boundary and would let a concatenated token through
 * (`'user' + token`). The three-segment shape is specific enough on its own that
 * scanning from any offset costs nothing in false positives.
 */
const JWT_PATTERN = /eyJ[A-Za-z0-9_-]{8,}\.[A-Za-z0-9_-]{8,}\.[A-Za-z0-9_-]*/g;

/** Credentials embedded in a URL — proxy URLs, git remotes. */
const URL_CREDENTIALS_PATTERN = /\/\/[^/\s:@]+:[^/\s@]+@/g;

export function scrubString(input: string): string {
  if (!input) return input;

  const scrubbed = input
    .replace(KEY_VALUE_PATTERN, '$1"' + REDACTED + '"')
    .replace(BEARER_PATTERN, 'Bearer ' + REDACTED)
    .replace(JWT_PATTERN, REDACTED_JWT)
    .replace(URL_CREDENTIALS_PATTERN, '//' + REDACTED + '@');

  // Truncate only after every pattern has had the whole string to match against.
  return scrubbed.length > MAX_SCRUB_LENGTH
    ? scrubbed.slice(0, MAX_SCRUB_LENGTH) +
        '…[truncated ' + (scrubbed.length - MAX_SCRUB_LENGTH) + ' chars]'
    : scrubbed;
}

/** Duck-typed: an Error subclass from another realm still has both fields. */
function isErrorLike(value: any): boolean {
  return value && typeof value.message === 'string' && typeof value.stack === 'string';
}

/**
 * Errors serialize to `{}` through JSON.stringify — `message` and `stack` are
 * non-enumerable. That is why `[MAIN] Uncaught Exception: {}` is what the log
 * file holds today for the single most important line it can carry. Expand them
 * to a plain object, keeping any own enumerable props (AxiosError's `code` and
 * `status` ride along).
 */
function expandError(value: any): Record<string, unknown> {
  const out: Record<string, unknown> = {
    name: value.name,
    message: value.message,
    stack: value.stack,
  };
  for (const key of Object.keys(value)) {
    if (!Object.prototype.hasOwnProperty.call(out, key)) out[key] = value[key];
  }
  return out;
}

function describeBinary(value: any): string | null {
  if (typeof ArrayBuffer !== 'undefined' && ArrayBuffer.isView(value)) {
    return '[Binary ' + (value as any).byteLength + ' bytes]';
  }
  if (typeof ArrayBuffer !== 'undefined' && value instanceof ArrayBuffer) {
    return '[Binary ' + value.byteLength + ' bytes]';
  }
  return null;
}

/**
 * The JSON.stringify replacer. One per call, because the WeakSet is per-walk.
 *
 * THE SPEC DETAIL THAT MAKES THIS WORK: ECMA-262 SerializeJSONProperty calls
 * `value.toJSON(key)` BEFORE the replacer. So for an AxiosError the replacer
 * never sees the instance — it sees toJSON()'s output and descends into
 * `config.headers.Authorization` as an ordinary nested key, which the key rules
 * catch. A plain Error has no toJSON, so the replacer DOES receive the instance
 * and can expand it. Both fixes compose with no axios special-casing.
 *
 * Repeat object references degrade to '[Circular]' rather than being tracked
 * precisely. That over-reports a genuinely shared (acyclic) reference, and it is
 * the right trade: a logger must be total. Today a circular argument throws out
 * of console.warn itself — from inside whatever catch block called it.
 */
function createReplacer(): (this: any, key: string, value: any) => any {
  const seen = new WeakSet<object>();

  return function replacer(this: any, key: string, value: any): any {
    if (key && isSensitiveKey(key)) return REDACTED;
    if (typeof value === 'string') return scrubString(value);
    if (typeof value === 'bigint') return String(value);
    if (typeof value === 'function') return '[Function]';

    if (value && typeof value === 'object') {
      const binary = describeBinary(value);
      if (binary) return binary;
      if (seen.has(value)) return CIRCULAR;
      seen.add(value);
      if (isErrorLike(value)) return expandError(value);
      if (value instanceof RegExp) return value.source;
    }

    return value;
  };
}

/** JSON.stringify with redaction that can never throw. */
export function safeStringify(value: unknown, space?: number): string {
  try {
    const out = JSON.stringify(value, createReplacer(), space);
    return out === undefined ? String(value) : out;
  } catch {
    try {
      return scrubString(String(value));
    } catch {
      return '[Unserializable]';
    }
  }
}

/**
 * Structural deep copy with the same rules, for payloads where no replacer is
 * available (Sentry events and breadcrumbs).
 *
 * Depth and array caps make the walk provably terminating and stop a stray
 * WebContents or IncomingMessage from producing a megabyte of output.
 */
const MAX_DEPTH = 8;
const MAX_ARRAY_ENTRIES = 200;

export function redactValue<T>(value: T): unknown {
  return redactInner(value, 0, new WeakSet<object>());
}

function redactInner(value: any, depth: number, seen: WeakSet<object>): any {
  if (typeof value === 'string') return scrubString(value);
  if (value === null || typeof value !== 'object') {
    return typeof value === 'bigint' || typeof value === 'function' ? String(value) : value;
  }

  const binary = describeBinary(value);
  if (binary) return binary;
  if (seen.has(value)) return CIRCULAR;
  if (depth >= MAX_DEPTH) return '[MaxDepth]';
  seen.add(value);

  if (value instanceof Date) return value.toISOString();
  if (value instanceof RegExp) return value.source;

  if (Array.isArray(value)) {
    const items: any[] = value
      .slice(0, MAX_ARRAY_ENTRIES)
      .map(item => redactInner(item, depth + 1, seen));
    if (value.length > MAX_ARRAY_ENTRIES) {
      items.push('[+' + (value.length - MAX_ARRAY_ENTRIES) + ' more]');
    }
    return items;
  }

  const source: any = isErrorLike(value) ? expandError(value) : value;
  const out: Record<string, unknown> = {};
  for (const key of Object.keys(source)) {
    out[key] = isSensitiveKey(key) ? REDACTED : redactInner(source[key], depth + 1, seen);
  }
  return out;
}

/**
 * The exact serializer setupLogging() needs.
 *
 * Note the non-object branch runs through scrubString: that is what closes the
 * template-literal and string-concatenation paths, which no replacer can reach.
 */
export function formatLogArgs(args: unknown[]): string {
  return args
    .map(arg =>
      typeof arg === 'object' && arg !== null ? safeStringify(arg, 2) : scrubString(String(arg)),
    )
    .join(' ');
}

/**
 * Sentry beforeBreadcrumb.
 *
 * Returns a NEW top-level object and never mutates nested values: the console
 * integration builds `data.arguments` out of the caller's live object
 * references, so rewriting them in place would corrupt the caller's AxiosError.
 *
 * `message` is the one that matters most — it is `util.format(...args)`, i.e.
 * where the token appears already flattened into text.
 */
export function redactSentryBreadcrumb<T>(crumb: T): T {
  const input = crumb as any;
  if (!input || typeof input !== 'object') return crumb;

  const out: any = Object.assign({}, input);
  if (typeof input.message === 'string') out.message = scrubString(input.message);
  if (input.data && typeof input.data === 'object') out.data = redactValue(input.data);
  return out as T;
}

/**
 * Sentry beforeSend.
 *
 * Walks an ALLOWLIST rather than the whole event: `sdkProcessingMetadata` holds
 * `originalException`, live spans and node IncomingMessage objects, and walking
 * it is both expensive and capable of mangling SDK internals.
 *
 * `breadcrumbs[]` is in the list because it is the backstop for RENDERER
 * breadcrumbs. Those bypass main's beforeBreadcrumb entirely — @sentry/electron
 * forwards them over IPC and calls `scope.addBreadcrumb` directly
 * (main/ipc.js:131) — but they ride on events that do pass main's beforeSend.
 *
 * `stacktrace.frames[].vars` is latent, not live: localVariablesIntegration is
 * in the default set, but it early-returns unless `includeLocalVariables: true`,
 * which this app never sets. Covered here so that enabling it can't reintroduce
 * the leak (the `const headers` local in loadBootSession is exactly what it would carry).
 */
export function redactSentryEvent<T>(event: T): T {
  const input = event as any;
  if (!input || typeof input !== 'object') return event;

  const out: any = Object.assign({}, input);

  if (typeof input.message === 'string') out.message = scrubString(input.message);

  if (input.logentry && typeof input.logentry === 'object') {
    out.logentry = Object.assign({}, input.logentry);
    if (typeof input.logentry.message === 'string') {
      out.logentry.message = scrubString(input.logentry.message);
    }
    if (input.logentry.params !== undefined) {
      out.logentry.params = redactValue(input.logentry.params);
    }
  }

  if (input.exception && Array.isArray(input.exception.values)) {
    out.exception = Object.assign({}, input.exception, {
      values: input.exception.values.map((entry: any) => {
        if (!entry || typeof entry !== 'object') return entry;
        const value: any = Object.assign({}, entry);
        if (typeof entry.value === 'string') value.value = scrubString(entry.value);
        if (entry.stacktrace && Array.isArray(entry.stacktrace.frames)) {
          value.stacktrace = Object.assign({}, entry.stacktrace, {
            frames: entry.stacktrace.frames.map((frame: any) =>
              frame && frame.vars ? Object.assign({}, frame, { vars: redactValue(frame.vars) }) : frame,
            ),
          });
        }
        return value;
      }),
    });
  }

  if (Array.isArray(input.breadcrumbs)) {
    out.breadcrumbs = input.breadcrumbs.map((crumb: any) => redactSentryBreadcrumb(crumb));
  }

  if (input.request && typeof input.request === 'object') {
    out.request = Object.assign({}, input.request);
    const requestFields = ['headers', 'cookies', 'data', 'query_string'];
    for (const field of requestFields) {
      if (input.request[field] !== undefined) out.request[field] = redactValue(input.request[field]);
    }
  }

  const eventFields = ['extra', 'contexts', 'tags', 'user'];
  for (const field of eventFields) {
    if (input[field] !== undefined) out[field] = redactValue(input[field]);
  }

  return out as T;
}
