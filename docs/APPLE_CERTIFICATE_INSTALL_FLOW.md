# Code Signing & Notarization Setup

This guide explains how to set up code signing and notarization credentials on a new development machine to build and release Sayso for macOS.

## Account Details

| | |
|---|---|
| **Apple ID** | kuvaalpatel@gmail.com (team owner) |
| **Team ID** | AFGHD8M3VK |
| **Legal entity** | AskSayso, Inc. |
| **Certificate name** | `Developer ID Application: AskSayso, Inc. (AFGHD8M3VK)` |
| **App ID** | `com.asksayso.app` |

> **Credentials**: The `.p12` certificate file, `.p12` password, and App-Specific Password are all stored in the **"Sayso Apple Certificate"** Safe Note in LastPass. Do not commit any of these to the repository.

---

## One-time setup (certificate owner only)

This section only needs to be done once — by whoever manages the Apple Developer account — to create the certificate and export it for the team.

### 1. Create the Developer ID Application certificate

1. Go to [developer.apple.com](https://developer.apple.com) → **Certificates, Identifiers & Profiles** → **Certificates**
2. Click **+** to create a new certificate
3. Select **Developer ID Application** → Continue
4. Generate a Certificate Signing Request (CSR) on your Mac:
   - Open **Keychain Access** → menu bar **Keychain Access → Certificate Assistant → Request a Certificate from a Certificate Authority**
   - Fill in your email and a common name (e.g. "Sayso Signing")
   - Select **Saved to disk** → Continue → save the `.certSigningRequest` file
5. Upload the `.certSigningRequest` file on developer.apple.com → Continue → Download
6. Double-click the downloaded `.cer` file — it imports into Keychain Access automatically

### 2. Export the certificate as a .p12 file

1. Open **Keychain Access** → **My Certificates**
2. Find `Developer ID Application: AskSayso, Inc. (AFGHD8M3VK)`
3. Right-click → **Export** → choose `.p12` format
4. Set a strong password when prompted
5. Store the `.p12` file and its password in the **"Sayso Apple Certificate"** Safe Note in LastPass

---

## Per-machine setup (every developer who builds releases)

### Step 1: Import the code signing certificate

1. Download the `.p12` certificate file from the **"Sayso Apple Certificate"** Safe Note in LastPass
2. Double-click the `.p12` file to import it
3. Enter the `.p12` password (also in the **"Sayso Apple Certificate"** Safe Note) when prompted
4. Make sure it imports to the **login** keychain (not "System")
5. Verify the certificate is installed:

```sh
security find-identity -v -p codesigning
# Should show: Developer ID Application: AskSayso, Inc. (AFGHD8M3VK)
```

### Step 2: Generate an App-Specific Password

This is a separate Apple-generated password (not your Apple ID password, not the `.p12` password) that lets `notarytool` authenticate to Apple's servers.

The App-Specific Password is already stored in the **"Sayso Apple Certificate"** Safe Note in LastPass — retrieve it from there. If it has expired or needs to be regenerated:

1. Go to [appleid.apple.com](https://appleid.apple.com) → sign in as `kuvaalpatel@gmail.com`
2. **Sign-In and Security → App-Specific Passwords → Generate**
3. Name it "Sayso Notary" → copy the generated password (format: `xxxx-xxxx-xxxx-xxxx`)
4. Update the **"Sayso Apple Certificate"** Safe Note in LastPass with the new password

### Step 3: Create the Notary Profile

The Notary Profile stores credentials so `xcrun notarytool` can submit builds to Apple without interactive login.

```sh
xcrun notarytool store-credentials NotaryProfile \
  --apple-id kuvaalpatel@gmail.com \
  --team-id AFGHD8M3VK
```

When prompted, paste the App-Specific Password from Step 2.

### Step 4: Verify setup

```sh
# Check code signing certificate
security find-identity -v -p codesigning | grep AFGHD8M3VK

# Check notary profile
xcrun notarytool history --keychain-profile NotaryProfile
```

Both commands should succeed without errors.

### Step 5: Test build

```sh
npm run fresh-export
```

This will:
1. Clean previous builds
2. Rebuild native modules and the app
3. Sign the `.app` bundle and DMGs with the certificate
4. Submit to Apple for notarization
5. Staple the notarization tickets
6. Create a draft GitHub release

---

## Troubleshooting

### Certificate not found
- Make sure the certificate is in the **login** keychain, not System
- Try re-importing the `.p12` file
- Verify with: `security find-identity -v -p codesigning`

### Notary Profile not found
- The profile name must be exactly `NotaryProfile` (case-sensitive)
- Recreate it with the command in Step 2
- Check with: `xcrun notarytool history --keychain-profile NotaryProfile`

### Notarization fails
- Confirm the App-Specific Password hasn't expired — regenerate at [appleid.apple.com](https://appleid.apple.com)
- Verify `kuvaalpatel@gmail.com` has Admin access in the Apple Developer account
- Check submission history: `xcrun notarytool history --keychain-profile NotaryProfile`

---

## How it works

The build process (`npm run fresh-export`) uses three layers:

1. **electron-builder** — signs the `.app` bundle during packaging using `mac.identity` in `package.json`
2. **`codesign`** in `scripts/rebuild-and-package.sh` — re-signs the DMGs after electron-builder creates them
3. **`xcrun notarytool`** — submits signed artifacts to Apple's Notary Service; `xcrun stapler` then attaches the ticket to each artifact for offline Gatekeeper verification

Both signing and notarization are required for macOS apps distributed outside the App Store.

## Security notes

- **Never commit** `.p12` files or passwords to git
- Store all credentials in LastPass
- The Team ID, Apple ID, and certificate name are safe in documentation (they're already in the codebase)
- Rotate App-Specific Passwords periodically
