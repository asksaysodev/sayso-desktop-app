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

This will clean previous builds, rebuild everything, create DMGs for both Intel and Apple Silicon Macs, sign and notarize them, then attach them to the GitHub release for that version — creating it as a draft if the Windows build has not already.

### 3. Build the Windows installer into the same draft

Windows installers are built in CI, not on the Mac. Kick that off against the
draft:

```bash
gh workflow run "Release Windows" --ref staging -f channel=production -f upload_to_release=true
```

Order does not matter — run this before the mac build if you prefer, and the mac
script will join the release it created.

### 4. Publish

```bash
node scripts/release-preflight.js v1.0.1 --publish
```

This refuses to publish unless **both** platforms' update manifests are attached
(`latest.yml` and `latest-mac.yml`). Publishing with one missing makes every
update check on the other platform hard-error — see
[VERSIONING.md](VERSIONING.md).

Add release notes describing what changed on GitHub; `/changelog` generates them.

### 5. Users Get Auto-Updated

Once published, users get the update automatically on their next app launch. The app detects their Mac type (Intel or Apple Silicon), downloads the right DMG, and prompts them to restart.

## Architecture Support

The build process creates binaries for:
- **arm64**: MacBooks with Apple Silicon (M1, M2, M3, M4)
- **x64**: MacBooks with Intel processors

Both files must be uploaded to the same GitHub release for auto-updates to work correctly.

## How It Works

The auto-update system uses two tools:

1. **electron-builder** packages the app; `scripts/release-upload.js` attaches the artifacts to the GitHub release
2. **electron-updater** runs on users' machines and downloads updates from that release
