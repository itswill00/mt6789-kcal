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

static void apply_surfaceflinger_matrix() {
    // User-approved Ultra Clarity matrix (R:246, G:250, B:256, Sat:108%)
    const char *cmd = "service call SurfaceFlinger 1015 i32 1 f 1.042 f 0.000 f 0.000 f 0.000 f 0.000 f 1.018 f 0.000 f 0.000 f 0.000 f 0.000 f 1.095 f 0.000 f 0.000 f 0.000 f 0.000 f 1.000 >/dev/null 2>&1";
    system(cmd);
    system("cmd color_display set-saturation 108 >/dev/null 2>&1");
}

static void apply_kcal_sysfs() {
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
            snprintf(cmd, sizeof(cmd), "chmod 664 %s 2>/dev/null; echo '246 250 256' > %s 2>/dev/null", nodes[i], nodes[i]);
            system(cmd);
            break;
        }
    }

    if (access("/sys/devices/platform/kcal_ctrl.0/kcal_enable", F_OK) == 0) {
        system("echo 1 > /sys/devices/platform/kcal_ctrl.0/kcal_enable 2>/dev/null");
    }
}

int main(int argc, char **argv) {
    log_info("MT6789-Engine", "Starting native C engine v%s...", VERSION);

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

    log_info("MT6789-Engine", "Module verification passed. Applying Ultra Clarity calibration...");
    apply_surfaceflinger_matrix();
    apply_kcal_sysfs();

    log_info("MT6789-Engine", "Native engine calibration complete.");
    return 0;
}
