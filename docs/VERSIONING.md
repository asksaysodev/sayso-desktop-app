# Versioning & Release Management

## Version bump commands

Use `npm version` before cutting a staging release. It updates `package.json`, commits the change, and creates a git tag automatically.

```bash
npm version patch   # bug fixes: 1.1.1 → 1.1.2
npm version minor   # new features: 1.1.1 → 1.2.0
npm version major   # breaking changes: 1.1.1 → 2.0.0
```

**When to bump:** once, on the `development` branch, before merging into `staging`. Never bump directly on `staging`.

Bumping on `staging` is what broke the 1.2.5 production build: the bump commit stayed on `staging`, `development` still read 1.2.4, and a release built from a development-based branch produced 1.2.4 artifacts, a 1.2.4 `latest-mac.yml`, and a 1.2.4 tag. If a bump does land on `staging` by mistake, merge it back into `development` immediately so the two branches agree.

---

## Branch flow

```
feature/* → development → (npm version patch) → staging
```

There is no `main` branch. `staging` is the release branch — both the staging
pre-release and the production release are built from it.

1. Merge all features into `development`
2. Bump version on `development`
3. Merge `development → staging`
4. Build and publish the staging release from `staging`
5. Once validated, build the production release from the same `staging` commit

---

## Building a release

The step-by-step commands for staging and production, both platforms, live in
[PUBLISH_RELEASE.md](PUBLISH_RELEASE.md). The sections below are background.

---

## Generate the changelog

After bumping the version, run the `/changelog` slash command in Claude Code. It will:
- Compare the two most recent git tags
- Group commits into Features, Fixes, Style, and Chore/Infra
- Output a formatted summary ready to paste into the GitHub release

```
/changelog
```

---

## Publishing the GitHub release draft

A release holds **both platforms'** artifacts, and the mac and Windows builds
arrive independently. Publish it with the preflight script, not by clicking
Publish in the UI:

```bash
node scripts/release-preflight.js v1.4.0            # check only
node scripts/release-preflight.js v1.4.0 --publish  # check, then publish
```

It refuses to publish unless the release carries:

- **both** channel files — `latest.yml` + `latest-mac.yml`, or `staging.yml` +
  `staging-mac.yml`
- a version in each that matches the tag
- every file those manifests reference, plus the `.blockmap` beside each Windows
  installer

Then paste the `/changelog` output into the description on GitHub.

**Why not just click Publish.** `electron-updater` resolves the newest release
from the atom feed and then demands *that tag's own* channel file, throwing
`ERR_UPDATER_CHANNEL_FILE_NOT_FOUND` on a 404 with no fallback to an older
release. Publishing with one platform's manifest missing therefore does not mean
"no update available" on the other platform — it means **every update check on
every installed client hard-errors**, for as long as that release stays newest.

The UI still works and nothing prevents it, so the `verify` job in
`release-windows.yml` runs the same check after the fact on every publish and
tag push. A red run there means a live release is half-populated: attach what is
missing, it needs no re-publish.

---

## Production release

Built from the same `staging` commit once the staging build is validated — steps
in [PUBLISH_RELEASE.md](PUBLISH_RELEASE.md).

Both release scripts pass `--target staging` through to `gh release create`, so a
release created for a new tag lands that tag on `staging`. Without it, GitHub
tags the repo's default branch (`development`) — a tree whose `package.json` may
still hold the pre-bump version. It has no effect when the release already
exists, which it will whenever the Windows build got there first.

`fresh-export` also aborts if the local `package.json` version doesn't match
`origin/staging`, which is the guard against building production from the wrong
branch. Override with `SKIP_RELEASE_BRANCH_CHECK=1` only when you know why.

Then follow the same publish steps above on the production draft.

---

## Windows artifacts

Windows installers cannot be built on a Mac — the native audio addon needs MSVC
and the Windows SDK, and node-gyp cannot cross-compile that from darwin. The
`.github/workflows/release-windows.yml` workflow builds them on a hosted
`windows-2022` runner and attaches them to the same GitHub release the mac side
uses. Either platform may get there first.

| | Production | Staging |
|---|---|---|
| Installer | `release/Sayso-{version}-x64-win.exe` + `.blockmap` | `release-staging/Sayso-Beta-{version}-x64-win.exe` + `.blockmap` |
| Channel file | `latest.yml` | `staging.yml` |
| Icon | `assets/icon.ico` | `assets/icon-staging.ico` |
| appId | `com.asksayso.app` | `com.asksayso.app.staging` |
| NSIS GUID | `1df72287-21cc-56ef-85b6-db3bb8892905` | `f22c30f9-0b55-5c67-bd9a-e6b2879172ba` |
| Install dir | `%LOCALAPPDATA%\Programs\Sayso` | `%LOCALAPPDATA%\Programs\sayso-app-staging` |
| Shortcut | `Sayso` | `Sayso [beta]` |
| Uninstall entry | `Sayso {version}` | `Sayso [beta] {version}` |

