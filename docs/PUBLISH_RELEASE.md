# Publishing a New Release

How to publish a new version of Sayso with automatic updates.

## Prerequisites

### 1. GitHub Personal Access Token


Add to .env
```
GH_TOKEN=ghp_your_token_here
```

To get a GitHub token:
1. Click your profile picture on GitHub → Settings
2. Scroll all the way down → Developer settings
3. Personal access tokens → Tokens (classic)
4. Generate new token (classic)
5. Give it `repo` scope

## Release Process

### 1. Update Version Number

Bump on `development`, before merging into `staging` — never on `staging` itself.
See [VERSIONING.md](VERSIONING.md) for why. Production builds run from `staging`
and abort if the local version doesn't match `origin/staging`.

Edit `package.json` and bump the version:

```
"version": "1.0.1"  // From 1.0.0
```

Version format is `MAJOR.MINOR.PATCH`:
- **PATCH** (1.0.0 → 1.0.1): Bug fixes only
- **MINOR** (1.0.0 → 1.1.0): New features (backwards compatible)
- **MAJOR** (1.0.0 → 2.0.0): Breaking changes

You can also use these commands:
```
npm version patch  # For bug fixes
npm version minor  # For new features
npm version major  # For breaking changes
```

### 2. Build and Upload Release

Run the export script:

```
npm run fresh-export
```

This will clean previous builds, rebuild everything, create DMGs for both Intel and Apple Silicon Macs, sign and notarize them, then automatically create a draft release on GitHub with both DMG files uploaded.

### 3. Publish on GitHub

1. Go to https://github.com/asksaysodev/sayso-desktop-app/releases
2. Find the draft release (tagged `v1.0.1`)
3. Make sure both DMG files are there (arm64 and Intel)
4. Add release notes describing what changed
5. Click "Publish release"

### 4. Users Get Auto-Updated

Once published, users get the update automatically on their next app launch. The app detects their Mac type (Intel or Apple Silicon), downloads the right DMG, and prompts them to restart.

## Architecture Support

The build process creates binaries for:
- **arm64**: MacBooks with Apple Silicon (M1, M2, M3, M4)
- **x64**: MacBooks with Intel processors

Both files must be uploaded to the same GitHub release for auto-updates to work correctly.

## How It Works

The auto-update system uses two tools:

1. **electron-builder** runs on your machine and uploads releases to GitHub
2. **update-electron-app** runs on users machines and downloads updates from GitHub
