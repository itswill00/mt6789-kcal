#ifndef EMBEDDED_CHECKSUMS_H
#define EMBEDDED_CHECKSUMS_H

typedef struct {
    const char *rel_path;
    const char *expected_sha256;
} file_checksum_t;

static const file_checksum_t g_embedded_checksums[] = {
    { "service.sh", "977fc9c13fc76fd8b53b6f2fc4d316b41eda43fea8cc3a01f4c4ca811129966d" },
    { "module.prop", "32d4d0c7b56d084c7c4b8a33efee99efa4bd0994b5d179314545643a74f081ed" },
    { "webroot/index.html", "b68b081fe215dae5c5801bb454c0dc0479f8a62f58d5d8282d63d2044ce669a5" },
};

#endif /* EMBEDDED_CHECKSUMS_H */
