#!/usr/bin/env node
// Refuse to publish a release that would break auto-updates on one platform,
// and publish it when it is whole.
//
// Usage:
//   node scripts/release-preflight.js v1.3.1            # check only
//   node scripts/release-preflight.js v1.3.1 --publish  # check, then publish
//   node scripts/release-preflight.js v1.3.1 --only-if-published   # CI: skip drafts
//
// Why this exists, and why it is a hard failure rather than a warning:
// electron-updater resolves the newest release out of the repo's atom feed and
// then demands THAT tag's own channel file. A 404 there throws
// ERR_UPDATER_CHANNEL_FILE_NOT_FOUND with no fallback to an older release
// (node_modules/electron-updater/out/providers/GitHubProvider.js:48 and :124).
// So publishing a release that carries only one platform's channel file does
// not mean "no update available" for the other platform — it means every update
// check on every installed client hard-errors, until someone notices. The
// window lasts as long as the release stays newest.
//
// Checked, per channel:
//   * both channel files are attached — latest.yml + latest-mac.yml for
//     production, staging.yml + staging-mac.yml for staging
//   * each says the same version the tag does
//   * every file each one references is actually attached to the release. This
//     is the check that matters: those URLs are precisely what a client fetches
//     after reading the manifest.
//   * the .blockmap sits beside each Windows installer. It is not named in the
//     manifest, but electron-updater looks for it next to the .exe to download
//     only the changed chunks.
//
// The same script runs locally before publishing and as the `verify` job in
// .github/workflows/release-windows.yml, so a release published straight from
// the GitHub UI still gets caught — just after the fact rather than before.
//
// `--only-if-published` is for that job: a draft is a release still being
// assembled, and nothing has shipped out of one, so an incomplete draft is the
// normal mid-flight state rather than a fault. Without the flag, a pushed `v*`
// tag — which builds Windows days before any mac build exists — would fail the
// run over a draft holding only the Windows three.

const { execFileSync } = require('child_process');
const fs = require('fs');
const os = require('os');
const path = require('path');

const LOG = '[release-preflight]';

const IN_ACTIONS = Boolean(process.env.GITHUB_ACTIONS);

// A workflow command ends at the first newline, and `%` is its escape
// character, so a multi-line message has to be encoded or everything after the
// first line drops off the annotation — here, the per-platform detail and the
// HOW_TO_FIX remediation, i.e. the whole actionable part. `%` goes first.
function encodeAnnotation(message) {
  return message.replace(/%/g, '%25').replace(/\r/g, '%0D').replace(/\n/g, '%0A');
}

function notice(message) {
  if (IN_ACTIONS) console.log(`::notice::${encodeAnnotation(message)}`);
  else console.log(`${LOG} ${message}`);
}

function fail(message) {
  // Workflow commands are read from stdout; the plain form goes to stderr.
  if (IN_ACTIONS) console.log(`::error::${encodeAnnotation(message)}`);
  else console.error(`${LOG} ERROR: ${message}`);
  process.exit(1);
}

// argv, never a shell — staging artifact names contain `[beta]`.
function gh(args, { capture = false } = {}) {
  return execFileSync('gh', args, {
    encoding: 'utf8',
    stdio: capture ? ['ignore', 'pipe', 'pipe'] : ['ignore', 'inherit', 'inherit'],
  });
}

function ghJson(args) {
  try {
    return { ok: true, data: JSON.parse(gh(args, { capture: true })) };
  } catch (err) {
    // "release not found" and "GitHub is having a bad day" both exit non-zero;
    // the caller reports them differently.
    const stderr = String(err.stderr || '').trim();
    return { ok: false, notFound: /release not found/i.test(stderr), stderr, error: err };
  }
}

