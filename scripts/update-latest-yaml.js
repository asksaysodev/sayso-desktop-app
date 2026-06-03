const fs = require('fs');
const path = require('path');
const crypto = require('crypto');

const isStaging = process.argv.includes('--staging');

const packageJson = require('../package.json');
const version = packageJson.version;

const releaseDir = path.join(__dirname, isStaging ? '../release-staging' : '../release');
const yamlPath = path.join(releaseDir, isStaging ? 'latest-staging-mac.yml' : 'latest-mac.yml');
const appName = isStaging ? 'Sayso [beta]' : 'Sayso';

const files = {
  x64: `${appName}-${version}-mac.zip`,
  arm64: `${appName}-${version}-arm64-mac.zip`,
};

function escapeRegex(str) {
  return str.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
}

function getFileStats(filePath) {
  try {
    const buffer = fs.readFileSync(filePath);
    const hash = crypto.createHash('sha512').update(buffer).digest('base64');
    const size = fs.statSync(filePath).size;
    return { hash, size };
  } catch (error) {
    console.warn(`⚠️  Could not read ${path.basename(filePath)}: ${error.message}`);
    return null;
  }
}

function updateYaml() {
  if (!fs.existsSync(yamlPath)) {
    console.error('❌ latest-mac.yml not found in release directory.');
    process.exit(1);
  }

  let yamlContent = fs.readFileSync(yamlPath, 'utf8');
  let updated = false;

  // Process x64 file (Intel)
  const x64Path = path.join(releaseDir, files.x64);
  const x64Stats = getFileStats(x64Path);
  
  if (x64Stats) {
    console.log(`📝 Updating x64 (Intel) hash for ${files.x64}...`);
    // Regex to match the file entry block in the files array
    // We look for the url, then capture the sha512 and size lines following it
    // This simple regex assumes standard electron-builder yaml formatting
    
    // Update sha512 for this specific file in the 'files' list
    const escapedX64 = escapeRegex(files.x64);
    const fileBlockRegex = new RegExp(`(- url: ${escapedX64}[\\s\\S]*?sha512: )[^\\n]+`, 'g');
    if (fileBlockRegex.test(yamlContent)) {
      yamlContent = yamlContent.replace(fileBlockRegex, `$1${x64Stats.hash}`);
      updated = true;
    }

    // Update size for this specific file
    const sizeRegex = new RegExp(`(- url: ${escapedX64}[\\s\\S]*?size: )\\d+`, 'g');
    if (sizeRegex.test(yamlContent)) {
      yamlContent = yamlContent.replace(sizeRegex, `$1${x64Stats.size}`);
      updated = true;
    }

    // Also update the top-level path/sha512 if it points to this file
    // electron-builder usually sets the top-level path to the 'default' artifact (usually x64)
    if (yamlContent.includes(`path: ${files.x64}`)) {
      console.log('📝 Updating top-level hash...');
      const topLevelShaRegex = /sha512: [^\n]+$/m; // carefully target specific top level keys if possible, but yaml structure varies. 
      // Actually, let's be more specific to avoid replacing the array items again if they matched above (though replace matches first)
      // The top level sha512 usually appears at the end or typically outside the files array.
      
      // Safer approach: replace specifically the "sha512: ..." that is NOT indented (or check context)
      // Standard latest-mac.yml has top level keys at start of line
      const topLevelShaRegexLine = /^sha512: .+$/m;
      yamlContent = yamlContent.replace(topLevelShaRegexLine, `sha512: ${x64Stats.hash}`);
      
      // Also releaseDate is updated by builder, we can leave it.
    }
  }

  // Process arm64 file (Apple Silicon)
  const arm64Path = path.join(releaseDir, files.arm64);
  const arm64Stats = getFileStats(arm64Path);

  if (arm64Stats) {
    console.log(`📝 Updating arm64 hash for ${files.arm64}...`);
    
    const escapedArm64 = escapeRegex(files.arm64);
    const fileBlockRegex = new RegExp(`(- url: ${escapedArm64}[\\s\\S]*?sha512: )[^\\n]+`, 'g');
    if (fileBlockRegex.test(yamlContent)) {
      yamlContent = yamlContent.replace(fileBlockRegex, `$1${arm64Stats.hash}`);
      updated = true;
    }

    const sizeRegex = new RegExp(`(- url: ${escapedArm64}[\\s\\S]*?size: )\\d+`, 'g');
    if (sizeRegex.test(yamlContent)) {
      yamlContent = yamlContent.replace(sizeRegex, `$1${arm64Stats.size}`);
      updated = true;
    }
  }

  if (updated) {
    fs.writeFileSync(yamlPath, yamlContent, 'utf8');
    console.log('✅ latest-mac.yml updated successfully.');
  } else {
    console.log('⚠️  No matching entries found in latest-mac.yml to update.');
  }
}

updateYaml();