The `.blockmap` is not optional. `electron-updater` uses it to download only the
changed chunks of an installer, and looks for it next to the `.exe`.

The two channels install side by side. Every name and registry key above is a
default that falls out of the differing `appId` and `productName` — the NSIS
GUID is `uuid5(appId)`, and it keys both `HKCU\Software\{GUID}` and the
uninstall entry. `build.staging.js` needs no `nsis` block to get any of it.

The one thing that does *not* fall out of `appId` is `updaterCacheDirName`,
`APP_PACKAGE_NAME` and the install dir — electron-builder derives those from
package.json `name`. That is why `build.staging.js` sets
`extraMetadata.name = 'sayso-app-staging'`: without it both channels write
`%LOCALAPPDATA%\sayso-app-updater\installer.exe`, and since NSIS copies the
running installer there on every install, each channel destroys the other's
differential-update base — both then re-download the full ~97 MB every time.
It also keeps a beta uninstall from targeting production's `%APPDATA%\sayso-app`
if `deleteAppDataOnUninstall` is ever turned on. The value is chosen to match the
`app.setName()` call in `electron/main.ts`, so code running after that line sees no
change. **One thing runs before it:** `Sentry.init` at `main.ts:36`, and
`@sentry/electron` defaults `release` to `${app.name}@${version}`. That is why
`electron/sentry.config.ts` pins `release` to the literal `sayso-app@<version>`
instead of letting it derive — otherwise staging's main process would report a
different release from its renderers and the two halves of an incident would not
correlate (SAYSO-355).

**Upgrading a beta installed before SAYSO-404: uninstall it first.** NSIS restores
`$INSTDIR` from the registry on upgrade, then `instFilesPre` appends `APP_FILENAME`
if the restored path does not already contain it — and it still does this on a silent
or `--updated` run. An old beta sits in `\Programs\sayso-app`, which does not contain
`sayso-app-staging`, so the app would end up one directory deeper at
`\Programs\sayso-app\sayso-app-staging`. Nothing in the wild is affected — no release has
ever carried a Windows asset, so every Windows beta is a hand-built local one — but
uninstall before re-installing on a dev machine. Fresh installs are unaffected.

### Regenerating the Windows icons

`win.icon` alone drives the app exe, the installer **and** the uninstaller —
NSIS takes `MUI_ICON`/`MUI_UNICON` from it unless `assets/installerIcon.ico`
exists, which it does not. So there is one icon to regenerate per channel.

Doing it is fiddlier than it looks, for two reasons worth not rediscovering:

- **`assets/icon*.icns` are a single 1024×1024 JPEG-2000 `ic10` chunk.** Nothing
  on Windows decodes that out of the box.
- **`app-builder icon --format ico` is not the tool**, even though it is
  electron-builder's own converter. It shells out to `opj_decompress` for
  JPEG-2000, which ships only for `linux/amd64` and is absent from the Windows
  `winCodeSign` bundle; its other path reads `is32/il32/ih32/icp6/it32/ic08/ic09`
  but not `ic10`. And it emits a **single 256×256 entry** regardless. Pointing
  `win.icon` at an `.icns` hits that same code path, so that does not work either.

Hence `assets/src/icon-1024.png` and `assets/src/icon-staging-1024.png`: the
artwork in a format any toolchain can open. Regenerate from those, not from the
`.icns`. The `.ico` must carry **16/32/48/64/128/256 at 32bpp, PNG-compressed**
— match `assets/icon.ico`. A conforming `.ico` is embedded byte for byte, so
you can verify by searching the built `.exe` for each frame's bytes.

```python
# pip install pillow
from PIL import Image
im = Image.open("assets/src/icon-staging-1024.png").convert("RGBA")
im.save("assets/icon-staging.ico", format="ICO",
        sizes=[(16,16),(32,32),(48,48),(64,64),(128,128),(256,256)])
```

The PNGs themselves came out of the `.icns` with the same library, which does
read JPEG-2000 (`Image.open("assets/icon-staging.icns")` — check
`PIL.features.check_codec("jpg_2000")` first). On a Mac, `sips -s format png`
does the same job.

### When it runs

| Trigger | What it does |
|---|---|
| `release: published` | Fires when you publish the draft, or create a release without one. |
| `push` of a `v*` tag | The same Publish (which creates the tag), and hand-pushed tags. |
| `workflow_dispatch` | Manual. Uploads a workflow artifact only, unless you tick `upload_to_release`. Pick the channel from a dropdown. |

