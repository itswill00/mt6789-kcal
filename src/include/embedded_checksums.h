#ifndef EMBEDDED_CHECKSUMS_H
#define EMBEDDED_CHECKSUMS_H

typedef struct {
    const char *rel_path;
    const char *expected_sha256;
} file_checksum_t;

static const file_checksum_t g_embedded_checksums[] = {
    { "service.sh", "461e90e972749332cca43a108793c2208f1d1cdfdc581fcdd788800d3bfb293e" },
    { "module.prop", "2cf91ed48d01f2fe73905e97ef21d277590256075c6d079d2ba91d847d109d9c" },
    { "webroot/index.html", "0498b7af72c93d16e601d0943b8b22c87972f4676ab09287fabe336022c14053" },
};

#endif /* EMBEDDED_CHECKSUMS_H */
