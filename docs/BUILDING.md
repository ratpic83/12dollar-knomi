# Building the Knomi Clone Firmware

This guide explains how to compile and upload the firmware to your ESP32-C3 display.

## Method 1: PlatformIO (Recommended)

### Prerequisites

1. **Arduino IDE 1.8.19** (Legacy version)
   - Download: https://www.arduino.cc/en/software
   - Direct link: https://downloads.arduino.cc/arduino-1.8.19-windows.zip

2. **ESP32 Board Support**
   - Open Arduino IDE
   - Go to `File` → `Preferences`
   - Add to "Additional Board Manager URLs":
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Go to `Tools` → `Board` → `Boards Manager`
   - Search for "ESP32" and install version 2.0.11

### Required Libraries

Install these libraries via `Sketch` → `Include Library` → `Manage Libraries`:

1. **TFT_eSPI** by Bodmer (v2.5.43 or later)
2. **ArduinoJson** by Benoit Blanchon (v6.21.3 or later)

### Configure TFT_eSPI

The TFT_eSPI library needs to be configured for the GC9A01 display:

1. Locate your Arduino libraries folder:
   - Windows: `Documents/Arduino/libraries/`
   - Mac: `~/Documents/Arduino/libraries/`
   - Linux: `~/Arduino/libraries/`

2. Navigate to `TFT_eSPI` folder

3. **Option A**: Copy the provided `User_Setup.h`
   ```bash
   cp firmware/KnomiC3/User_Setup.h ~/Documents/Arduino/libraries/TFT_eSPI/User_Setup.h
   ```

4. **Option B**: Edit `User_Setup_Select.h`
   - Comment out the default setup
   - Add: `#include <User_Setups/Setup_Custom.h>`
   - Copy `User_Setup.h` to `User_Setups/Setup_Custom.h`

### Add Custom Partition Scheme

1. Locate ESP32 package folder:
   ```
   Windows: %LOCALAPPDATA%/Arduino15/packages/esp32/hardware/esp32/2.0.11/
   Mac: ~/Library/Arduino15/packages/esp32/hardware/esp32/2.0.11/
   Linux: ~/.arduino15/packages/esp32/hardware/esp32/2.0.11/
   ```

2. Copy partition file:
   ```bash
   cp firmware/partitions/c3_partitions.csv [ESP32_FOLDER]/tools/partitions/
   ```

3. Edit `boards.txt` in the ESP32 folder:
   - Find the section `esp32c3.menu.PartitionScheme`
   - Add at the end:
   ```
   esp32c3.menu.PartitionScheme.knomi_c3=Knomi C3 (3.3MB No OTA/1MB SPIFFS)
   esp32c3.menu.PartitionScheme.knomi_c3.build.partitions=c3_partitions
   esp32c3.menu.PartitionScheme.knomi_c3.upload.maximum_size=3342336
   ```

### Configure WiFi Settings

Edit `firmware/src/WifiConfig.h`:

```cpp
#define WIFI_SSID "YourWiFiSSID"
#define WIFI_PASSWORD "YourWiFiPassword"
#define KLIPPER_IP "192.168.1.100"  // Your Klipper IP
```

### Board Settings

In Arduino IDE, configure these settings under `Tools`:

- **Board**: "ESP32C3 Dev Module"
- **USB CDC On Boot**: "Enabled"
- **CPU Frequency**: "160MHz"
- **Flash Frequency**: "80MHz"
- **Flash Mode**: "DIO"
- **Flash Size**: "4MB"
- **Partition Scheme**: "Knomi C3 (3.3MB No OTA/1MB SPIFFS)"
- **PSRAM**: "Disabled"
- **Upload Speed**: "921600"
- **Port**: Select your ESP32-C3 COM port

### Compile and Upload

**Note**: For Arduino IDE, you'll need to copy all files from `firmware/src/` to a single sketch folder named `KnomiClone/` and rename `main.cpp` to `KnomiClone.ino`.

1. Open `KnomiClone.ino` in Arduino IDE
2. Click `Verify` (✓) to compile
3. Click `Upload` (→) to flash to device
4. Open `Serial Monitor` (115200 baud) to see debug output

**Important**: The project is now primarily designed for PlatformIO. Arduino IDE support requires manual file reorganization.

## Method 2: Arduino IDE (Alternative)

### Prerequisites

1. **Visual Studio Code**
   - Download: https://code.visualstudio.com/

2. **PlatformIO Extension**
   - Install from VS Code Extensions marketplace

### Building

1. Open the `firmware` folder in VS Code
2. PlatformIO will automatically detect `platformio.ini`
3. Configure WiFi in `src/WifiConfig.h`:
   ```cpp
   #define WIFI_SSID "YourWiFiSSID"
   #define WIFI_PASSWORD "YourWiFiPassword"
   #define KLIPPER_IP "192.168.1.100"
   ```
4. Click "Build" (✓) in PlatformIO toolbar
5. Connect ESP32-C3 via USB-C
6. Click "Upload" (→) to flash

### Commands

```bash
# Build firmware
pio run

# Upload to device
pio run --target upload

# Monitor serial output
pio device monitor

# Clean build
pio run --target clean
```

## Troubleshooting

### Compilation Errors

**Error: "TFT_eSPI.h: No such file or directory"**
- Install TFT_eSPI library

**Error: "ArduinoJson.h: No such file or directory"**
- Install ArduinoJson library

**Error: "Sketch too big"**
- Ensure custom partition scheme is selected
- Check that `c3_partitions.csv` is properly installed

### Upload Errors

**Error: "Failed to connect to ESP32"**
- Hold BOOT button while connecting USB
- Try different USB cable
- Check COM port selection

**Error: "Timed out waiting for packet header"**
- Press and hold BOOT button
- Click Upload
- Release BOOT when upload starts

### Runtime Errors

**Display stays black**
- Check pin configuration in `User_Setup.h`
- Verify display connections
- Check backlight pin/power

**WiFi won't connect**
- Verify SSID and password in `WifiConfig.h`
- Check WiFi signal strength
- Ensure 2.4GHz WiFi (ESP32 doesn't support 5GHz)

**Can't connect to Klipper**
- Verify Klipper IP address
- Ensure Moonraker is running (port 7125)
- Check firewall settings
- Test with: `curl http://[KLIPPER_IP]:7125/server/info`

## Pin Configuration

If your display has different pins, edit `User_Setup.h`:

```cpp
#define TFT_MOSI 7   // SDA
#define TFT_SCLK 6   // SCL
#define TFT_CS   10  // Chip select
#define TFT_DC   2   // Data/Command
#define TFT_RST  3   // Reset
#define TFT_BL   11  // Backlight
```

Common pin configurations for ESP32-C3 displays:
- Check your display's documentation
- Use multimeter to trace pins if needed
- Some displays have pins labeled on PCB

## Memory Optimization

If you run into memory issues:

1. Reduce `UPDATE_INTERVAL` in main sketch
2. Decrease JSON buffer sizes in `KlipperAPI.cpp`
3. Disable debug output: `#define DEBUG_SERIAL false`
4. Remove unused fonts from `User_Setup.h`

## Next Steps

After successful upload:
1. Check serial monitor for connection status
2. Configure Klipper (see main README.md)
3. Test display with a print job
