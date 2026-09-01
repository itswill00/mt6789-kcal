#include "sha256.h"
#include <stdio.h>
#include <string.h>

int sha256_file(const char *filepath, char output_hex[65]) {
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "sha256sum '%s' 2>/dev/null", filepath);

    FILE *pipe = popen(cmd, "r");
    if (!pipe) return -1;

    char buffer[128];
    if (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        pclose(pipe);
        if (sscanf(buffer, "%64s", output_hex) == 1) {
            output_hex[64] = '\0';
            return 0;
        }
    } else {
        pclose(pipe);
    }

    return -1;
}
