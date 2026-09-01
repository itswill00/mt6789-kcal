# MediaTek MT6789 Display Calibration (`mt6789-kcal`)

Hardware display color matrix calibration and KernelSU / APatch / Magisk WebUI analyzer designed for MediaTek MT6789 (Helio G99 / G100) and Android devices.

## Features
- **SurfaceFlinger 4x4 Color Transform**: Direct Binder IPC execution (`SET_COLOR_TRANSFORM` code `1015`) operating at the Hardware Composer (HWC) level.
- **Dynamic Refresh Rate Monitor**: Real-time event monitor loop ensuring color calibration remains enforced across 60Hz, 90Hz, and 120Hz dynamic refresh rate switches.
- **KCAL Sysfs Support**: Automatic detection and fallback writing to kernel sysfs nodes (`/sys/devices/platform/kcal_ctrl.0/kcal`).
- **Interactive WebUI Analyzer**: Lightweight WebRoot interface for live color tuning, real-time matrix visualization, system diagnostics, and fine-tuned presets.

## Presets Included
- **Ultra clarity (Default)**: `R: 246, G: 250, B: 256, Sat: 108%` — Crisp neutral white balance, micro-contrast enhancement, zero green/red tint.
- **Natural**: `R: 256, G: 254, B: 250, Sat: 100%` — Standard sRGB 6500K balanced white point.
- **Dynamic**: `R: 256, G: 250, B: 244, Sat: 110%` — Enhanced OLED color vibrancy and pop.
- **Warm comfort**: `R: 256, G: 246, B: 228, Sat: 100%` — Reduced blue light for night viewing comfort.
- **Cool crisp**: `R: 248, G: 252, B: 256, Sat: 100%` — Clean cool-white tone.
- **Anti-green fix**: `R: 256, G: 244, B: 240, Sat: 100%` — Target drop for low-brightness OLED green tinting.

## Installation
1. Download `mt6789-kcal.zip` from Releases or build locally using `./build.sh`.
2. Flash via **KernelSU Manager**, **APatch**, or **Magisk**.
3. Reboot device.
4. Open the module WebUI from KernelSU / APatch manager for live tuning.

## Building
```bash
chmod +x build.sh
./build.sh
```

## License
[AGPL-3.0](LICENSE)
