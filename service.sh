#!/system/bin/sh
MODDIR=${0%/*}

# Log initialization
log_msg() {
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] [INFO] [MT6789-KCAL] $1" >> "$MODDIR/calibration.log"
}

# Wait for boot completion
until [ "$(getprop sys.boot_completed)" = "1" ]; do
    sleep 3
done

sleep 5

log_msg "Boot completed. Executing native C binary verification..."

chmod 755 "$MODDIR/system/bin/kcal.so"
"$MODDIR/system/bin/kcal.so" "$MODDIR" >> "$MODDIR/calibration.log" 2>&1

# Refresh rate locking & event monitor loop
while true; do
    if [ -f "$MODDIR/rate_mode.txt" ]; path_val=$(cat "$MODDIR/rate_mode.txt" 2>/dev/null); then
        case "$path_val" in
            "60")
                settings put global user_preferred_refresh_rate 60.0 2>/dev/null
                settings put system miui_refresh_rate 60 2>/dev/null
                settings put secure miui_refresh_rate 60 2>/dev/null
                settings put system user_refresh_rate 60 2>/dev/null
                settings put global user_refresh_rate 60 2>/dev/null
                settings put system peak_refresh_rate 60.0 2>/dev/null
                settings put system min_refresh_rate 60.0 2>/dev/null
                cmd display set-user-preferred-display-mode 1080 2400 60.0 2>/dev/null
                ;;
            "90")
                settings put global user_preferred_refresh_rate 90.0 2>/dev/null
                settings put system miui_refresh_rate 90 2>/dev/null
                settings put secure miui_refresh_rate 90 2>/dev/null
                settings put system user_refresh_rate 90 2>/dev/null
                settings put global user_refresh_rate 90 2>/dev/null
                settings put system peak_refresh_rate 90.0 2>/dev/null
                settings put system min_refresh_rate 90.0 2>/dev/null
                cmd display set-user-preferred-display-mode 1080 2400 90.0 2>/dev/null
                ;;
            "120")
                settings put global user_preferred_refresh_rate 120.0 2>/dev/null
                settings put system miui_refresh_rate 120 2>/dev/null
                settings put secure miui_refresh_rate 120 2>/dev/null
                settings put system user_refresh_rate 120 2>/dev/null
                settings put global user_refresh_rate 120 2>/dev/null
                settings put system peak_refresh_rate 120.0 2>/dev/null
                settings put system min_refresh_rate 120.0 2>/dev/null
                cmd display set-user-preferred-display-mode 1080 2400 120.0 2>/dev/null
                ;;
        esac
    fi
    sleep 5
done
