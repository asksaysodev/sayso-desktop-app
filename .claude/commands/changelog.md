Generate a changelog comparing the current version to the previous git tag.

1. Run `git tag --sort=-version:refname | head -2` to get the two most recent tags.
2. Run `git log <previous-tag>..<latest-tag> --oneline` to get all commits between them.
3. Exclude merge commits and version bump commits (lines that are just a version number like "1.1.2").
4. Group the remaining commits into these categories based on their message:
   - **Features** — new functionality (feat:, feature/, sayso-NNN with new capability, add/adds)
   - **Fixes** — bug fixes (fix:, bugfix/, fix/)
   - **Style** — UI/visual/copy changes (style/, updated styles, padding, copy, UI tweaks)
   - **Chore/Infra** — everything else (chore/, config/, docs/, gitignore, pipeline, staging setup)

   If a category has no entries, omit it.

5. Output in exactly this format:

**Features**
- Brief description of what was added (ticket reference if available)
- ...

**Fixes**
- Brief description of what was fixed
- ...

**Style**
- Brief description of visual change
- ...

Keep each line to one sentence. Strip commit hashes. Use plain language, not the raw commit message verbatim — summarize what the change does for the user.
