# Code Signing & Notarization Setup

This guide explains how to set up code signing and notarization credentials on a new development machine to build and release Sayso for macOS.

## Prerequisites

You'll need access to:
- **Code Signing Certificate** (`.p12` file) 
- **Certificate Password** - stored in LastPass
- **Apple ID** - `darbyarollins@gmail.com`
- **App-Specific Password** - stored in LastPass
- **Team ID** - `Y57SJLCC9H`

> **Note**: All passwords are stored securely in LastPass. Do not commit passwords to the repository.

## Step 1: Import Code Signing Certificate

1. Download the `.p12` certificate file 
2. Double-click the `.p12` file to import it
3. When prompted, enter the certificate password (from LastPass)
4. Make sure it imports to the **"login"** keychain (not "System")
5. Verify the certificate is installed:
   
   security find-identity -v -p codesigning
      You should see: `Developer ID Application: EXOMEND LLC (Y57SJLCC9H)`

## Step 2: Create Notary Profile

The Notary Profile allows submitting builds to Apple for notarization. Create it with:

xcrun notarytool store-credentials NotaryProfile \
  --apple-id darbyarollins@gmail.com \
  --team-id Y57SJLCC9H

When prompted, enter the **App-Specific Password** (from LastPass).

> **Note**: Use the App-Specific Password, NOT your regular Apple ID password.

## Step 3: Verify Setup

Verify both credentials are working:

# Check code signing certificate
security find-identity -v -p codesigning | grep "Y57SJLCC9H"

# Check notary profile
xcrun notarytool history --keychain-profile NotaryProfile

Both commands should succeed without errors.

## Step 4: Test Build

Run a test build to ensure everything works:

npm run fresh-export

This will:
1. Clean previous builds
2. Rebuild the app
3. Sign the app and DMGs with your certificate
4. Submit to Apple for notarization
5. Staple the notarization tickets

## Troubleshooting

### Certificate Not Found
- Make sure the certificate is in the "login" keychain
- Try re-importing the `.p12` file
- Verify with: `security find-identity -v -p codesigning`

### Notary Profile Not Found
- Verify the profile name is exactly `NotaryProfile` (case-sensitive)
- Try recreating it with the command in Step 2
- Check with: `xcrun notarytool history --keychain-profile NotaryProfile`

### Notarization Fails
- Check that your Apple ID has access to the Developer account
- Verify the App-Specific Password is correct and not expired
- Check notarization status: `xcrun notarytool history --keychain-profile NotaryProfile`

## How It Works

The build process (`npm run fresh-export`) uses:

1. **Code Signing**: Signs the app, native modules, and DMGs using the Developer ID certificate
2. **Notarization**: Submits signed artifacts to Apple's Notary Service for validation
3. **Stapling**: Attaches the notarization ticket to the app/DMG for offline verification

Both steps are required for macOS apps distributed outside the App Store.

## Security Notes

- **Never commit** `.p12` files or passwords to git
- Store all sensitive credentials in LastPass
- The Team ID and Apple ID email are safe to include in documentation (they're already in the codebase)
- Rotate App-Specific Passwords periodically