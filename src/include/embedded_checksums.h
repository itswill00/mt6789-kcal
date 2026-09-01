#ifndef EMBEDDED_CHECKSUMS_H
#define EMBEDDED_CHECKSUMS_H

typedef struct {
    const char *rel_path;
    const char *expected_sha256;
} file_checksum_t;

static const file_checksum_t g_embedded_checksums[] = {
    { "service.sh", "9aac183e13a46a1fd2309db2a3aebe676ffbfe4cc44f070dc53fd9d7ce8e0419" },
    { "module.prop", "2cf91ed48d01f2fe73905e97ef21d277590256075c6d079d2ba91d847d109d9c" },
    { "webroot/index.html", "10d96004c807da218c09fbf6217a110c6d858b01959c2fcc0def779a5afda651" },
};

#endif /* EMBEDDED_CHECKSUMS_H */
