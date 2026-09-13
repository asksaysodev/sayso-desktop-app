#!/usr/bin/env node
// Attach build artifacts to the GitHub release for a version, creating that
// release if nobody has yet. Shared by every path that uploads:
//
//   scripts/rebuild-and-package.sh          mac production, after notarization
//   scripts/rebuild-and-package-staging.sh  mac staging
//   scripts/resume-release.sh               mac, re-upload without rebuilding
//   .github/workflows/release-windows.yml   the Windows installer
//
// One release per version holds both platforms' artifacts, and either platform
// may get there first. That is the whole point of this file: the mac scripts
// used to call `gh release create` unconditionally, so a Windows-first run made
// fresh-export abort at its final step — after the entire notarize cycle, which
// is the expensive part. The Windows workflow worked around it by refusing to
// create a release at all, which made "mac goes first" a rule people had to
// remember. Neither side has to care now (SAYSO-403).
//
// Usage:
//   node scripts/release-upload.js --tag v1.3.1 [--channel production|staging]
//                                  [--target staging] <file>...
//
// --channel defaults to staging when the tag ends in -staging, matching how
// .github/workflows/release-windows.yml reads the channel off the tag. It only
// affects a release this run CREATES: staging releases are prereleases.
//
// --target is the branch a newly created tag points at, and defaults to
// `staging`, the release branch. Without it `gh` tags the repo's default branch
// (development), whose package.json may still hold the pre-bump version. It is
// ignored when the tag already exists.

const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const LOG = '[release-upload]';

// GitHub renders ::warning:: and ::notice:: as annotations on the run; on a
// laptop they are just noise, so they are only emitted under Actions.
const IN_ACTIONS = Boolean(process.env.GITHUB_ACTIONS);

function notice(message) {
  console.log(IN_ACTIONS ? `::notice::${message}` : `${LOG} ${message}`);
}

function warn(message) {
  console.log(IN_ACTIONS ? `::warning::${message}` : `${LOG} WARNING: ${message}`);
}

function fail(message) {
  console.error(IN_ACTIONS ? `::error::${message}` : `${LOG} ERROR: ${message}`);
  process.exit(1);
}

// Every gh call goes through argv, never a shell. Staging's productName is
// `Sayso [beta]`, so its artifact paths carry brackets that a shell would treat
// as a glob — see the manual escaping in scripts/publish-test-release.sh for
// what that costs.
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
    // `gh release view` on a tag with no release exits non-zero, which is a
    // normal outcome here rather than a failure.
    return { ok: false, error: err };
  }
}

function parseArgs(argv) {
  const options = { tag: null, channel: null, target: 'staging', files: [] };

  for (let i = 0; i < argv.length; i++) {
    const arg = argv[i];
    switch (arg) {
      case '--tag':
      case '--channel':
      case '--target': {
        const value = argv[++i];
        if (!value) fail(`${arg} needs a value.`);
        options[arg.slice(2)] = value;
        break;
      }
      default:
        if (arg.startsWith('--')) fail(`Unknown option ${arg}.`);
        options.files.push(arg);
    }
  }

  if (!options.tag) fail('Missing --tag. Usage: release-upload.js --tag v1.3.1 <file>...');
  if (options.files.length === 0) fail('No files to upload.');

  if (!options.channel) {
    options.channel = options.tag.endsWith('-staging') ? 'staging' : 'production';
  }
  if (options.channel !== 'production' && options.channel !== 'staging') {
    fail(`--channel must be production or staging, got ${options.channel}.`);
  }

  return options;
}

// GitHub permits several drafts on one tag — they are not tags yet, just rows.
// Two first-runs racing (mac finishing while the Windows job uploads) could
// leave a pair, and `gh release view <tag>` then picks one of them with no way
// to say which. Assets would split across two releases and the published one
// could be missing a platform. Refuse instead of guessing.
function assertSingleRelease(tag) {
  const result = ghJson(['release', 'list', '--limit', '100', '--json', 'tagName,isDraft,createdAt']);
  if (!result.ok) return; // Listing is a nicety; view below is the real check.

  const matches = result.data.filter(release => release.tagName === tag);
  if (matches.length > 1) {
    fail(
      `${matches.length} releases exist for ${tag} (GitHub allows duplicate drafts on one tag). ` +
        'Delete all but one with `gh release delete ' +
        tag +
        '` and re-run, or assets will split across them.'
    );
  }
}

function main() {
  const { tag, channel, target, files } = parseArgs(process.argv.slice(2));

  // Fail before touching GitHub, so a missing artifact cannot leave a release
  // half-populated. Mirrors the check the mac scripts already do inline.
  const missing = files.filter(file => !fs.existsSync(file));
  if (missing.length > 0) {
    fail(`Missing file(s) to upload:\n  ${missing.join('\n  ')}`);
  }
  console.log(`${LOG} ${files.length} file(s) for ${tag} (${channel})`);

  assertSingleRelease(tag);

  const view = ghJson(['release', 'view', tag, '--json', 'isDraft,assets']);

  if (!view.ok) {
    console.log(`${LOG} No release for ${tag} yet — creating a draft on ${target}.`);

    const args = ['release', 'create', tag, '--target', target, '--title', tag, '--draft'];
    if (channel === 'staging') {
      args.push('--prerelease', '--notes', `Staging release ${tag}`);
    } else {
      args.push('--notes', `Release ${tag}`);
    }
    gh([...args, ...files]);

    notice(`Created draft ${tag} with ${files.length} asset(s).`);
    return;
  }

  if (view.data.isDraft) {
    // Nothing has shipped out of a draft, so replacing assets is safe.
    console.log(`${LOG} ${tag} is a draft — uploading with --clobber.`);
    gh(['release', 'upload', tag, '--clobber', ...files]);
    notice(`${tag} (draft) now carries ${files.length} uploaded asset(s).`);
    return;
  }

  // Published. Users may already hold these bytes and the sha512 in the channel
  // file beside them points at exactly those, so only fill gaps — never
  // replace. This is also the repair path for an upload that failed part way.
  const attached = new Set(view.data.assets.map(asset => asset.name));
  const absent = files.filter(file => !attached.has(path.basename(file)));

  if (absent.length === 0) {
    notice(`${tag} is published and already carries all ${files.length} asset(s); nothing uploaded.`);
    return;
  }

  warn(`${tag} is already published — adding ${absent.length} missing asset(s) without replacing anything.`);
  gh(['release', 'upload', tag, ...absent]);
  notice(`Added to ${tag}: ${absent.map(file => path.basename(file)).join(', ')}`);
}

main();