The first two overlap on purpose. Whichever fires first builds; the second finds
the assets already attached to the release and exits without starting a Windows
runner.

**Both automatic triggers fire at Publish, not before.** GitHub emits no
`release` event at all for a *draft*, so nothing can run off the draft that
`fresh-export` creates — a published release is Windows-less for the ~30-60 min
the job takes, and clients polling `latest.yml` in that window get a 404. To
avoid that, kick the build off yourself as soon as `fresh-export` finishes, then
publish once it lands:

```bash
gh workflow run release-windows.yml --ref staging \
  -f channel=production -f upload_to_release=true    # -f channel=staging for beta
gh run watch "$(gh run list --workflow=release-windows.yml -L1 --json databaseId --jq '.[0].databaseId')"
```

The channel comes from the tag: `v1.4.0` builds production, `v1.4.0-staging`
builds staging.

**Either platform may run first.** The workflow and both mac scripts upload
through `scripts/release-upload.js`, which creates the draft when the tag has no
release and joins the existing one when it does. A Windows-first run no longer
breaks the mac `fresh-export` at its final step, after the whole notarize cycle
— which is what SAYSO-403 fixed.

**Assets are only ever added to a published release, never replaced.** Once a
release is out, users may already hold those bytes and `latest.yml`'s `sha512`
points at them. Uploading into a *draft* still replaces freely. The upload
script picks between the two itself.

**A `verify` job runs after every publish and tag push** and fails the run if the
release is missing either platform's channel file. It cannot block the publish —
GitHub has no such hook — so it is the backstop for a release published from the
UI, while `release-preflight.js --publish` is the gate that stops it happening.

It runs with `--only-if-published`, so a **draft** is skipped: nothing has
shipped out of one, and a half-filled draft is the normal state while the two
platforms arrive. That matters because a pushed `npm version` tag builds Windows
days before any mac build exists — it now leaves a draft for the mac side to
join and stays green, rather than going red over a release nobody can resolve.

Everything derives from `package.json`, never from the tag. A tag that disagrees
with the `package.json` on the ref being built fails the run before any compile
— the same guard `rebuild-and-package.sh` applies to the branch.

Note where each trigger reads the workflow from: `release` events use the copy on
the default branch (`development`), tag pushes use the copy in the tagged tree
(`staging`), and `workflow_dispatch` uses the copy on whatever `--ref` you give
it. Both branches need this file before any of it works — and `development` in
particular, or the workflow is not listed for dispatch at all.

### Required secrets

| Secret | Contents |
|---|---|
| `ENV_PRODUCTION` | The full text of `.env.production` |
| `ENV_STAGING` | The full text of `.env.staging` |
| `SENTRY_AUTH_TOKEN` | Sentry token, for the sourcemap upload |
| `AZURE_TENANT_ID` | Authenticode signing — see below |
| `AZURE_CLIENT_ID` | Authenticode signing — see below |
| `AZURE_CLIENT_SECRET` | Authenticode signing — see below |

No `.env*` file is in git, so a clean CI checkout has none. Vite bakes the
`VITE_*` values into the renderer and its `copy-env-files` plugin drops the file
into `dist/` and `electron/`, from where `main.ts` reads it at runtime inside the
asar. Without the secret the app builds fine and then points at nothing.

Two rules for those secrets:

- **They must be self-contained.** Vite loads `.env` *and* `.env.<mode>`, and a
  runner has no `.env`. A local `.env.staging` that only overrides the backend
  URL works on your laptop and produces a broken build in CI. `ENV_STAGING` needs
  every variable, not just the ones that differ from production.
- **Never put `GH_TOKEN` or `SENTRY_AUTH_TOKEN` in them.** That file is packed
  into the asar and ships to users. The release upload uses the workflow's own
  token and the Sentry plugin reads its own environment variable. The workflow
  fails the run if it finds either line.

### Signing

Windows builds are Authenticode-signed through **Azure Artifact Signing** (the
service Microsoft used to call Trusted Signing), as `AskSayso, Inc.` — the same
legal entity as the Apple identity.

Since the June 2023 CA/Browser Forum baseline change the private key must live on
FIPS 140-2 Level 2 hardware, so there is no `.pfx` anywhere and none can be
produced. The key is non-exportable inside Azure's HSM. That rules out
electron-builder's built-in signtool path, so `build.win.sign` points at
`scripts/sign-windows.js`, which shells out to `signtool` with the Trusted
Signing dlib. electron-builder calls that hook once per artifact it signs — the
app exe, `resources/elevate.exe`, the NSIS uninstaller and the installer — so the
executable *inside* the installer is signed too, which signing the finished
installer alone would not achieve.

Signing runs **only in CI**. There are no credentials on a laptop by design: a
local `npm run package` prints a warning and produces an unsigned installer,
while the same missing credentials in CI fail the build rather than shipping one
silently.

