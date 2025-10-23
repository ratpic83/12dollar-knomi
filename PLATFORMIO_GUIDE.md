# PlatformIO Build Guide

## ✅ Project Structure

The project is configured for PlatformIO with the following structure:

```
firmware/
├── platformio.ini      # PlatformIO configuration
├── src/               # Source files
│   ├── main.cpp       # Main application
│   ├── WifiConfig.h   # WiFi settings (configure this!)
│   ├── DisplayDriver.h/cpp  # Display control
│   ├── KlipperAPI.h/cpp     # Klipper API client
│   ├── UIManager.h/cpp      # UI management
│   ├── SimpleUI.h/cpp       # Simple UI implementation
│   ├── LVGLDriver.h/cpp     # LVGL driver (optional)
│   └── User_Setup.h         # TFT_eSPI configuration
├── include/           # Header files
├── lib/               # Custom libraries
└── partitions/        # Custom partition scheme
```

## 🚀 Quick Start with PlatformIO

### 1. Open Project in VS Code

In VS Code, open the **firmware** folder:
```
File → Open Folder → Select: /Users/patricweidermann/Documents/dev/knomi_clone/firmware
```

### 2. Build the Firmware

Click the **Build** button in the PlatformIO toolbar (checkmark icon)

Or use the terminal:
```bash
cd /Users/patricweidermann/Documents/dev/knomi_clone/firmware
pio run
```

### 3. Connect Your ESP32-C3 Display

- Connect via USB-C cable
- PlatformIO will auto-detect the port

### 4. Upload to Device

Click the **Upload** button in the PlatformIO toolbar (arrow icon)

Or use the terminal:
```bash
pio run --target upload
```

### 5. Monitor Serial Output

Click the **Serial Monitor** button in the PlatformIO toolbar (plug icon)

Or use the terminal:
```bash
pio device monitor
```

## 📋 PlatformIO Commands

All commands should be run from the `firmware/` directory:

```bash
# Build firmware
pio run

# Upload to device
pio run --target upload

# Build and upload in one command
pio run --target upload

# Open serial monitor
pio device monitor

# Clean build files
pio run --target clean

# Update libraries
pio pkg update

# List connected devices
pio device list
```

## 🔧 Configuration

### Configure Your WiFi Settings
Edit `src/WifiConfig.h` and set your WiFi credentials:

```cpp
#define WIFI_SSID "YourWiFiSSID"
#define WIFI_PASSWORD "YourWiFiPassword"
#define KLIPPER_IP "192.168.1.100"  // Your Klipper IP
#define KLIPPER_PORT 7125
```

### Display Pin Configuration
Pins are configured in `platformio.ini` build flags:
- MOSI: GPIO 7
- SCLK: GPIO 6
- CS: GPIO 10
- DC: GPIO 2
- RST: GPIO 3
- BL: GPIO 11

**If your display has different pins**, edit the build flags in `platformio.ini`.

### Partition Scheme
Custom partition scheme is already configured:
- App: 3.3MB
- SPIFFS: 1MB
- No OTA (to maximize app space)

## 🐛 Troubleshooting

### "Upload failed"
1. Hold the BOOT button on the display
2. Click Upload
3. Release BOOT when upload starts

### "No device found"
```bash
# List available ports
pio device list

# Manually specify port in platformio.ini
upload_port = /dev/cu.usbserial-XXXXX
```

### "Library not found"
```bash
# Install dependencies
pio pkg install
```

### "Compilation error"
```bash
# Clean and rebuild
pio run --target clean
pio run
```

## 📊 Build Information

After building, you'll see output like:
```
RAM:   [====      ]  60.2% (used 197KB from 327KB)
Flash: [=========]  89.5% (used 2.99MB from 3.34MB)
```

This shows memory usage. The firmware is optimized for ESP32-C3's limited resources.

## 🎯 What Happens After Upload

1. Display shows boot screen (2 seconds)
2. Connects to your configured WiFi network
3. Connects to Klipper at your configured IP
4. Shows rolling eyes animation (idle)
5. Updates when printing starts

## 🔍 Monitoring

To see debug output:
```bash
pio device monitor --baud 115200
```

You should see:
```
Knomi Clone - ESP32-C3
Initializing display...
Connecting to WiFi...
WiFi connected!
IP address: 192.168.X.XXX
Connected to Klipper!
Printer state: idle
```

## ⚙️ Advanced Options

### Change Upload Speed
Edit `platformio.ini`:
```ini
upload_speed = 460800  ; Slower, more reliable
; or
upload_speed = 921600  ; Faster (default)
```

### Enable More Debug Output
Edit `src/WifiConfig.h`:
```cpp
#define DEBUG_SERIAL true
#define DEBUG_API true  ; Shows all API calls
```

### Change Update Frequency
Edit `src/WifiConfig.h`:
```cpp
#define STATUS_UPDATE_INTERVAL 500   ; Update every 0.5s (faster)
#define TEMP_UPDATE_INTERVAL 1000    ; Update temps every 1s
```

## 🎉 You're Ready!

Your project is fully configured for PlatformIO. Just:
1. Open the `firmware/` folder in VS Code
2. Click the Upload button
3. Watch your Knomi Clone come to life!

## 📚 More Help

- PlatformIO Docs: https://docs.platformio.org/
- ESP32-C3 Guide: https://docs.platformio.org/en/latest/boards/espressif32/esp32-c3-devkitm-1.html
- Project Documentation: See `docs/` folder
