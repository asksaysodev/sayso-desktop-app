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

## 3. Publish

```bash
node scripts/release-preflight.js vX.Y.Z-staging --publish   # production: vX.Y.Z
```

It refuses unless both platforms are attached. **Never click Publish on GitHub
instead:** a release missing one platform breaks the update check for every user
on that platform. Then add release notes (`sayso-release-notes` skill in Claude Code).

## Production

Once staging is validated, repeat steps 2–3 with the production commands from
the same `staging` commit.

Background (branch flow, Windows secrets and signing internals, why the preflight
exists): [VERSIONING.md](VERSIONING.md).
