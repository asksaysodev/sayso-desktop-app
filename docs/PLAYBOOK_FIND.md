# Playbook Window Find (⌘F)

> **Audience:** anyone touching the playbook window's search, highlighting, or block rendering.
> **Scope:** in-playbook find and cross-playbook suggestions. Renderer-only — no server, no new IPC channels.
> **Last updated:** 2026-08-14 (SAYSO-366, superseding SAYSO-340's floating-bar UI).

---

## TL;DR

- Search lives as a persistent input in the playbook header (`playbook-search`), not a bar you have to summon. ⌘F (Ctrl+F on Windows) just focuses/selects it; Esc clears the query. There is no more open/closed state — visibility of matches and results is driven entirely by whether `query` is non-empty.
- Search is case-insensitive plain substring across `h2`/`p` text and every `ul` item. No regex, no fuzzy matching — a query containing `[` or `$` is matched literally.
- **A match is a set of coordinates, not a DOM node.** `findMatches` returns `{blockIndex, itemIndex, start, end}` in document order; the DOM is rebuilt from those coordinates on every render.
- **The array position *is* the match identity.** `matches[3]` is match "4/N" in the counter, is `data-match-index="3"` in the DOM, is the scroll target. Nothing carries an ID.
- The cross-playbook results dropdown is **live**: it shows as soon as there's a query, not just when the open playbook has zero matches (that was SAYSO-340's rule; SAYSO-366 deliberately reverses it — see "Cross-playbook suggestions" below). It's suppressed only when the open playbook already has hits and no other playbook is relevant, to avoid an empty panel for no reason.
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

All find keys live in one `window` keydown listener in `PlaybookWindowApp`, not on the input, so they behave the same whether focus is in the search field or back in the body. No `globalShortcut`, no menu accelerator, nothing in `main.ts`.

| Key | Behavior |
|---|---|
| ⌘F / Ctrl+F | Focus and select the header search input. Ignores `e.repeat`. Closes the selector dropdown. |
| Esc | Layered: closes the selector dropdown if open, else clears the query, else nothing. |
| Enter / ⇧Enter | Next / previous match, wrapping. When the results dropdown is up, Enter opens the highlighted suggestion instead. |
| ⌘G / ⌘⇧G | Next / previous match (macOS find-again convention). |
| ↑ / ↓ | Move through suggestions — only when the dropdown is showing, so arrows still scroll the body otherwise. |

`focusToken` is a counter, not a flag: pressing ⌘F while the input is already focused changes no boolean, so nothing would re-render and the input would stay as-is. Bumping a number always produces a new effect dependency. `PlaybookHeader` skips the effect on the initial `focusToken === 0` — the input is now always mounted, so a mount-time focus would steal keyboard focus the instant the window opens.

---

## Cross-playbook suggestions

**Live as of SAYSO-366**, reversing SAYSO-340's original "only when the open playbook has zero matches" rule. `buildSuggestions` now runs for any non-empty query regardless of local match state — a query can be relevant to another playbook even while the open one also has hits.

`showResults` in `PlaybookWindowApp` is `query.length > 0 && (hasNoMatches || suggestions.length > 0)`: the dropdown is suppressed only in the specific case where the open playbook already has matches *and* no other playbook has anything — an empty "no other results" panel would be pure noise there. It's still shown for the true zero-everywhere case (the original "No results" empty state survives unchanged).

- Only `ready` playbooks qualify. The selector refuses to open the others, so a row for one would be a dead end.
- Only the **displayed** name is searched (`alias || file_name`). A hidden `file_name` behind an alias is not matched — a suggestion appearing for invisible text is baffling.
- Sort: name matches first, then by content match count descending.
- A row shows `N matches` plus a snippet, or `Name match` with no snippet when only the title matched.
- Selecting **any** suggestion — content or name-only — jumps to that playbook and clears the query, closing the dropdown. It used to stay live for content hits, but that left the dropdown open (now listing whatever still matches, possibly including the playbook you just left) hovering over the playbook you just navigated to.
- If nothing matches anywhere, the panel is replaced by a plain "No results" state.

**Why this matters for layout:** because the dropdown can now appear while the open playbook *also* has an active highlighted match, it's no longer structurally impossible for the dropdown to cover that match (under SAYSO-340's rule this couldn't happen — the panel only ever showed when there was nothing highlighted to cover). See Layout below for how this is handled.

---

## Layout

