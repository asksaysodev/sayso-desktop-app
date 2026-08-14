# Playbook Window Find (⌘F)

> **Audience:** anyone touching the playbook window's search, highlighting, or block rendering.
> **Scope:** in-playbook find and cross-playbook suggestions. Renderer-only — no server, no new IPC channels.
> **Last updated:** 2026-08-14 (SAYSO-340).

---

## TL;DR

- ⌘F (Ctrl+F on Windows) opens a find bar floating over the top-right of the playbook body. Esc closes it and clears highlighting.
- Search is case-insensitive plain substring across `h2`/`p` text and every `ul` item. No regex, no fuzzy matching — a query containing `[` or `$` is matched literally.
- **A match is a set of coordinates, not a DOM node.** `findMatches` returns `{blockIndex, itemIndex, start, end}` in document order; the DOM is rebuilt from those coordinates on every render.
- **The array position *is* the match identity.** `matches[3]` is match "4/N" in the counter, is `data-match-index="3"` in the DOM, is the scroll target. Nothing carries an ID.
- When the open playbook has zero matches, the bar grows a suggestions panel listing other playbooks that match by **name**, by **content**, or both. Otherwise no panel.
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

## Highlight rendering

`formatText` has to apply **two independent, possibly overlapping** decorations to one string: the pre-existing blue `[bracket]` spans and the amber match marks. Overlapping ranges can't nest arbitrarily in HTML, so it cuts the string at every boundary of every range, leaving each segment wholly inside or wholly outside each decoration, then styles per segment:

| Segment is in | Renders as |
|---|---|
| bracket only | `<span class="playbook-highlight-bracket">` |
| match only | `<mark class="playbook-find-match">` |
| both | `<mark>` wrapping the bracket span |
| neither | plain text |

A match that *straddles* a bracket edge becomes two adjacent `<mark>`s sharing one `data-match-index`. `querySelector` takes the first, so cycling and scrolling still work; the two pills render contiguously. Known cosmetic edge, not worth complicating the segmenter.

`helpers/highlightText.tsx` is the separate, simpler renderer used for suggestion names and snippets. It deliberately does **not** apply bracket styling, so a playbook named `[Draft] FSBO` stays literal. Don't merge the two.

---

## Keyboard contract

All find keys live in one `window` keydown listener in `PlaybookWindowApp`, not on the input, so they behave the same whether focus is in the bar or back in the body. No `globalShortcut`, no menu accelerator, nothing in `main.ts`.

| Key | Behavior |
|---|---|
| ⌘F / Ctrl+F | Open and focus; if already open, re-focus and select. Ignores `e.repeat`. Closes the selector dropdown. |
| Esc | Layered: closes the dropdown if open, else closes find, else nothing. |
| Enter / ⇧Enter | Next / previous match, wrapping. When the suggestions panel is up, Enter opens the highlighted suggestion instead. |
| ⌘G / ⌘⇧G | Next / previous match (macOS find-again convention). |
| ↑ / ↓ | Move through suggestions — only when the panel is showing, so arrows still scroll the body otherwise. |

`focusToken` is a counter, not a flag: pressing ⌘F while the bar is already open changes no boolean, so nothing would re-render and the input would stay unfocused. Bumping a number always produces a new effect dependency.

---

## Cross-playbook suggestions

Shown **only** when the open playbook has zero matches — that is ticket spec and an acceptance criterion, not an oversight.

- Only `ready` playbooks qualify. The selector refuses to open the others, so a row for one would be a dead end.
- Only the **displayed** name is searched (`alias || file_name`). A hidden `file_name` behind an alias is not matched — a suggestion appearing for invisible text is baffling.
- Sort: name matches first, then by content match count descending.
- A row shows `N matches` plus a snippet, or `Name match` with no snippet when only the title matched.
- Selecting a **content** suggestion keeps the query live and jumps to that playbook's first match. Selecting a **name-only** suggestion closes the find bar — there is nothing to highlight, so leaving it open would sit on `0/0` with the panel still hovering.
- If nothing matches anywhere, the panel is replaced by a plain "No results" state.

---

## Layout

The find bar is absolutely positioned inside `.playbook-content-region`, not in the header flow, so it stays correctly placed as the M/L font settings change the header height.

While find is open, `.playbook-body` gets `padding-top` **and** `scroll-padding-top` sized to the bar. The padding is not cosmetic: without it a match on the first line sits under the bar at scroll position 0, and `scrollIntoView({ block: 'center' })` cannot help because it can't scroll past the top. `scroll-padding` additionally keeps `center` centering within the unoccluded region on short windows.

Every interactive element needs `-webkit-app-region: no-drag` — the window container is a drag region, and forgetting this makes clicks silently dead.

The body's `scrollTop` is reset when `playbook.id` changes: the scroller is the same DOM element across playbook switches, so it would otherwise keep the previous document's offset. That effect is declared *before* the match-scroll effect so the latter can override it.

---

## What we deliberately did NOT do

- **No `globalShortcut` for ⌘F.** It would steal ⌘F from every other app on the machine, including the call the user is on. Find requires window focus, like every native app.
- **No debounce.** After the single-pass rewrite this is one `indexOf`-class scan over in-memory strings per playbook. Adding latency to a tool used live on calls should follow a measurement, not a guess.
- **No query trimming.** A whitespace-only query matches every space, same as Chrome. Trimming would break phrase searches containing spaces.
- **No suggestions while the current playbook has hits.** See above — spec.
- **No client-side dedupe of `block.index`.** See invariant 5.

---

## Files

| File | What it owns |
|---|---|
| `src/playbookWindow/helpers/searchPlaybook.ts` | All matching. `findMatches`, `buildSuggestions`, `matchKey`, `indexAtOrAfter`, snippet building. Pure — no React. |
| `src/playbookWindow/helpers/blocks.ts` | `sortBlocks`, shared by search and render. |
| `src/playbookWindow/helpers/formatText.tsx` | Boundary splitting for bracket + match styling. |
| `src/playbookWindow/helpers/highlightText.tsx` | Plain range highlighting for names and snippets. |
| `src/playbookWindow/helpers/renderBlock.tsx` | Threads per-block highlight ranges into block markup. |
| `src/playbookWindow/PlaybookWindowApp.tsx` | Find state, keyboard contract, index adjustment, suggestion selection. |
| `src/playbookWindow/components/PlaybookBody.tsx` | Groups matches per block, scrolls the active one into view. |
| `src/playbookWindow/components/PlaybookFindBar.tsx` | The bar: input, counter, prev/next/close. |
| `src/playbookWindow/components/FindSuggestions.tsx` | Suggestions panel and the "No results" state. |
| `src/utils/platform.ts` | Renderer mirror of `electron/utils/platform.ts`, reading `window.sayso.platform`. Used for the ⌘F / Ctrl+F hint. |

---

## When to revisit this

- If playbooks grow large enough that per-keystroke scanning shows up in a profile — measure first, then debounce or index.
- If the "suggestions only on zero matches" rule proves wrong in use. One incidental local hit currently hides matches elsewhere.
- If the server ever emits non-unique `block.index` values (invariant 5).
- If find is wanted in the coach window too — the helpers are pure and portable; only the state and layout are playbook-specific.
