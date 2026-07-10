#!/bin/bash

# Script to move rust-ui folder to tayo6/mistraltest repository
# Usage: ./move_to_mistraltest.sh

set -e

echo "=========================================="
echo "Moving rust-ui to tayo6/mistraltest"
echo "=========================================="

# Check if we're in the right directory
if [ ! -d "rust-ui" ]; then
    echo "Error: rust-ui directory not found!"
    echo "Please run this script from the MonoDelay-1 repository root."
    exit 1
fi

# Clone the target repository (if not already cloned)
TARGET_REPO="mistraltest"
TARGET_DIR="/tmp/${TARGET_REPO}"

if [ -d "$TARGET_DIR" ]; then
    echo "Target repository already cloned at $TARGET_DIR"
    cd "$TARGET_DIR"
    git pull origin main
else
    echo "Cloning tayo6/mistraltest..."
    git clone https://github.com/tayo6/${TARGET_REPO}.git "$TARGET_DIR"
    cd "$TARGET_DIR"
fi

# Copy the rust-ui folder
cd /workspace/tayo6__MonoDelay-1
if [ -d "$TARGET_DIR/rust-ui" ]; then
    echo "rust-ui already exists in target repo. Overwriting..."
    rm -rf "$TARGET_DIR/rust-ui"
fi

echo "Copying rust-ui folder..."
cp -r rust-ui "$TARGET_DIR/"

# Commit and push
cd "$TARGET_DIR"
git add rust-ui/
git commit -m "Add rust-ui folder from MonoDelay-1

This contains the eframe application with three interactive knobs:
- Custom Knob widget with angular gradient arcs
- Three knobs (Delay, Feedback, Mix) with 0-100 range
- 10px outer stroke for dial range indication
- HTML/WASM integration for web deployment
- GitHub Actions workflow for WASM build and preview
"

echo "Pushing to tayo6/mistraltest..."
git push origin main

echo ""
echo "=========================================="
echo "Success! rust-ui has been moved to tayo6/mistraltest"
echo "=========================================="
echo ""
echo "You can now access it at:"
echo "  https://github.com/tayo6/mistraltest/tree/main/rust-ui"
echo ""
