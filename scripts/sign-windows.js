// Authenticode signing for the Windows build, via Azure Artifact Signing
// (the service Microsoft used to call Trusted Signing).
//
// Wired in as `build.win.sign` in package.json. electron-builder calls this once
// per file it decides to sign — with the default `signDlls`/`signExts` that is
// four per build: Sayso.exe, resources/elevate.exe, the NSIS uninstaller, and
// the finished installer. Signing only the installer afterwards would leave the
// app executable inside it unsigned, which is why this is a hook and not a
// post-build step.
//
// Since the June 2023 CA/Browser Forum baseline change the private key has to
// live on FIPS 140-2 Level 2 hardware, so there is no .pfx to point
// `certificateFile` at. The key stays non-exportable inside Azure's HSM and
// signtool reaches it through the Trusted Signing dlib.
//
// Five things about electron-builder 24.13.3 shape this file. All were read out
// of node_modules, not the docs — current docs describe v25+/v26, whose
// `signtoolOptions` nesting and `win.azureSignOptions` do NOT apply here.
//
//   1. The hook is called once per (file, hash algorithm) pair
//      (windowsCodeSign.js:20-32). `signingHashAlgorithms` defaults to
//      ["sha1", "sha256"], so without the explicit ["sha256"] in package.json
//      this would be asked for a SHA-1 signature Azure cannot issue, plus a
//      nested SHA-256 one. Asserted below rather than trusted.
//   2. The return value is ignored (windowsCodeSign.js:38). Throwing is the
//      only way to fail the build. Returning nothing looks like success.
//   3. There is no retry above us. The 3-attempt retry in winPackager.doSign
//      only guards the certificate-file path; the cscInfo == null path we are
//      on (winPackager.js:178) calls straight through. Hence withRetry here.
//   4. `configuration.cscInfo` is undefined and `computeSignToolArgs()` must
//      never be called — it dereferences `options.cscInfo.file`
//      (windowsCodeSign.js:153) and would throw.
//   5. electron-builder signs concurrently (Bluebird.map with concurrency 4 for
//      the asar-unpacked walk, unbounded for the top-level readdir), so the
//      calls are serialised here.

const { execFileSync } = require('child_process');
const fs = require('fs');
const os = require('os');
const path = require('path');

// Non-secret, and deliberately in the repo: recorded on SAYSO-402 as safe to
// reference in code. The actual credentials are the three AZURE_* environment
// variables, which DefaultAzureCredential reads inside the dlib — nothing in
// this file ever touches their values.
const ENDPOINT = process.env.AZURE_SIGN_ENDPOINT || 'https://eus.codesigning.azure.net/';
const ACCOUNT = process.env.AZURE_SIGN_ACCOUNT || 'AskSayso';
const PROFILE = process.env.AZURE_SIGN_PROFILE || 'sayso-windows-prod';

// Not optional. Artifact Signing issues short-lived 72-hour certificates that
// rotate automatically, so an untimestamped signature stops validating three
// days later — including on installers users already downloaded. The CI verify
// step asserts a timestamp is present precisely to catch this going missing.
const TIMESTAMP_URL = 'http://timestamp.acs.microsoft.com';

// The credential env vars, plus the path to Azure.CodeSigning.Dlib.dll that the
// workflow exports after unpacking the NuGet package.
const CREDENTIAL_VARS = ['AZURE_TENANT_ID', 'AZURE_CLIENT_ID', 'AZURE_CLIENT_SECRET'];
const DLIB_VAR = 'AZURE_SIGN_DLIB';

// signtool grew /dlib in the 10.0.22621 SDK.
const MIN_SDK = [10, 0, 22621];

const LOG = '[sign-windows]';

let signToolPath = null;
let metadataPath = null;
let skipWarningShown = false;

function parseVersion(name) {
  const m = /^(\d+)\.(\d+)\.(\d+)(?:\.(\d+))?$/.exec(name);
  return m ? [+m[1], +m[2], +m[3], +(m[4] || 0)] : null;
}

function compareVersions(a, b) {
  for (let i = 0; i < Math.max(a.length, b.length); i++) {
    const diff = (a[i] || 0) - (b[i] || 0);
    if (diff !== 0) return diff;
  }
  return 0;
}

// The Windows Kits layout is bin/<sdk version>/x64/signtool.exe, with an older
// unversioned bin/x64/ alongside it. Take the newest versioned one so a machine
// with several SDKs installed behaves predictably.
function findSignTool() {
  if (signToolPath) return signToolPath;

  if (process.env.SIGNTOOL_PATH) {
    if (!fs.existsSync(process.env.SIGNTOOL_PATH)) {
      throw new Error(`${LOG} SIGNTOOL_PATH is set to ${process.env.SIGNTOOL_PATH}, which does not exist.`);
    }
    signToolPath = process.env.SIGNTOOL_PATH;
    return signToolPath;
  }

  if (process.platform !== 'win32') {
    throw new Error(
      `${LOG} Windows code signing needs signtool.exe, which only exists on Windows. ` +
        'Build the Windows target on a Windows runner — see .github/workflows/release-windows.yml.'
    );
  }

  const roots = [process.env['ProgramFiles(x86)'], process.env.ProgramFiles]
    .filter(Boolean)
    .map(base => path.join(base, 'Windows Kits', '10', 'bin'));

  const candidates = [];
  for (const root of roots) {
    if (!fs.existsSync(root)) continue;
    for (const entry of fs.readdirSync(root)) {
      const version = parseVersion(entry);
      if (!version) continue;
      const exe = path.join(root, entry, 'x64', 'signtool.exe');
      if (fs.existsSync(exe)) candidates.push({ version, exe, entry });
    }
  }

  if (candidates.length === 0) {
    throw new Error(
      `${LOG} No signtool.exe found under ${roots.join(' or ')}. ` +
        'Install the Windows SDK (10.0.22621 or newer), or set SIGNTOOL_PATH.'
    );
  }

  candidates.sort((a, b) => compareVersions(b.version, a.version));
  const best = candidates[0];

  if (compareVersions(best.version, MIN_SDK) < 0) {
    throw new Error(
      `${LOG} Newest signtool.exe is from SDK ${best.entry}, but /dlib needs ${MIN_SDK.join('.')} or newer. ` +
        'Install a newer Windows SDK, or set SIGNTOOL_PATH.'
    );
  }

  console.log(`${LOG} signtool from SDK ${best.entry}`);
  signToolPath = best.exe;
  return signToolPath;
}