The search input lives in normal document flow in the header (`PlaybookHeader`), not floating over the body — it no longer needs any headroom compensation itself. The counter and prev/next buttons render inline in that same header row when there's a query, sharing space with the title (see the flex-basis/min-width tuning on `.playbook-title-trigger` / `.playbook-search-field` — both need a floor so a long alias or a long query doesn't crush the other to unreadable).

The results dropdown (`.playbook-search-results`) is absolutely positioned below `.playbook-header-region`, the same pattern as `.playbook-selector`. Being absolutely positioned, it floats over the top of `.playbook-content-region` when shown — and because suggestions are now live (see above), it can do this while the body has an active highlighted match.

`.playbook-body` reserves room for this with `padding-top` / `scroll-padding-top` driven by a `--pbw-search-offset` CSS custom property, set inline on `.playbook-content-region` from `PlaybookWindowApp`. Unlike SAYSO-340's fixed constant (sized for a single-row bar), this is **measured**: a ref on the results wrapper reads `offsetHeight` in a `useLayoutEffect` (so it's set before paint, no flash of overlapped content) whenever `showResults`/`suggestions`/`hasNoMatches` change, and `0` when nothing is showing. `padding-top` transitions (180ms ease) so the reserved space grows/shrinks smoothly instead of snapping — the dropdown's height varies a lot with result count, so a hard jump would be jarring. `scroll-padding-top` is set to the same value but isn't animated (it only affects `scrollIntoView` math at call time, not paint).

Every interactive element needs `-webkit-app-region: no-drag` — the window container is a drag region, and forgetting this makes clicks silently dead.

The body's `scrollTop` is reset when `playbook.id` changes: the scroller is the same DOM element across playbook switches, so it would otherwise keep the previous document's offset. That effect is declared *before* the match-scroll effect so the latter can override it.

---

## What we deliberately did NOT do

- **No `globalShortcut` for ⌘F.** It would steal ⌘F from every other app on the machine, including the call the user is on. Find requires window focus, like every native app.
- **No debounce.** After the single-pass rewrite this is one `indexOf`-class scan over in-memory strings per playbook. Adding latency to a tool used live on calls should follow a measurement, not a guess.
- **No query trimming.** A whitespace-only query matches every space, same as Chrome. Trimming would break phrase searches containing spaces.
- **No client-side dedupe of `block.index`.** See invariant 5.
- **No fixed-height headroom constant for the results dropdown.** SAYSO-340 could get away with one because it only ever compensated for a single-row bar. Now that the dropdown's height varies with result count, a fixed constant would either waste space (short lists) or under-reserve (long lists) — see Layout.

---

## Files

| File | What it owns |
|---|---|
| `src/playbookWindow/helpers/searchPlaybook.ts` | All matching. `findMatches`, `buildSuggestions`, `matchKey`, `indexAtOrAfter`, snippet building. Pure — no React. |
| `src/playbookWindow/helpers/blocks.ts` | `sortBlocks`, shared by search and render. |
| `src/playbookWindow/helpers/formatText.tsx` | Boundary splitting for bracket + match styling. |
| `src/playbookWindow/helpers/highlightText.tsx` | Plain range highlighting for names and snippets. |
| `src/playbookWindow/helpers/renderBlock.tsx` | Threads per-block highlight ranges into block markup. |
| `src/playbookWindow/PlaybookWindowApp.tsx` | Find state, keyboard contract, index adjustment, suggestion selection, the results-offset measurement effect. |
| `src/playbookWindow/components/PlaybookHeader.tsx` | Title trigger + the search field, counter, prev/next, close — all in the header row now. |
| `src/playbookWindow/components/PlaybookBody.tsx` | Groups matches per block, scrolls the active one into view. |
| `src/playbookWindow/components/FindSuggestions.tsx` | Results dropdown and the "No results" state. Positioned by the wrapping `.playbook-search-results` in `PlaybookWindowApp`, not by anything in this file. |
| `src/utils/platform.ts` | Renderer mirror of `electron/utils/platform.ts`, reading `window.sayso.platform`. Used for the ⌘F / Ctrl+F hint. |

---

## When to revisit this

- If playbooks grow large enough that per-keystroke scanning shows up in a profile — measure first, then debounce or index.
- If the header feels cramped on narrow windows: `.playbook-title-trigger` (min 64px) and `.playbook-search-field` (min 76px) both have floors tuned against a ~322px header content width. A narrower default window size would need these re-tuned.
- If the server ever emits non-unique `block.index` values (invariant 5).
- If find is wanted in the coach window too — the helpers are pure and portable; only the state and layout are playbook-specific.
