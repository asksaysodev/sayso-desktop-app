/**
 * Redaction helpers for the renderer — see electron/shared/redact.ts for why the
 * re-export points this direction (same reason as src/types/update.ts:
 * tsconfig.electron.json pins rootDir to electron/, so electron → src is a hard
 * TS6059, while src → electron works).
 *
 * VALUE re-export, not type-only: this is runtime code and it must survive into
 * the Vite bundle. Safe to pull in, because the shared module is import-free and
 * node-free by construction.
 */
export {
  redactSentryEvent,
  redactSentryBreadcrumb,
  scrubString,
  redactValue,
} from '../../electron/shared/redact';
