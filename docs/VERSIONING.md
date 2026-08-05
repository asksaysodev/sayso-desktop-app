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

## Quick reference

| Command | What it does |
|---------|-------------|
| `npm version patch` | Bump patch version + commit + tag |
| `npm version minor` | Bump minor version + commit + tag |
| `npm run fresh-export:staging` | Clean build → notarize → create GH draft (staging) |
| `npm run fresh-export` | Clean build → notarize → create GH draft (production) |
| `/changelog` | Generate formatted release notes from git history |
