# CRM Integration Design

## Goal

Link Sayso coach sessions to leads in connected CRMs (starting with Follow Up Boss) without relying on fragile timing heuristics or assumptions about agent workflow order.

## The Core Problem

Sayso sessions and CRM calls are independent event streams with no shared key. Agents may start Sayso before or after initiating a call, and during bulk calling the order is unpredictable. Any approach that infers the link by timing or session order will have edge cases that break silently in production.

## Design

Two layers that cover each other's failure modes:

### Layer 1 — Manual retroactive linking (web app)
Agents can link any session to a lead from the Sayso web app after the fact. This is the safety net for missed captures and requires no real-time coordination.

### Layer 2 — Capture button (widget)
A button in the coach widget (and a keyboard shortcut) lets the agent explicitly trigger a lead capture at any point during a call. Explicit user intent replaces all timing heuristics.

**Flow:**
```
Agent presses button/shortcut
  → Sayso captures the active/focused window (not full screen)
  → OCR extracts candidate: name, phone number, or email
  → POST to server with raw OCR text + session ID
  → Server queries CRM /people (or equivalent)
  → 1 match   → auto-link, confirm in widget ("Linked to Jane Smith")
  → N matches → show mini picker in widget, agent selects
  → 0 matches → show "not found" + fallback to manual entry or web app
```

Server validation is the correctness gate — OCR errors don't silently link the wrong person.

## CRM Abstraction Layer

The server normalizes across CRMs so the widget is CRM-agnostic:

```
CaptureService.findLead(rawOcrText)
  → FUBAdapter.searchPeople(text)      // Follow Up Boss
  → HubSpotAdapter.searchContacts(text) // future
  → ...
```

Each adapter implements the same interface. Adding a new CRM means adding a new adapter without touching the widget or capture logic.

## Follow Up Boss API Notes

- Auth: HTTP Basic with API key (Admin > API), or OAuth with Bearer token
- Agent-level API key only accesses assigned contacts; broker-level is account-wide
- Relevant endpoints: `GET /people?query=<text>`, `POST /calls`, `POST /notes`
- Webhooks exist (`callsCreated`, `callsUpdated`) but fire when FUB logs the call — typically after the call ends — so they are not useful for real-time session linking. They are useful for syncing Sayso session data back to FUB post-call.

## What Was Ruled Out

**Webhook-based real-time linking** — `callsCreated` fires when FUB logs the call (usually post-call), not at call start. Unreliable for forward linking.

**Time-window reconciliation** — Matches session timestamps against FUB call records after the fact. Works for single calls but becomes ambiguous during bulk calling and fails silently.

**Audio trigger ("hello" detection)** — False positives, language variation, and the CRM screen may have already navigated away from the lead by the time the trigger fires.

**Full-screen OCR on session start** — Cannot guarantee the right lead is on screen at that moment, especially during bulk calls.

## Screen Capture Detail

Use ScreenCaptureKit window-level capture (already in use) targeting the active/focused window only. This scopes OCR to the CRM tab rather than picking up unrelated content from other windows.

## Future Considerations

- Confidence scoring: auto-link on high confidence, prompt on low confidence
- CRM URL parsing as a secondary signal (e.g., `app.followupboss.com/contacts/12345`)
- Dialer-specific integrations (Kixie, Mojo, FUB Phone) for call-start events if reliability needs to be raised further
