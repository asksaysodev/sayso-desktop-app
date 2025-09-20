# #!/bin/bash

# # Configuration - UPDATE THESE VALUES
# APPLE_ID="darbyarollins@gmail.com"
# APPLE_ID_PASSWORD="edid-trva-ydkc-kbxq"
# TEAM_ID="Y57SJLCC9H"
# APP_PATH="release/Sayso-1.0.0.dmg"

# echo "�� Starting notarization process..."

# # Check if the app exists
# if [ ! -f "$APP_PATH" ]; then
#     echo "❌ App not found at $APP_PATH"
#     echo "Please run 'npm run package' first"
#     exit 1
# fi

# echo "📦 Uploading app for notarization..."

# # Submit for notarization and capture the submission ID
# SUBMISSION_ID=$(xcrun notarytool submit "$APP_PATH" \
#     --apple-id "$APPLE_ID" \
#     --password "$APPLE_ID_PASSWORD" \
#     --team-id "$TEAM_ID" \
#     --wait \
#     --output-format json | jq -r '.id')

# if [ $? -eq 0 ] && [ "$SUBMISSION_ID" != "null" ]; then
#     echo "✅ Notarization successful!"
    
#     # Staple the notarization to the app
#     echo " Stapling notarization to app..."
#     xcrun stapler staple "$APP_PATH"
    
#     if [ $? -eq 0 ]; then
#         echo "🎉 App is now notarized and ready for distribution!"
#         echo " Notarized app: $APP_PATH"
#     else
#         echo "❌ Failed to staple notarization"
#         exit 1
#     fi
# else
#     echo "❌ Notarization failed"
#     echo " Getting detailed error log..."
#     xcrun notarytool log "$SUBMISSION_ID" \
#         --apple-id "$APPLE_ID" \
#         --password "$APPLE_ID_PASSWORD" \
#         --team-id "$TEAM_ID"
#     exit 1
# fi
