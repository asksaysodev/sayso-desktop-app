# Collecting logs from a user

Internal guide for anyone asking a user to send their Sayso logs.
(`docs/INSTALL_GUIDE.md` is the user-facing document — this one is for us.)

## ⚠️ Logs from builds before 1.3.4 contain live credentials

Until SAYSO-460, **every axios failure wrote the full request headers into the log
file**, including `Authorization: Bearer <token>`. Supabase access tokens are valid
for **one hour**, and the JWT payload also carries the user's name, phone number,
company and email in the clear.

So a log file collected from a user on **1.3.3 or earlier** may hand you a working
credential for their account.

**If you receive a pre-1.3.4 log file:**

1. Treat the file as secret material — not a chat attachment, not a Downloads
   folder, not a ticket comment.
2. Check whether it is still hot: find the newest `[…]` timestamp next to a
   `Bearer ` string. Under an hour old means the token is **live**.
3. If it is live, have the user sign out and back in — that rotates the refresh
   token and invalidates the session.
4. Delete the file when you are done with it.

From **1.3.4** onward, redaction happens at the serialization boundary
(`electron/shared/redact.ts`), so `Authorization`, `Cookie`, API-key headers,
bearer tokens and bare JWTs are written as `[REDACTED]`. The first launch of 1.3.4
also **deletes every log file written by an earlier build**, so the exposure clears
itself once a user upgrades. Logs are kept for **7 days** after that.

A redacted log still contains the error name, message, stack, HTTP status, error
code and request URL — everything needed to diagnose a failure.

## Where the logs are

Two different things are both called "logs", and on macOS they are **not the same
directory**:

| File | Location | What it is |
|---|---|---|
| `sayso-<YYYY-MM-DD>.log` | `<userData>/logs/` | **The one you almost always want.** Daily app log. |
| `main.log` | `<userData>/logs/` (Windows) · `~/Library/Logs/<app>/` (macOS) | Auto-updater log (electron-log). Only for update problems. |
| `debug-startup.log` | `<userData>/` | One line per launch. Only for "it won't start". |
| `shipit-watchdog.log` | `~/Library/Logs/<app>/` (macOS only) | Update install watchdog. |

`<userData>` depends on the channel — the production app, staging and a dev build
each use their own directory:

| Channel | macOS | Windows |
|---|---|---|
| Production | `~/Library/Application Support/Sayso/` | `%APPDATA%\Sayso\` |
| Staging | `~/Library/Application Support/sayso-app-staging/` | `%APPDATA%\sayso-app-staging\` |
| Dev (unpackaged) | `~/Library/Application Support/sayso-app-dev/` | `%APPDATA%\sayso-app-dev\` |

Note the daily log is only written when `NODE_ENV=production`, which every packaged
build sets. A developer running `npm run dev` produces no daily log.

## What to ask the user for

> Could you send me today's Sayso log file?
>
> **macOS** — open Finder, press `⇧⌘G`, paste this and hit Enter, then send the
> newest `sayso-*.log`:
> ```
> ~/Library/Application Support/Sayso/logs
> ```
>
> **Windows** — press `Win+R`, paste this and hit Enter, then send the newest
> `sayso-*.log`:
> ```
> %APPDATA%\Sayso\logs
> ```

Ask for the specific day that the problem happened. There is no in-app "export
logs" button — retrieval is manual.
