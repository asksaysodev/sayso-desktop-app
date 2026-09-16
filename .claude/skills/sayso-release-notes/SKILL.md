---
name: sayso-release-notes
description: >
  Use this skill whenever generating user-facing release notes or a changelog for a
  Sayso desktop-app version from git history. Triggers include: "release notes",
  "changelog", "what changed in this version", "summarize the diff between versions",
  "what's in 1.2.x", or preparing notes before/after merging to staging. It captures
  the team's exact format (Features / Fixes / Style, with SAYSO-xxx IDs) and the rules
  for what to include vs. omit, which Claude cannot guess.
---

# Sayso Release Notes Generator

Produce a **user-facing** changelog for a Sayso desktop-app release from git commit
history between two refs.

## Repo

Run git commands from the root of this repo (`sayso-desktop-app`, the desktop-app
git repo — checked out as `client/` on some machines). Release tags are `vX.Y.Z` (e.g. `v1.2.1`, `v1.2.2`).
Staging-specific tags use the `-staging` suffix.

## Determining the range

Default range is **previous release tag → current `staging` HEAD**.

1. List tags newest-first: `git tag --sort=-creatordate | head`
2. Baseline = the previous shipped `vX.Y.Z` (not `-staging`). Target = `staging`
   (or a specific `vX.Y.Z` tag if the user names one).
3. If the user gives explicit versions, use those. If the baseline tag doesn't exist,
   ask which ref to diff against rather than guessing.

## Gathering material

```
git log <base>..<target> --no-merges --pretty=format:'%s'    # commit subjects
git log <base>..<target> --merges   --pretty=format:'%s'     # PR merges (SAYSO IDs live here)
git diff <base>..<target> --stat                              # files changed, for context
```

Extract `SAYSO-<n>` IDs from both commit subjects (`fix(SAYSO-268): ...`) and merge
PR branch names (`feature/sayso-268-...`). Collapse multiple commits/PRs for the same
SAYSO ID into a single bullet.

## Output format

Exactly these three sections, in this order. **Omit any section that has no items**
(do not show an empty "Features"). Plain text, one bullet per change:

```
Features
- <new capability the user can see> (SAYSO-xxx)

Fixes
- Fixed <user-visible problem> (SAYSO-xxx)

Style
- <visual / copy / layout change>
```

Rules:
- **User-facing only.** Write from the user's perspective ("Fixed sign-out getting
  stuck on the loading screen"), not the implementation ("gate boot on single-instance
  lock"). Translate technical commit messages into what the user experiences.
- Append the `(SAYSO-xxx)` ID when one exists; omit the parenthetical if there's no ID.
- One bullet per logical change. Merge duplicate/follow-up commits for the same issue.

## What to OMIT entirely

These are not user-facing — never include them:
- Build / packaging / signing / notarization / output-dir changes
- CI / GitHub Actions / workflow changes
- Auto-update **infrastructure** plumbing (publish target, blockmap, etc.) — but DO
  include a fix if it changes update behavior the user sees
- Version-bump commits (e.g. a bare `1.2.2` commit), dependency bumps, refactors,
  docs, and chores with no user-visible effect

When in doubt: "would a non-technical user notice or care?" If no, omit it.

## Categorizing

- **Features** — new capability or option the user can now use.
- **Fixes** — something that was broken now works; phrase as "Fixed ...".
- **Style** — visual refresh, copy change, padding/layout, new imagery — no behavior change.

A fix that also restyles goes under Fixes (the functional change wins).
