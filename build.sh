#!/system/bin/sh

PROJECT_DIR="/data/data/com.termux/files/home/mt6789-kcal"
RELEASE_DIR="/data/data/com.termux/files/home/mt6789-kcal/release"
INTERNAL_DIR="/sdcard/mt6789-kcal"

set -e

cd "$PROJECT_DIR"

if [ ! -f "module.prop" ]; then
    echo "error: module.prop not found"
    exit 1
fi
VERSION=$(grep '^version=' module.prop | cut -d= -f2)
VERSION_CODE=$(grep '^versionCode=' module.prop | cut -d= -f2)
GIT_HASH=$(git rev-parse --short HEAD 2>/dev/null || echo "local")
ZIP_OUT="mt6789-kcal(@itswill00)-b${VERSION_CODE}-${GIT_HASH}.zip"

echo "Building mt6789-kcal ${VERSION} (${VERSION_CODE})"

for tool in clang zip; do
    if ! command -v "$tool" >/dev/null 2>&1; then
        echo "error: $tool is not installed"
        exit 1
    fi
done

echo "Generating sha256 checksums..."
rm -f checksums.txt
for file in service.sh module.prop webroot/index.html; do
    if [ -f "$file" ]; then
        sha256sum "$file" >> checksums.txt
    fi
done

cat << 'EOF' > src/include/embedded_checksums.h
#ifndef EMBEDDED_CHECKSUMS_H
#define EMBEDDED_CHECKSUMS_H

typedef struct {
    const char *rel_path;
    const char *expected_sha256;
} file_checksum_t;

static const file_checksum_t g_embedded_checksums[] = {
EOF

while read -r hash path; do
    rel_path=$(echo "$path" | sed 's|^\./||')
    echo "    { \"$rel_path\", \"$hash\" }," >> src/include/embedded_checksums.h
done < checksums.txt

cat << 'EOF' >> src/include/embedded_checksums.h
};

#endif /* EMBEDDED_CHECKSUMS_H */
EOF

echo "Compiling native C binary..."
mkdir -p system/bin
clang -O3 -Wall -Werror \
    -DVERSION=\"${VERSION}\" \
    -I./src/include \
    src/main.c \
    src/integrity.c \
    src/sha256.c \
    src/log.c \
    -o system/bin/kcal.so

mkdir -p "$RELEASE_DIR"
rm -f "$RELEASE_DIR"/*.zip

echo "Packaging ZIP module..."
chmod 755 service.sh system/bin/kcal.so

zip -r "$RELEASE_DIR/$ZIP_OUT" \
    module.prop \
    service.sh \
    system/bin/kcal.so \
    webroot/index.html >/dev/null

# Internal storage output handling (clean up previous builds to avoid duplicates)
mkdir -p "$INTERNAL_DIR"
rm -f "$INTERNAL_DIR"/mt6789-kcal*.zip
rm -f /sdcard/Apktool_M/mt6789-kcal*.zip 2>/dev/null || true
rm -f /sdcard/mt6789-kcal*.zip 2>/dev/null || true
rm -f /sdcard/DisplayColorCalibration_Magisk.zip 2>/dev/null || true
rm -f /sdcard/Apktool_M/DisplayColorCalibration_Magisk.zip 2>/dev/null || true

cp "$RELEASE_DIR/$ZIP_OUT" "$INTERNAL_DIR/$ZIP_OUT"
cp "$RELEASE_DIR/$ZIP_OUT" "/sdcard/Apktool_M/$ZIP_OUT" 2>/dev/null || true

echo "Build finished cleanly: ${INTERNAL_DIR}/${ZIP_OUT}"

# Live deploy support without rebooting
if [ "$1" = "--deploy" ] || [ "$1" = "-d" ]; then
    echo "Deploying to live device module..."
    if su -c "
        pkill -9 -x kcal.so 2>/dev/null || true
        MOD_TARGET=\"/data/adb/modules/mt6789-kcal\"
        if [ -d \"\$MOD_TARGET\" ]; then
            mkdir -p \$MOD_TARGET/system/bin \$MOD_TARGET/webroot
            cp system/bin/kcal.so \$MOD_TARGET/system/bin/kcal.so
            cp webroot/index.html \$MOD_TARGET/webroot/index.html
            cp module.prop \$MOD_TARGET/module.prop
            cp service.sh \$MOD_TARGET/service.sh
            chmod 755 \$MOD_TARGET/system/bin/*
            chmod 755 \$MOD_TARGET/service.sh
            exec \$MOD_TARGET/system/bin/kcal.so \$MOD_TARGET
        fi
    "; then
        echo "Live deploy completed & module restarted successfully"
    else
        echo "error: deploy failed"
        exit 1
    fi
fi
