#!/bin/sh
# Build script for mt6789-kcal Magisk/KernelSU module

OUTPUT="mt6789-kcal.zip"
rm -f "$OUTPUT"

chmod 755 service.sh

zip -r "$OUTPUT" module.prop service.sh webroot/ -x "*.DS_Store"

echo "Build complete: $OUTPUT"
