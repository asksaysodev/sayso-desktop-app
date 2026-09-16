# Publishing a Release

Everything runs from a Mac. macOS is built, signed and notarized **locally**
(certificates live in the keychain). Windows is built and signed **in GitHub
Actions** on a Windows runner (Azure Artifact Signing) — you only start it. Both
land in one draft release, which you publish once both are there.

## One-time setup (Mac)

- "Developer ID Application: AskSayso, Inc." certificate in the keychain
- Notary credentials saved as the keychain profile `NotaryProfile`
- `.env.production` (GH_TOKEN, SENTRY_AUTH_TOKEN) and `.env.staging` in the repo root
- `gh auth login`

## 1. Bump and merge (skip if `staging` already has the new version)

```bash
git checkout development && git pull
npm version patch                 # or minor / major
git push origin development       # do NOT push tags (no --tags / --follow-tags)
git checkout staging && git pull && git merge development && git push origin staging
```

Never push release tags by hand: a `v*` tag push starts the Windows build from
whatever commit the tag points at. The release scripts create the tag on
`staging` when the release is published.

## 2. Build both platforms (from `staging`)

| | Staging | Production |
|---|---|---|
| Windows (CI, ~5 min) | `gh workflow run "Release Windows" --ref staging -f channel=staging -f upload_to_release=true` | same, with `-f channel=production` |
| macOS (this Mac, ~15 min) | `npm run fresh-export:staging` | `npm run fresh-export` |

Start Windows first so both build in parallel. Whichever finishes first creates
the draft and the other joins it. Follow the Windows run with `gh run watch`.

## 3. Release notes, then publish

Ask Claude Code for release notes (e.g. "release notes from 1.3.0 to 1.3.1"; uses
the `sayso-release-notes` skill), save them to `notes.md`, then replace the draft's
placeholder description and publish (don't commit `notes.md`):

```bash
gh release edit vX.Y.Z-staging --notes-file notes.md          # production: vX.Y.Z
node scripts/release-preflight.js vX.Y.Z-staging --publish    # production: vX.Y.Z
```

The preflight refuses unless both platforms are attached. **Never click Publish on
GitHub instead:** a release missing one platform breaks the update check for every
user on that platform.

## Production

Once staging is validated, repeat steps 2–3 with the production commands from
the same `staging` commit.

Background (branch flow, Windows secrets and signing internals, why the preflight
exists): [VERSIONING.md](VERSIONING.md).
