# Playbook Window Find (⌘F)

> **Audience:** anyone touching the playbook window's search, highlighting, or block rendering.
> **Scope:** find within the playbook currently on view. Renderer-only — no server, no new IPC channels.
> **Last updated:** 2026-08-21 (SAYSO-377, reducing SAYSO-366's cross-playbook scope back to a single-document find.)

---

## TL;DR

- Search lives as a persistent input in the playbook header (`playbook-search`), not a bar you have to summon. ⌘F (Ctrl+F on Windows) just focuses/selects it; Esc clears the query. There is no open/closed state — visibility of the counter and highlights is driven entirely by whether `query` is non-empty.
- **Only the open playbook is searched.** No other playbook is scanned, and there is no results dropdown. See "What we deliberately did NOT do" — this was built twice and removed once, so read that before adding it back.
- Search is case-insensitive plain substring across `h2`/`p` text and every `ul` item. No regex, no fuzzy matching — a query containing `[` or `$` is matched literally.
- **A match is a set of coordinates, not a DOM node.** `findMatches` returns `{blockIndex, itemIndex, start, end}` in document order; the DOM is rebuilt from those coordinates on every render.
- **The array position *is* the match identity.** `matches[3]` is match "4/N" in the counter, is `data-match-index="3"` in the DOM, is the scroll target. Nothing carries an ID.
- A query typed into an empty field lands on match 1 and scrolls there. Refining a live query keeps the reader in place instead — see "Which match is active" below.
- Everything runs against `usePlaybooksCache`, which already holds every playbook with its full `blocks[]`.

---

## Invariants

These are the things that break silently. Three of them were real bugs caught in review.

**1. Search and render must gate on the same status.** `PlaybookBody` only renders blocks for `status === 'ready'`, so the `matches` memo in `PlaybookWindowApp` checks the same thing before calling `findMatches`. If they drift, the counter reads `1/7` while nothing highlights and nothing scrolls, because `querySelector('[data-match-index=…]')` finds nothing.

**2. Offsets must be taken from the original string.** `createMatcher` builds a case-insensitive regex that runs against the raw text. Do **not** "simplify" this back to `indexOf` over `text.toLowerCase()`: case folding is not length-preserving (`'İ'.toLowerCase()` is 2 code units), so one such character upstream shifts every highlight in that block.

**3. The active index is adjusted during render, never in an effect.** See the `lastSelectedId` / `lastQuery` block in `PlaybookWindowApp`. An effect would commit one stale frame — at 12 matches, step to 7, then narrow the query to 3 results, and the counter paints "8/3" with no highlight before the effect corrects it. React re-runs the component before committing, so the render-phase adjustment never reaches the DOM.

**4. The scroll effect keys on match *position*, not array identity.** `PlaybookBody` builds `activeMatchKey` from `playbook.id` + `matchKey(match)`. A `playbooks-updated` broadcast (a rename or upload in App Settings) rebuilds the `matches` array with identical contents; keying on the array would re-fire the effect and yank the reader mid-read.

**5. `block.index` is assumed unique per playbook.** It is the sort key, the React `key`, *and* the highlight-map key. Duplicates would fan highlights out to every block sharing that index. This is the server's contract; nothing validates it client-side, deliberately — a duplicate should surface as a visible bug, not be papered over.

**6. Block ordering lives in `helpers/blocks.ts`,** imported by both the search helper and `PlaybookBody`. Search order and render order must agree or match numbering goes wrong.

---

## Which match is active

Rebuilding `matches` on every keystroke means the previous active index is meaningless — there may now be three matches where there were twelve. Two different answers apply, decided in the render-phase block described in invariant 3:

| Query change | Active match |
|---|---|
| empty → non-empty (a new search) | `0` — the first match in document order, scrolled into view |
| non-empty → non-empty (refining) | `indexAtOrAfter(matches, matchAnchorRef.current)` — the first match at or after where the reader was parked |

`matchAnchorRef` holds the *coordinates* of the last active match (updated in an effect whenever `matches`/`effectiveMatchIndex` change), so refining survives the array being thrown away. Without it, typing one more character while parked at match 8 would snap the reader back to the top of the playbook mid-call.

`clearQuery` (Esc, the ✕ button, opening the selector) nulls the anchor, so the next search is unambiguously a new one. Switching playbooks resets the index to `0` outright.

---

## Highlight rendering

`formatText` has to apply **two independent, possibly overlapping** decorations to one string: the pre-existing blue `[bracket]` spans and the amber match marks. Overlapping ranges can't nest arbitrarily in HTML, so it cuts the string at every boundary of every range, leaving each segment wholly inside or wholly outside each decoration, then styles per segment:

| Segment is in | Renders as |
|---|---|
| bracket only | `<span class="playbook-highlight-bracket">` |
| match only | `<mark class="playbook-find-match">` |
| both | `<mark>` wrapping the bracket span |
| neither | plain text |

A match that *straddles* a bracket edge becomes two adjacent `<mark>`s sharing one `data-match-index`. `querySelector` takes the first, so cycling and scrolling still work; the two pills render contiguously. Known cosmetic edge, not worth complicating the segmenter.

---

## Keyboard contract

All find keys live in one `window` keydown listener in `PlaybookWindowApp`, not on the input, so they behave the same whether focus is in the search field or back in the body. No `globalShortcut`, no menu accelerator, nothing in `main.ts`.

| Key | Behavior |
|---|---|
| ⌘F / Ctrl+F | Focus and select the header search input. Ignores `e.repeat`. Closes the selector dropdown. |
| Esc | Layered: closes the selector dropdown if open, else clears the query, else nothing. |
| Enter / ⇧Enter | Next / previous match, wrapping. |
| ⌘G / ⌘⇧G | Next / previous match (macOS find-again convention). |
| ↑ / ↓ | Nothing — they scroll the playbook body, as they would with no find at all. |

`focusToken` is a counter, not a flag: pressing ⌘F while the input is already focused changes no boolean, so nothing would re-render and the input would stay as-is. Bumping a number always produces a new effect dependency. `PlaybookHeader` skips the effect on the initial `focusToken === 0` — the input is always mounted, so a mount-time focus would steal keyboard focus the instant the window opens.

---

## Layout

`.playbook-search` is a bordered, 6px-radius box in normal document flow in the header (`PlaybookHeader`), grouping the input, the `N/M` counter, the prev/next chevrons and the ✕ clear button. The border is on the *container*, not the input: `.playbook-header-actions` also holds the window close button, and without it the find controls read as loose siblings of that button. `:focus-within` brightens the border.

Nothing about find floats over the body, so `.playbook-body` reserves no headroom — it has a flat 4px `padding-top` / `scroll-padding-top`, the latter only so `scrollIntoView` doesn't butt the active match against the header.

The header row is `align-items: stretch` (with `align-self: center` on the close button, which stays a 24px square). The title trigger's hover pill is sized by `--pbw-fs-title` and the search box by `--pbw-fs-small`; those tokens scale at different rates across the S/M/L font settings, so matching the two by padding would need re-tuning per tier. Stretching makes the shorter one adopt the taller one's height at every size.

`.playbook-search-field` carries `min-width: 174px` and `.playbook-title-trigger` `min-width: 64px` — both floors tuned so a long alias and a long query can't crush each other. The 174 is 180 minus the 10px of horizontal chrome (padding + border) the box added; re-tune both together if the default window width changes.

Every interactive element needs `-webkit-app-region: no-drag` — the window container is a drag region, and forgetting this makes clicks silently dead.

The body's `scrollTop` is reset when `playbook.id` changes: the scroller is the same DOM element across playbook switches, so it would otherwise keep the previous document's offset. That effect is declared *before* the match-scroll effect so the latter can override it.

---

## What we deliberately did NOT do

- **No cross-playbook search.** SAYSO-340 added a results dropdown listing other playbooks matching the query, and SAYSO-366 made it live for every query rather than only for zero-match ones. SAYSO-377 removed it after usability review: the panel is absolutely positioned over the top of the body, its height changes as you type (the body's `padding-top` had to be measured and animated to compensate), and it competes for attention with the highlighted match the user is actually reading. On a live call that is noise. Re-adding it means re-adding the measured-offset machinery — treat that as the real cost, not the search itself, which is only a scan over `usePlaybooksCache`.
- **No `globalShortcut` for ⌘F.** It would steal ⌘F from every other app on the machine, including the call the user is on. Find requires window focus, like every native app.
- **No debounce.** This is one `indexOf`-class scan over the open playbook's in-memory strings per keystroke. Adding latency to a tool used live on calls should follow a measurement, not a guess.
- **No query trimming.** A whitespace-only query matches every space, same as Chrome. Trimming would break phrase searches containing spaces.
- **No client-side dedupe of `block.index`.** See invariant 5.
- **No zero-match messaging beyond the counter.** A query that matches nothing shows `0/0` in the counter's muted style and nothing else — no panel, no layout shift, no scroll.

---

## Files

| File | What it owns |
|---|---|
| `src/playbookWindow/helpers/searchPlaybook.ts` | All matching. `findMatches`, `matchKey`, `indexAtOrAfter`. Pure — no React. |
| `src/playbookWindow/helpers/blocks.ts` | `sortBlocks`, shared by search and render. |
| `src/playbookWindow/helpers/formatText.tsx` | Boundary splitting for bracket + match styling. |
| `src/playbookWindow/helpers/renderBlock.tsx` | Threads per-block highlight ranges into block markup. |
| `src/playbookWindow/PlaybookWindowApp.tsx` | Find state, keyboard contract, active-index adjustment. |
| `src/playbookWindow/components/PlaybookHeader.tsx` | Title trigger + the search box: input, counter, prev/next, clear, window close. |
| `src/playbookWindow/components/PlaybookBody.tsx` | Groups matches per block, scrolls the active one into view. |
| `src/utils/platform.ts` | Renderer mirror of `electron/utils/platform.ts`, reading `window.sayso.platform`. Used for the ⌘F / Ctrl+F hint. |

---

## When to revisit this

- If playbooks grow large enough that per-keystroke scanning shows up in a profile — measure first, then debounce or index.
- If the header feels cramped on narrow windows: see the `min-width` floors in Layout.
- If the server ever emits non-unique `block.index` values (invariant 5).
- If find is wanted in the coach window too — the helpers are pure and portable; only the state and layout are playbook-specific.
