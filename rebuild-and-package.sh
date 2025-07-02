#!/bin/bash

echo "🔧 Rebuilding and packaging Sayso with camera fixes..."

# Clean previous builds
echo "🧹 Cleaning previous builds..."
rm -rf dist/
rm -rf release/
rm -rf node_modules/

# Reinstall dependencies
echo "📦 Reinstalling dependencies..."
npm install

# Build the app
echo "🏗️ Building the app..."
npm run build

# Package the app
echo "📦 Packaging the app..."
npm run package

echo "✅ Build complete! Check the release/ folder for your packaged app."
echo "🎥 Camera should now work properly in the packaged version!" 