// signtool reads the account and profile out of a JSON file rather than flags.
// Written once per build into a temp dir; it holds no secrets.
function findMetadataFile() {
  if (metadataPath) return metadataPath;

  const dir = fs.mkdtempSync(path.join(os.tmpdir(), 'sayso-sign-'));
  const file = path.join(dir, 'metadata.json');
  fs.writeFileSync(
    file,
    JSON.stringify({ Endpoint: ENDPOINT, CodeSigningAccountName: ACCOUNT, CertificateProfileName: PROFILE }, null, 2)
  );

  console.log(`${LOG} ${ACCOUNT}/${PROFILE} at ${ENDPOINT}`);
  metadataPath = file;
  return metadataPath;
}

function missingCredentials() {
  const missing = CREDENTIAL_VARS.filter(name => !process.env[name]);
  if (!process.env[DLIB_VAR]) missing.push(DLIB_VAR);
  return missing;
}

// Role assignments on the signing account take a few minutes to propagate, so a
// 403 on the first run after provisioning is expected rather than a bug. Network
// blips get the same treatment.
async function withRetry(label, fn) {
  const delays = [15000, 30000];

  for (let attempt = 0; ; attempt++) {
    try {
      return fn();
    } catch (err) {
      if (attempt >= delays.length) throw err;
      const seconds = delays[attempt] / 1000;
      console.warn(`${LOG} ${label} failed (attempt ${attempt + 1} of ${delays.length + 1}), retrying in ${seconds}s`);
      console.warn(`${LOG}   ${(err.message || String(err)).split('\n')[0]}`);
      await new Promise(resolve => setTimeout(resolve, delays[attempt]));
    }
  }
}

// Deterministic failures, kept out of withRetry: re-running these would burn
// the backoff and print the same message three times before failing anyway.
function assertSignable(configuration) {
  const file = configuration.path;

  // See note 1. A regression that drops signingHashAlgorithms would otherwise
  // surface as an opaque Azure error on a SHA-1 request.
  if (configuration.hash && configuration.hash !== 'sha256') {
    throw new Error(
      `${LOG} Asked to sign ${path.basename(file)} with ${configuration.hash}. ` +
        'Azure Artifact Signing issues SHA-256 only — set build.win.signingHashAlgorithms to ["sha256"].'
    );
  }
  if (configuration.isNest) {
    throw new Error(
      `${LOG} Asked to append a nested signature to ${path.basename(file)}. ` +
        'That only happens with more than one entry in build.win.signingHashAlgorithms; it must be ["sha256"].'
    );
  }

  const dlib = process.env[DLIB_VAR];
  if (!fs.existsSync(dlib)) {
    throw new Error(`${LOG} ${DLIB_VAR} points at ${dlib}, which does not exist.`);
  }

  // Resolved here, not in signOne, so a missing SDK fails immediately instead of
  // being retried three times over something no amount of waiting will fix.
  // Both memoise, so the calls in signOne are free.
  findSignTool();
  findMetadataFile();
}

function signOne(configuration) {
  const file = configuration.path;
  const dlib = process.env[DLIB_VAR];
  const signTool = findSignTool();
  const metadata = findMetadataFile();

  const args = [
    'sign',
    '/v',
    '/fd', 'SHA256',
    '/dlib', dlib,
    '/dmdf', metadata,
    '/tr', TIMESTAMP_URL,
    '/td', 'SHA256',
  ];

  // Shown in the UAC prompt and in the file's digital signature details.
  if (configuration.name) args.push('/d', configuration.name);
  if (configuration.site) args.push('/du', configuration.site);

  args.push(file);

  console.log(`${LOG} signing ${path.basename(file)}`);
  execFileSync(signTool, args, { stdio: 'inherit' });
}

// See note 5. Each caller still gets its own promise and its own failure; the
// chain is kept alive past a rejection so one bad file does not strand the rest.
let queue = Promise.resolve();

function enqueue(task) {
  const result = queue.then(task, task);
  queue = result.catch(() => {});
  return result;
}

exports.default = async function signWindows(configuration) {
  const missing = missingCredentials();

  if (missing.length > 0) {
    const detail = `missing ${missing.join(', ')}`;

    // A CI run that quietly produces an unsigned installer is the failure worth
    // being loudest about: it would reach a release looking fine, and then be
    // refused by every client's update check.
    if (process.env.CI) {
      throw new Error(
        `${LOG} Cannot sign — ${detail}. This is CI, where a build must never ship unsigned. ` +
          'Check the AZURE_* repo secrets and the dlib step. See docs/VERSIONING.md.'
      );
    }

    // Locally there is nothing to sign with by design — signing runs in CI. Keep
    // `npm run package` working and produce an unsigned build.
    if (!skipWarningShown) {
      console.warn(`${LOG} Not signing — ${detail}. Local build, so this installer will be UNSIGNED.`);
      skipWarningShown = true;
    }
    return;
  }

  await enqueue(() => {
    assertSignable(configuration);
    return withRetry(path.basename(configuration.path), () => signOne(configuration));
  });
};
