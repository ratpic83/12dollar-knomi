# Knomi Clone Firmware

## 🚀 Quick Start with PlatformIO

### 1. Open in VS Code
Open this **firmware** folder in VS Code:
```
File → Open Folder → Select this folder
```

### 2. PlatformIO will initialize automatically
The PlatformIO extension will detect `platformio.ini` and set up the project.

### 3. Build & Upload
Use the PlatformIO toolbar at the bottom of VS Code:
- **✓** Build
- **→** Upload
- **🔌** Serial Monitor

## ⚙️ Configuration Required

Before building, configure your WiFi settings in `src/WifiConfig.h`:
```cpp
#define WIFI_SSID "YourWiFiSSID"
#define WIFI_PASSWORD "YourWiFiPassword"
#define KLIPPER_IP "192.168.1.100"  // Your Klipper IP
```

## 📁 Project Structure

```
firmware/
├── platformio.ini      # PlatformIO configuration
├── src/               # Source code
│   ├── main.cpp       # Main application
│   ├── WifiConfig.h   # WiFi & Klipper settings
│   ├── DisplayDriver.h/cpp
│   ├── KlipperAPI.h/cpp
│   └── UIManager.h/cpp
├── lib/               # Custom libraries
└── partitions/        # ESP32-C3 partition scheme
```

## 🔧 Common Tasks

### Build Firmware
Click the **✓ Build** button in PlatformIO toolbar

### Upload to ESP32-C3
1. Connect display via USB-C
2. Click the **→ Upload** button
3. If it fails, hold BOOT button and try again

### View Serial Output
Click the **🔌 Serial Monitor** button (115200 baud)

### Clean Build
PlatformIO → Clean

## 📝 Notes

- All TFT_eSPI configuration is in `platformio.ini` as build flags
- Custom partition scheme allows 3.3MB for the app
- Libraries are automatically downloaded on first build

## 🐛 Troubleshooting

**Can't find PlatformIO toolbar?**
- Make sure PlatformIO IDE extension is installed
- Restart VS Code
- Look at the bottom of the window for the toolbar

**Upload fails?**
- Hold BOOT button on display during upload
- Try a different USB cable
- Check that the port is detected: PlatformIO → Devices

**Build errors?**
- Let PlatformIO download libraries first (takes a minute)
- Clean and rebuild: PlatformIO → Clean → Build

## 📚 More Information

See `../PLATFORMIO_GUIDE.md` for detailed instructions.