| Non-secret, in `scripts/sign-windows.js` | |
|---|---|
| Endpoint | `https://eus.codesigning.azure.net/` |
| Account / profile | `AskSayso` / `sayso-windows-prod` |
| Certificate CN | `AskSayso, Inc.` |

The full subject on the issued certificate is:

```
CN="AskSayso, Inc.", O="AskSayso, Inc.", L=Avondale, S=Arizona, C=US
```

Note the CN is **quoted**, because it contains a comma. `publisherName` is still
just `AskSayso, Inc.`: `electron-updater` runs the subject through `parseDn()`,
which strips the quotes, and then — because `publisherName` itself parses to an
empty DN — compares it against the CN alone. Verified against the real
certificate, so the CN-only form is correct and does not need widening to the
full DN. Anything comparing against the *raw* subject string, though, has to
cope with those quotes.

Authentication is a service principal (`sayso-github-signing`) holding the
**Artifact Signing Certificate Profile Signer** role — Owner and Contributor do
*not* grant signing. Its three `AZURE_*` repo secrets are read by
`DefaultAzureCredential` inside the dlib; nothing in this repo touches their
values.

Four things that will otherwise cost you a day:

- **The client secret expires in 24 months** (set 2026-09-10, so ~2028-09). It
  lapses as an authentication failure, not as anything that says "expired".
- **Timestamping is mandatory.** Artifact Signing issues 72-hour certificates
  that rotate automatically. An untimestamped signature stops validating three
  days later, including on installers already downloaded. That is what the `/tr`
  flag in the hook is for, and the CI verify step asserts a countersignature is
  present so it cannot go missing quietly.
- **`build.win.signingHashAlgorithms` must stay `["sha256"]`.** Left unset,
  electron-builder defaults to `["sha1", "sha256"]` and calls the hook twice per
  file, asking first for a SHA-1 signature Azure cannot issue. The hook throws
  with a pointer here rather than surfacing an opaque Azure error.
- **Never drop `build.win.publisherName` to work around a signing failure.** It
  must equal the certificate CN exactly. `electron-updater` compares the two
  after downloading and fails with `ERR_UPDATER_INVALID_SIGNATURE` on a mismatch
  — but when the field is **absent** it skips verification entirely and accepts
  the update. Removing it does not disable a check, it disables the protection.

Role assignments take a few minutes to propagate, so a 403 on the very first run
after any permission change is expected. The hook retries three times with
backoff; if it still fails, wait and re-run before debugging.

Changing the certificate identity after Windows users are on signed builds
breaks verification for the whole installed base. Treat the CN as permanent.

### Building by hand

Still possible on a Windows machine, and the fallback if the runner is down:

```bash
# Any shell. `npm run clean` is still `rm -rf`, so run that part from Git Bash.
npm run build:electron
npm run build         && npx electron-builder --win --publish never                          # production
npm run build:staging && npx electron-builder --win --config build.staging.js --publish never # staging
```

The renderer must be rebuilt between the two. Vite bakes the `VITE_*` values in
at build time, so packing a production `dist/` with `build.staging.js` produces a
`Sayso [beta]` installer that talks to the production backend — `build_env` would
be the only thing about it that is staging.

Attach the three files per channel with:

```bash
node scripts/release-upload.js --tag v1.4.0   release/Sayso-1.4.0-x64-win.exe release/Sayso-1.4.0-x64-win.exe.blockmap release/latest.yml
```

It creates the release if there is none, clobbers into a draft, and only fills
gaps in a published one — replacing an asset users may have downloaded would
invalidate the `sha512` in the manifest beside it.

### Toolchain pins

The workflow pins `python-version: '3.11'` rather than `'3.x'`. The lockfile in
`electron/native-audio` resolves `node-gyp@9.4.1`, whose bundled gyp does an
unguarded `from packaging.version import Version`, and `actions/setup-python`
stopped shipping `setuptools`/`packaging` at 3.12. Floating that version means
`rebuild-native-win` starts failing with `ModuleNotFoundError` on an unchanged
repo the day a new CPython minor ships. Raise it only alongside a node-gyp bump.

---

## Quick reference

| Command | What it does |
|---------|-------------|
| `npm version patch` | Bump patch version + commit + tag |
| `npm version minor` | Bump minor version + commit + tag |
| `npm run fresh-export:staging` | Clean build → notarize → create GH draft (staging) |
| `npm run fresh-export` | Clean build → notarize → create GH draft (production) |
| `node scripts/release-upload.js --tag <tag> <files>` | Create-or-upload artifacts onto the release |
| `node scripts/release-preflight.js <tag> [--publish]` | Check both platforms are attached, then publish |
| `/changelog` | Generate formatted release notes from git history |
