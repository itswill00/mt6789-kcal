#include "integrity.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef VERSION
#define VERSION "1.0.0"
#endif

static const char *REQUIRED_AUTHOR = "itswill00";

static int verify_author(const char *mod_dir) {
    char prop_path[512];
    snprintf(prop_path, sizeof(prop_path), "%s/module.prop", mod_dir);

    FILE *f = fopen(prop_path, "r");
    if (!f) return 1;

    char line[256];
    int author_matched = 0;

    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "author=", 7) == 0) {
            char *val = line + 7;
            val[strcspn(val, "\r\n")] = 0;
            if (strcmp(val, REQUIRED_AUTHOR) == 0) {
                author_matched = 1;
            }
        }
    }
    fclose(f);
    return author_matched ? 0 : 1;
}

static void compute_and_apply_matrix(int r, int g, int b, int sat) {
    float nr = r / 256.0f;
    float ng = g / 256.0f;
    float nb = b / 256.0f;
    float s = sat / 100.0f;

    float m00 = nr * (0.2126f + 0.7874f * s);
    float m11 = ng * (0.7152f + 0.2848f * s);
    float m22 = nb * (0.0722f + 0.9278f * s);

    char cmd[512];
    snprintf(cmd, sizeof(cmd),
        "service call SurfaceFlinger 1015 i32 1 f %.3f f 0.000 f 0.000 f 0.000 f 0.000 f %.3f f 0.000 f 0.000 f 0.000 f 0.000 f %.3f f 0.000 f 0.000 f 0.000 f 0.000 f 1.000 >/dev/null 2>&1",
        m00, m11, m22);
    system(cmd);

    char sat_cmd[128];
    snprintf(sat_cmd, sizeof(sat_cmd), "cmd color_display set-saturation %d >/dev/null 2>&1", sat);
    system(sat_cmd);
}

static void apply_calibration(const char *mod_dir) {
    char cfg_path[512];
    snprintf(cfg_path, sizeof(cfg_path), "%s/config.json", mod_dir);

    int r = 246, g = 250, b = 256, sat = 108;

    if (access(cfg_path, F_OK) == 0) {
        FILE *f = fopen(cfg_path, "r");
        if (f) {
            char buf[2048];
            size_t bytes = fread(buf, 1, sizeof(buf) - 1, f);
            buf[bytes] = '\0';
            fclose(f);

            char *glob = strstr(buf, "\"global\"");
            if (glob) {
                char *pr = strstr(glob, "\"r\"");
                char *pg = strstr(glob, "\"g\"");
                char *pb = strstr(glob, "\"b\"");
                char *ps = strstr(glob, "\"sat\"");

                if (pr) r = atoi(pr + 4);
                if (pg) g = atoi(pg + 4);
                if (pb) b = atoi(pb + 4);
                if (ps) sat = atoi(ps + 6);
            }
        }
    }

    compute_and_apply_matrix(r, g, b, sat);

    const char *nodes[] = {
        "/sys/devices/platform/kcal_ctrl.0/kcal",
        "/sys/module/kcal_val/parameters/rgb",
        "/sys/class/graphics/fb0/kcal",
        "/sys/class/graphics/fb0/rgb",
        NULL
    };

    for (int i = 0; nodes[i] != NULL; i++) {
        if (access(nodes[i], F_OK) == 0) {
            char cmd[256];
            snprintf(cmd, sizeof(cmd), "chmod 664 %s 2>/dev/null; echo '%d %d %d' > %s 2>/dev/null", nodes[i], r, g, b, nodes[i]);
            system(cmd);
            break;
        }
    }

    if (access("/sys/devices/platform/kcal_ctrl.0/kcal_enable", F_OK) == 0) {
        system("echo 1 > /sys/devices/platform/kcal_ctrl.0/kcal_enable 2>/dev/null");
    }
}

int main(int argc, char **argv) {
    log_info("MT6789-KCAL", "Initialising v%s...", VERSION);

    const char *mod_dir = "/data/adb/modules/mt6789-kcal";
    if (argc > 1) {
        mod_dir = argv[1];
    }

    // Verify Author Signature
    if (verify_author(mod_dir) != 0) {
        log_error("Security", "Author mismatch or tampered module.prop! Aborting execution.");
        return 1;
    }

    // Verify File Integrity
    if (verify_module_integrity(mod_dir) != 0) {
        log_error("Security", "Module integrity check failed! Aborting execution.");
        return 1;
    }

    log_info("MT6789-KCAL", "Module verification passed. Applying calibration...");
    apply_calibration(mod_dir);

    log_info("MT6789-KCAL", "Calibration complete.");
    return 0;
}
