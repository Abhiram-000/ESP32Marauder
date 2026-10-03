#!/usr/bin/env bash
# Build + flash ESP32 Marauder for the ESP32-S3-DevKitC-1
# (1.8" ST7735S 160x128, 5 buttons, WS2812, 16MB flash / 8MB octal PSRAM)
#
# Requirements (matching the project's CI formula for S3-with-screen boards):
#   arduino-cli core install esp32:esp32@2.0.17
#   arduino-cli lib install NimBLE-Arduino@1.4.1
#   TFT_eSPI 2.5.34 in ~/Arduino/libraries (this board's User_Setup lives here)
#
# IMPORTANT: S3 + TFT requires core 2.0.x. Core 3.x (IDF5) has an S3 SPI
# register-macro incompatibility with TFT_eSPI 2.5.34 that panics in
# tft.init(). This is why the project's CI pins all S3 TFT boards to 2.0.11.
#
# Usage: ./build_s3_devkitc.sh [upload]
set -e
cd "$(dirname "$0")"

TFT_DIR="$HOME/Arduino/libraries/TFT_eSPI"
FQBN="esp32:esp32:esp32s3:FlashSize=16M,PartitionScheme=default_8MB,PSRAM=opi,CDCOnBoot=cdc"

# 1. Point TFT_eSPI at this board's display setup
if [ -d "$TFT_DIR" ]; then
  sed -i 's|^#include <User_Setup_s3_devkitc.h>|//#include <User_Setup_s3_devkitc.h>|; s|^#include <User_Setup_s3_devkitc.h>.*|//#include <User_Setup_s3_devkitc.h>|' "$TFT_DIR/User_Setup_Select.h" 2>/dev/null || true
  sed -i 's|^//#include <User_Setup_s3_devkitc.h>.*|#include <User_Setup_s3_devkitc.h>|' "$TFT_DIR/User_Setup_Select.h"
fi

# 2. Custom 16MB partition table: the sketch-dir partitions.csv overrides the
#    FQBN scheme on core 2.0.x (prebuild hook). 3MB OTA slots + ~10MB SPIFFS.
cp installer/partitions/s3_devkitc_16mb.csv esp32_marauder/partitions.csv

# 3. Compile
arduino-cli compile \
  --fqbn "$FQBN" \
  --build-property "compiler.cpp.extra_flags=-DMARAUDER_S3_DEVKITC" \
  --library libraries/ESPAsyncWebServer \
  --warnings none \
  --output-dir "Release Bins/s3_devkitc" \
  esp32_marauder/esp32_marauder.ino

rm -f esp32_marauder/partitions.csv

# 4. Optionally upload. NOTE: pieces are flashed individually because
#    arduino-cli does not regenerate merged.bin in the output dir.
if [ "$1" == "upload" ]; then
  PORT=${PORT:-$(ls /dev/ttyACM* 2>/dev/null | head -1)}
  if [ -z "$PORT" ]; then echo "No /dev/ttyACM* port. Hold BOOT, tap RESET, release BOOT."; exit 1; fi
  BOOT_APP0=$(find ~/.arduino15/packages/esp32/hardware/esp32/2.0.17/tools/partitions -name "boot_app0.bin" | head -1)
  esptool --chip esp32s3 --port "$PORT" --baud 460800 --before default_reset --after hard_reset \
    write_flash -z \
    0x0     "Release Bins/s3_devkitc/esp32_marauder.ino.bootloader.bin" \
    0x8000  "Release Bins/s3_devkitc/esp32_marauder.ino.partitions.bin" \
    0xe000  "$BOOT_APP0" \
    0x10000 "Release Bins/s3_devkitc/esp32_marauder.ino.bin"
  echo "Flashed $PORT. Board rebooted."
else
  echo "Build ready in 'Release Bins/s3_devkitc/'. Run with 'upload' to flash."
fi
