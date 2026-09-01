#!/system/bin/sh

PROJECT_DIR="/data/data/com.termux/files/home/mt6789-kcal"
RELEASE_DIR="/data/data/com.termux/files/home/mt6789-kcal/release"

set -e

cd "$PROJECT_DIR"

if [ ! -f "module.prop" ]; then
    echo "error: module.prop not found"
    exit 1
fi
VERSION=$(grep '^version=' module.prop | cut -d= -f2)
VERSION_CODE=$(grep '^versionCode=' module.prop | cut -d= -f2)
ZIP_OUT="mt6789-kcal-${VERSION}.zip"

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

echo "Compiling native C engine daemon..."
mkdir -p system/bin
clang -O3 -Wall -Werror \
    -DVERSION=\"${VERSION}\" \
    -I./src/include \
    src/main.c \
    src/integrity.c \
    src/sha256.c \
    src/log.c \
    -o system/bin/libkcal_engine.so

mkdir -p "$RELEASE_DIR"
rm -f "$RELEASE_DIR/$ZIP_OUT"

echo "Packaging ZIP module..."
chmod 755 service.sh system/bin/libkcal_engine.so

zip -r "$RELEASE_DIR/$ZIP_OUT" \
    module.prop \
    service.sh \
    system/bin/libkcal_engine.so \
    webroot/index.html >/dev/null

cp "$RELEASE_DIR/$ZIP_OUT" "/sdcard/Apktool_M/DisplayColorCalibration_Magisk.zip"
cp "$RELEASE_DIR/$ZIP_OUT" "/sdcard/DisplayColorCalibration_Magisk.zip"

echo "Build finished: ${RELEASE_DIR}/${ZIP_OUT}"