// electron-builder writes these files itself and their shape is fixed, so a
// line reader beats taking on a YAML dependency for three keys. Only what is
// read below is understood: `version:`, the `- url:` entries under `files:`,
// and the top-level `path:`.
function readManifest(file) {
  const manifest = { version: null, urls: [], path: null };

  for (const rawLine of fs.readFileSync(file, 'utf8').split('\n')) {
    const line = rawLine.replace(/\r$/, '');

    const version = /^version:\s*(.+)$/.exec(line);
    if (version) manifest.version = unquote(version[1]);

    const url = /^\s+-\s+url:\s*(.+)$/.exec(line);
    if (url) manifest.urls.push(unquote(url[1]));

    const top = /^path:\s*(.+)$/.exec(line);
    if (top) manifest.path = unquote(top[1]);
  }

  return manifest;
}

function unquote(value) {
  return value.trim().replace(/^['"]|['"]$/g, '');
}

function parseArgs(argv) {
  const options = { tag: null, publish: false, onlyIfPublished: false };

  for (const arg of argv) {
    if (arg === '--publish') options.publish = true;
    else if (arg === '--only-if-published') options.onlyIfPublished = true;
    else if (arg.startsWith('--')) fail(`Unknown option ${arg}.`);
    else if (!options.tag) options.tag = arg;
    else fail(`Unexpected argument ${arg}. Only one tag is accepted.`);
  }

  if (!options.tag) {
    fail('Missing tag. Usage: release-preflight.js v1.3.1 [--publish] [--only-if-published]');
  }

  if (options.publish && options.onlyIfPublished) {
    fail('--publish and --only-if-published are contradictory: one publishes a draft, the other skips drafts.');
  }

  const staging = options.tag.endsWith('-staging');
  options.channel = staging ? 'staging' : 'production';
  options.version = options.tag.replace(/^v/, '').replace(/-staging$/, '');

  // Windows first, because its absence is the failure mode that prompted all
  // of this — the mac side has always created the release.
  options.manifests = staging
    ? [
        { name: 'staging.yml', platform: 'Windows' },
        { name: 'staging-mac.yml', platform: 'macOS' },
      ]
    : [
        { name: 'latest.yml', platform: 'Windows' },
        { name: 'latest-mac.yml', platform: 'macOS' },
      ];

  return options;
}

const HOW_TO_FIX = {
  Windows:
    'Windows artifacts come from CI: gh workflow run "Release Windows" --ref staging ' +
    '-f channel=<channel> -f upload_to_release=true',
  macOS: 'macOS artifacts come from a Mac: npm run fresh-export (or fresh-export:staging)',
};

function main() {
  const { tag, channel, version, manifests, publish, onlyIfPublished } = parseArgs(process.argv.slice(2));

  console.log(`${LOG} Checking ${tag} (${channel}, version ${version})`);

  // Duplicate drafts on one tag would make every check below ambiguous: `gh
  // release view` picks one of them and the other could be the one someone
  // publishes. Same guard as scripts/release-upload.js.
  const list = ghJson(['release', 'list', '--limit', '100', '--json', 'tagName,isDraft']);
  if (list.ok) {
    const matches = list.data.filter(release => release.tagName === tag);
    if (matches.length > 1) {
      fail(
        `${matches.length} releases exist for ${tag}. Delete all but one with \`gh release delete ${tag}\` ` +
          'before publishing — assets are split across them.'
      );
    }
  }

  const view = ghJson(['release', 'view', tag, '--json', 'isDraft,isPrerelease,assets,url']);
  if (!view.ok) {
    fail(view.notFound ? `No release found for ${tag}.` : `Could not read release ${tag}: ${view.stderr}`);
  }

  const release = view.data;
  const attached = new Set(release.assets.map(asset => asset.name));

  // Nothing has shipped out of a draft, so an incomplete one cannot break any
  // client — it is a release being assembled, which is the normal mid-flight
  // state now that either platform may get there first. The `verify` job passes
  // this so a pushed `v*` tag, which builds Windows days before any mac build
  // exists, does not go red over a draft holding only the Windows three.
  // Publishing clears isDraft, and publishing is what this guard is about.
  if (onlyIfPublished && release.isDraft) {
    notice(`${tag} is still a draft — nothing is live yet, so there is nothing to verify. ${release.url}`);
    return;
  }

  const absentManifests = manifests.filter(manifest => !attached.has(manifest.name));
  if (absentManifests.length > 0) {
    const detail = absentManifests
      .map(manifest => `  ${manifest.name} (${manifest.platform}) — ${HOW_TO_FIX[manifest.platform]}`)
      .join('\n');
    fail(
      `${tag} is missing ${absentManifests.length} of ${manifests.length} update manifest(s):\n${detail}\n` +
        'Publishing without one means every update check on that platform fails with ' +
        'ERR_UPDATER_CHANNEL_FILE_NOT_FOUND for as long as this release is the newest.'
    );
  }

  // Downloaded rather than read from a local build directory: the assets on the
  // release are what clients get, and this has to work on a machine that built
  // neither platform. `gh release download` reads drafts fine.
  const tmp = fs.mkdtempSync(path.join(os.tmpdir(), 'sayso-preflight-'));
  let failed = false;
  // Collected rather than reported inline: fail() calls process.exit, which does
  // not unwind the stack, so a fail() inside the try would skip the finally and
  // leak the temp dir.
  let aborted = null;

  try {
    for (const { name, platform } of manifests) {
      try {
        gh(['release', 'download', tag, '--pattern', name, '--dir', tmp, '--clobber'], { capture: true });
      } catch (err) {
        // capture: true pipes gh's stderr, so without this the `verify` log would
        // show a bare stack trace and no reason at all.
        throw new Error(`Could not download ${name} from ${tag}: ${String(err.stderr || err.message).trim()}`);
      }

      const manifest = readManifest(path.join(tmp, name));
      let manifestFailed = false;

      if (manifest.version !== version) {
        console.error(
          `${LOG} ${name} says version ${manifest.version}, but the tag is ${tag} (version ${version}). ` +
            'Clients compare that value against their own, so this would offer the wrong update.'
        );
        manifestFailed = true;
      }

      // The top-level `path` is what an older client reads; the `files` entries
      // are what a current one reads. Both have to resolve.
      const referenced = new Set(manifest.urls);
      if (manifest.path) referenced.add(manifest.path);

      // Not in the manifest, but electron-updater fetches it beside the .exe for
      // differential downloads.
      for (const url of [...referenced]) {
        if (url.endsWith('.exe')) referenced.add(`${url}.blockmap`);
      }

      const dangling = [...referenced].filter(url => !attached.has(url));
      if (dangling.length > 0) {
        console.error(
          `${LOG} ${name} references ${dangling.length} file(s) not attached to ${tag}:\n  ${dangling.join('\n  ')}`
        );
        manifestFailed = true;
      }

      if (manifestFailed) {
        failed = true;
      } else {
        console.log(`${LOG} ok  ${name} (${platform}) — version ${manifest.version}, ${referenced.size} file(s) present`);
      }
    }
  } catch (err) {
    aborted = err;
  } finally {
    fs.rmSync(tmp, { recursive: true, force: true });
  }

  if (aborted) {
    fail(aborted.message);
  }

  if (failed) {
    fail(`${tag} is not safe to publish. See the errors above.`);
  }

  if (!publish) {
    const state = release.isDraft ? 'draft' : 'published';
    notice(`${tag} (${state}) carries both platforms' artifacts. ${release.url}`);
    if (release.isDraft) {
      console.log(`${LOG} Publish it with: node scripts/release-preflight.js ${tag} --publish`);
    }
    return;
  }

  if (!release.isDraft) {
    notice(`${tag} is already published; nothing to do. ${release.url}`);
    return;
  }

  // --latest is explicit rather than left to GitHub's heuristic, which keys off
  // dates and can leave an older release marked latest. Never for staging: it
  // is a prerelease and must not become what a production client resolves.
  const args = ['release', 'edit', tag, '--draft=false'];
  if (channel === 'production') args.push('--latest');

  console.log(`${LOG} Publishing ${tag}...`);
  gh(args);

  // NOT release.url — that was read while this was still a draft, and a draft's
  // url is the `…/releases/tag/untagged-<sha>` form, which stops resolving the
  // moment it is published. Re-read it so the line the operator clicks works.
  const after = ghJson(['release', 'view', tag, '--json', 'url']);
  notice(`Published ${tag}. ${after.ok ? after.data.url : `see the ${tag} release on GitHub`}`);
}

main();
