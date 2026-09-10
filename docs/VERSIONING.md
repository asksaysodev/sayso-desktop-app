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

## Building a staging release

```bash
npm run fresh-export:staging
```

Requires `.env.staging` to exist locally (gitignored — get it from the team).

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

After `fresh-export:staging` completes, a **draft pre-release** is created automatically on GitHub tagged `v{version}-staging`.

To publish it:

1. Go to the repo on GitHub → **Releases**
2. Find the draft tagged `v{version}-staging`
3. Click **Edit** (pencil icon)
4. Paste the `/changelog` output into the description
5. Verify the attached files (DMGs + YML)
6. Click **Publish release**

For production releases (tagged `v{version}`), same flow but uncheck **This is a pre-release** before publishing.

---

## Production release

```bash
# After the staging build has been validated
git checkout staging
git pull origin staging

npm run fresh-export   # builds production DMGs and creates GitHub draft
```

Both release scripts pass `--target staging` to `gh release create`, so the tag
lands on `staging`. Without it, GitHub tags the repo's default branch
(`development`) — a tree whose `package.json` may still hold the pre-bump version.

`fresh-export` also aborts if the local `package.json` version doesn't match
`origin/staging`, which is the guard against building production from the wrong
branch. Override with `SKIP_RELEASE_BRANCH_CHECK=1` only when you know why.

Then follow the same publish steps above on the production draft.

---

## Windows artifacts

Windows installers cannot be built on a Mac — the native audio addon needs MSVC
and the Windows SDK, and node-gyp cannot cross-compile that from darwin. The
`.github/workflows/release-windows.yml` workflow builds them on a hosted
`windows-latest` runner and attaches them to the same GitHub release the mac
side creates. Nothing about the mac flow changes.

| Channel | Artifacts |
|---|---|
| Production | `release/Sayso-{version}-x64-win.exe` + `.blockmap`, `latest.yml` |
| Staging | `release-staging/Sayso-Beta-{version}-x64-win.exe` + `.blockmap`, `staging.yml` |

The `.blockmap` is not optional. `electron-updater` uses it to download only the
changed chunks of an installer, and looks for it next to the `.exe`.

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

**The workflow never creates a release.** If the tag has none, it fails and says
so, keeping the installer as a workflow-run artifact. That is deliberate:
`rebuild-and-package.sh` calls `gh release create` unconditionally, so a release
created here first would make `fresh-export` abort at its final step after the
whole notarize cycle. Converging the two is SAYSO-403.

**Assets are only ever added to a published release, never replaced.** Once a
release is out, users may already hold those bytes and `latest.yml`'s `sha512`
points at them. Uploading into a *draft* still replaces freely.

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

Builds from this workflow are **unsigned**. `build.win.publisherName` must equal
the exact CN of the Authenticode certificate the installer is signed with.
`electron-updater` downloads the new installer, then compares the two, and fails
the **download** with `ERR_UPDATER_INVALID_SIGNATURE` on a mismatch — the update
never reaches the install step. It **skips verification entirely if the field is
missing**, so never drop it to work around a signing failure. Until the
certificate is wired up, a Windows build installs correctly from scratch but will
be refused as an auto-update.

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

Attach the three files per channel to the draft with
`gh release upload <tag> --clobber`. Drop `--clobber` if the release is already
published — replacing an asset users may have downloaded invalidates the `sha512`
in the manifest beside it.

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
| `/changelog` | Generate formatted release notes from git history |
