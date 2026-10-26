// Rewrites sha512/size in latest-mac.yml (or staging-mac.yml) after
// notarization re-zips the apps and staples the DMGs, then verifies the result.
//
// The version comes from the caller (--version), never from package.json: the
// release script reads APP_VERSION once at the start, and re-reading it here
// ~10 minutes later picks up whatever branch is checked out by then. That is
// how v1.3.5 shipped a stale manifest (Sentry SAYSO-APP-C8).
//
// Usage: node scripts/update-latest-yaml.js --version <x.y.z> [--staging]

const fs = require('fs');
const path = require('path');
const { parseArgs, releasePaths, fileStats, verifyManifest } = require('./verify-latest-yaml');

const { version, isStaging } = parseArgs(process.argv);
if (!version) {
  console.error('❌ Usage: node scripts/update-latest-yaml.js --version <x.y.z> [--staging]');
  process.exit(1);
}

const { releaseDir, yamlPath, appName } = releasePaths(isStaging);
const yamlName = path.basename(yamlPath);

if (!fs.existsSync(yamlPath)) {
  console.error(`❌ ${yamlName} not found in ${releaseDir}.`);
  process.exit(1);
}

for (const arch of ['x64', 'arm64']) {
  const zip = `${appName}-${version}-${arch}-mac.zip`;
  if (!fs.existsSync(path.join(releaseDir, zip))) {
    console.error(`❌ ${zip} not found in ${releaseDir} — is --version ${version} right?`);
    process.exit(1);
  }
}

// Walk the manifest line by line: inside a files entry, sha512/size belong to
// the url above them; at the top level, sha512 belongs to `path:`.
const lines = fs.readFileSync(yamlPath, 'utf8').split('\n');
const topLevelPath = (lines.find((l) => l.startsWith('path: ')) || '').slice('path: '.length).trim();
let currentStats = null;
let updatedEntries = 0;

for (let i = 0; i < lines.length; i++) {
  const line = lines[i];
  let m;
  if ((m = line.match(/^(\s+)- url: (.+)$/))) {
    const url = m[2].trim();
    const filePath = path.join(releaseDir, url);
    if (!fs.existsSync(filePath)) {
      console.error(`❌ ${yamlName} lists ${url}, which is missing from ${releaseDir}.`);
      process.exit(1);
    }
    console.log(`📝 Updating hash for ${url}...`);
    currentStats = fileStats(filePath);
    updatedEntries++;
  } else if (currentStats && (m = line.match(/^(\s+)sha512: .+$/))) {
    lines[i] = `${m[1]}sha512: ${currentStats.sha512}`;
  } else if (currentStats && (m = line.match(/^(\s+)size: \d+$/))) {
    lines[i] = `${m[1]}size: ${currentStats.size}`;
  } else if (/^\S/.test(line)) {
    currentStats = null;
    if (line.startsWith('sha512: ')) {
      lines[i] = `sha512: ${fileStats(path.join(releaseDir, topLevelPath)).sha512}`;
    }
  }
}

if (updatedEntries === 0) {
  console.error(`❌ No file entries found in ${yamlName} to update.`);
  process.exit(1);
}

fs.writeFileSync(yamlPath, lines.join('\n'), 'utf8');

const errors = verifyManifest({ version, isStaging });
if (errors.length) {
  console.error(`❌ ${yamlName} still does not match the release files after updating:`);
  for (const e of errors) console.error(`   - ${e}`);
  process.exit(1);
}
console.log(`✅ ${yamlName} updated (${updatedEntries} entries) and verified.`);
