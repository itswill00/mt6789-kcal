#!/system/bin/sh
MODDIR=${0%/*}

# Wait for system boot completion
while [ "$(getprop sys.boot_completed)" != "1" ]; do
    sleep 2
done

# Ensure SurfaceFlinger service is responsive
until service check SurfaceFlinger | grep -q "found"; do
    sleep 1
done

LOGFILE="$MODDIR/calibration.log"
echo "[$(date)] MT6789 display calibration starting..." > "$LOGFILE"

# Execute native binary module
if [ -x "$MODDIR/system/bin/kcal.so" ]; then
    echo "[$(date)] Executing kcal.so..." >> "$LOGFILE"
    "$MODDIR/system/bin/kcal.so" "$MODDIR" >> "$LOGFILE" 2>&1
else
    service call SurfaceFlinger 1015 i32 1 f 1.042 f 0.000 f 0.000 f 0.000 f 0.000 f 1.018 f 0.000 f 0.000 f 0.000 f 0.000 f 1.095 f 0.000 f 0.000 f 0.000 f 0.000 f 1.000 >/dev/null 2>&1
    cmd color_display set-saturation 108 >/dev/null 2>&1
fi

# Refresh rate monitor loop
(
    while true; do
        logcat -v tag -b events,main SurfaceFlinger:I DisplayFeature:I *:S 2>/dev/null | while read -r line; do
            case "$line" in
                *refresh*|*setRefreshRate*|*displayfeature*|*screen_state*)
                    sleep 0.2
                    service call SurfaceFlinger 1015 i32 1 f 1.042 f 0.000 f 0.000 f 0.000 f 0.000 f 1.018 f 0.000 f 0.000 f 0.000 f 0.000 f 1.095 f 0.000 f 0.000 f 0.000 f 0.000 f 1.000 >/dev/null 2>&1
                    ;;
            esac
        done
        sleep 3
    done
) &

# Periodic check
(
    while true; do
        sleep 10
        service call SurfaceFlinger 1015 i32 1 f 1.042 f 0.000 f 0.000 f 0.000 f 0.000 f 1.018 f 0.000 f 0.000 f 0.000 f 0.000 f 1.095 f 0.000 f 0.000 f 0.000 f 0.000 f 1.000 >/dev/null 2>&1
    done
) &
