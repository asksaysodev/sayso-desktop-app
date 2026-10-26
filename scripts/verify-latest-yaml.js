// Checks that latest-mac.yml (or staging-mac.yml) describes the exact files
// sitting next to it: version matches, and every listed url exists with the
// listed sha512 and size. Exits 1 on any mismatch, so a stale manifest never
// reaches a release — a wrong sha512 fails every Mac update with
// ERR_CHECKSUM_MISMATCH after a full download (Sentry SAYSO-APP-C8, v1.3.5).
//
// Usage: node scripts/verify-latest-yaml.js --version <x.y.z> [--staging]
// Also exported for update-latest-yaml.js.

const fs = require('fs');
const path = require('path');
const crypto = require('crypto');

function parseArgs(argv) {
  const versionIdx = argv.indexOf('--version');
  return {
    isStaging: argv.includes('--staging'),
    version: versionIdx !== -1 ? argv[versionIdx + 1] : undefined,
  };
}

function releasePaths(isStaging) {
  const releaseDir = path.join(__dirname, isStaging ? '../release-staging' : '../release');
  return {
    releaseDir,
    yamlPath: path.join(releaseDir, isStaging ? 'staging-mac.yml' : 'latest-mac.yml'),
    appName: isStaging ? 'Sayso-Beta' : 'Sayso',
  };
}

function fileStats(filePath) {
  const buffer = fs.readFileSync(filePath);
  return {
    sha512: crypto.createHash('sha512').update(buffer).digest('base64'),
    size: buffer.length,
  };
}

// Reads the electron-builder manifest shape: top-level version/path/sha512 and
// a files list of { url, sha512, size }.
function parseManifest(yamlContent) {
  const manifest = { files: [] };
  let current = null;
  for (const line of yamlContent.split('\n')) {
    let m;
    if ((m = line.match(/^\s+- url: (.+)$/))) {
      current = { url: m[1].trim() };
      manifest.files.push(current);
    } else if (current && (m = line.match(/^\s+(sha512|size): (.+)$/))) {
      current[m[1]] = m[1] === 'size' ? Number(m[2]) : m[2].trim();
    } else if ((m = line.match(/^(version|path|sha512): (.+)$/))) {
      current = null;
      manifest[m[1]] = m[2].trim().replace(/^'(.*)'$/, '$1');
    }
  }
  return manifest;
}

function verifyManifest({ version, isStaging }) {
  const { releaseDir, yamlPath, appName } = releasePaths(isStaging);
  const errors = [];

  if (!fs.existsSync(yamlPath)) {
    return [`${path.basename(yamlPath)} not found in ${releaseDir}`];
  }
  const manifest = parseManifest(fs.readFileSync(yamlPath, 'utf8'));

  if (manifest.version !== version) {
    errors.push(`manifest version is ${manifest.version}, expected ${version}`);
  }

  for (const arch of ['x64', 'arm64']) {
    const zip = `${appName}-${version}-${arch}-mac.zip`;
    if (!manifest.files.some((f) => f.url === zip)) {
      errors.push(`manifest has no entry for ${zip}`);
    }
  }

  const actualByUrl = {};
  for (const entry of manifest.files) {
    const filePath = path.join(releaseDir, entry.url);
    if (!fs.existsSync(filePath)) {
      errors.push(`${entry.url} is listed but missing from ${releaseDir}`);
      continue;
    }
    const actual = fileStats(filePath);
    actualByUrl[entry.url] = actual;
    if (entry.sha512 !== actual.sha512) {
      errors.push(`${entry.url} sha512 mismatch: manifest ${entry.sha512}, file ${actual.sha512}`);
    }
    if (entry.size !== actual.size) {
      errors.push(`${entry.url} size mismatch: manifest ${entry.size}, file ${actual.size}`);
    }
  }

  const top = actualByUrl[manifest.path];
  if (!top) {
    errors.push(`top-level path ${manifest.path} is not one of the listed files`);
  } else if (manifest.sha512 !== top.sha512) {
    errors.push(`top-level sha512 mismatch for ${manifest.path}`);
  }

  return errors;
}

module.exports = { parseArgs, releasePaths, fileStats, verifyManifest };

if (require.main === module) {
  const { version, isStaging } = parseArgs(process.argv);
  if (!version) {
    console.error('❌ Usage: node scripts/verify-latest-yaml.js --version <x.y.z> [--staging]');
    process.exit(1);
  }
  const errors = verifyManifest({ version, isStaging });
  const name = path.basename(releasePaths(isStaging).yamlPath);
  if (errors.length) {
    console.error(`❌ ${name} does not match the release files:`);
    for (const e of errors) console.error(`   - ${e}`);
    process.exit(1);
  }
  console.log(`✅ ${name} matches every release file (sha512 + size).`);
}
