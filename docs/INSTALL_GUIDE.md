# Installing Sayso

## First-time install

1. Download the `.dmg` file for your Mac:
   - **Apple Silicon** (M1/M2/M3/M4): `Sayso-arm64-mac.dmg`
   - **Intel**: `Sayso-x64-mac.dmg`
2. Open the `.dmg` and drag **Sayso** into your **Applications** folder
3. Launch Sayso from Applications

## Updating from a previous version

When dragging to Applications, if prompted — choose **Replace**, not "Keep Both". Keeping both causes permission conflicts.

> **If you had Sayso installed before June 2026**, follow the steps in [Permissions reset after update](#permissions-reset-after-update) below before launching.

## Permissions

On first launch, Sayso will ask for two permissions — both are required:

- **Microphone** — to capture your voice during calls
- **Screen Recording** — to capture system audio from calls

If you accidentally denied a permission, go to **System Settings → Privacy & Security** and enable both for Sayso.

## Permissions reset after update

Sayso was re-signed with a new certificate in June 2026. macOS ties permissions to the certificate, so users who had the previous version installed will see permissions appear "on" but Sayso won't be able to use them. You need to clear the old entries once so macOS re-prompts with the new certificate.

**Option A — Terminal:**
```sh
tccutil reset ScreenCapture com.asksayso.app
tccutil reset Microphone com.asksayso.app
```
Then relaunch Sayso and grant both permissions when prompted.

**Option B — No terminal:**
1. **System Settings → Privacy & Security → Screen Recording** → find Sayso → toggle **off** → click Quit when prompted
2. Repeat for **Microphone**
3. Relaunch Sayso — it will prompt for both permissions fresh

This is a one-time step.

## Troubleshooting

**App won't open / "unidentified developer" warning**
Right-click the app → **Open** → Open. This only happens once.

**Permissions look enabled but Sayso says they're missing**
Follow the [Permissions reset after update](#permissions-reset-after-update) steps above. If you have two Sayso installs (e.g. "Sayso" and "Sayso 2"), delete both and reinstall fresh first.
