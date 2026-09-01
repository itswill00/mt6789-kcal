#ifndef EMBEDDED_CHECKSUMS_H
#define EMBEDDED_CHECKSUMS_H

typedef struct {
    const char *rel_path;
    const char *expected_sha256;
} file_checksum_t;

static const file_checksum_t g_embedded_checksums[] = {
    { "service.sh", "9aac183e13a46a1fd2309db2a3aebe676ffbfe4cc44f070dc53fd9d7ce8e0419" },
    { "module.prop", "2cf91ed48d01f2fe73905e97ef21d277590256075c6d079d2ba91d847d109d9c" },
    { "webroot/index.html", "0b38a3ff9365ffe2e43d185d45dae54ccb7d4c44c18e3f58deb84e3172f7f954" },
};

#endif /* EMBEDDED_CHECKSUMS_H */
