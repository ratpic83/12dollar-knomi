# 🖥️ PaWe i-print v1.1

**A Feature-Rich Klipper Display for ESP32-C3 Round Screens**

Professional 3D printer monitoring with touch gestures, animated graphics, and real-time status updates.

## 🔧 Hardware

**Board:** ESP32-2424S012C (ESP32-C3)
- **Purchase:** [AliExpress](https://de.aliexpress.com/item/1005005453515690.html) (~$12)
- **MCU:** ESP32-C3 (RISC-V, 160MHz, 400KB RAM)
- **Display:** 1.28" Round IPS LCD (240x240, GC9A01 driver)
- **Touch:** CST816D Capacitive Touch Controller
- **Connectivity:** WiFi 802.11 b/g/n
- **Power:** USB-C (5V/1A)
- **Dimensions:** 35mm diameter

## ✨ Features

### 🎮 **Touch Gestures**
- **Tap** - Cycle through color themes
- **Swipe Up/Down** - Adjust screen brightness (20-255)
- **Swipe Left/Right** - Navigate screens (manual mode)
- **Draw Circle** - Easter egg animation 🌈

### 📊 **Display Modes**
- **Idle Screen** - Rolling eyes animation with temps
- **Printing Screen** - Progress ring, temps, time remaining
- **Paused Screen** - Dimmed display with pause indicator
- **Complete Screen** - Success animation
- **Error Screen** - Visual error alerts

### 🎨 **Visual Features**
- **4 Color Themes** - Dark, Light, Neon, Minimal
- **Smooth Animations** - 60 FPS eye movements, no flicker
- **PWM Brightness** - Adjustable backlight (GPIO3)
- **Custom Boot Logo** - Electric Callboy rainbow theme
- **Anti-aliased Graphics** - Professional UI elements

### 🌐 **Connectivity**
- **Moonraker API** - Real-time printer data
- **WiFi** - 2.4GHz 802.11 b/g/n
- **No Klipper Config Required** - Works out of the box
- **Webcam Support** - Fetch and display camera snapshots
- **Print Thumbnails** - Show gcode preview images

### 🔧 **Advanced Features**
- **Manual/Auto Mode** - Lock screen or auto-switch
- **Image Converter Tools** - Convert any image to boot logo
- **GIF Animation Support** - Frame-by-frame playback
- **Memory Optimized** - ~500KB free for custom features

## 📁 Project Structure

```
pawe-i-print/
├── firmware/                      # ESP32-C3 Firmware
│   ├── src/
│   │   ├── main.cpp              # Main application
│   │   ├── DisplayDriver.*       # GC9A01 display driver
│   │   ├── TouchDriver.*         # CST816D touch + gestures
│   │   ├── UIManager.*           # Screen rendering
│   │   ├── KlipperAPI.*          # Moonraker communication
│   │   ├── ThemeManager.*        # Color themes
│   │   ├── AnimationPlayer.*     # GIF playback
│   │   ├── ImageFetcher.*        # Webcam/thumbnail fetcher
│   │   └── WifiConfig.h          # WiFi settings
│   └── platformio.ini            # Build configuration
├── tools/                         # Image conversion tools
│   ├── image_to_header.py        # Static image converter
│   ├── gif_to_animation.py       # GIF to C array converter
│   └── README.md                 # Tool documentation
├── klipper_config/                # Klipper integration
│   ├── knomi_minimal.cfg         # Safe minimal config
│   └── pawe_status_addon.cfg     # Status message examples
├── docs/                          # Documentation
│   ├── KLIPPER_SETUP.md          # Printer integration guide
│   ├── IMAGE_FEATURES.md         # Image/webcam features
│   └── ANIMATION_README.md       # Animation system
└── README.md                      # This file
```

## 🚀 Quick Start

### 1️⃣ **Install PlatformIO**

```bash
# Install VS Code
https://code.visualstudio.com/

# Install PlatformIO Extension
# In VS Code: Extensions → Search "PlatformIO" → Install
```

### 2️⃣ **Clone & Configure**

```bash
git clone https://github.com/ratpic83/12dollar-knomi.git
cd 12dollar-knomi/firmware
```

**Edit `src/WifiConfig.h`:**
```cpp
#define WIFI_SSID "YourWiFi"
#define WIFI_PASSWORD "YourPassword"
#define KLIPPER_IP "192.168.68.91"  // Your printer IP
#define KLIPPER_PORT 7125
```

### 3️⃣ **Build & Upload**

```bash
# In VS Code with PlatformIO:
# 1. Open firmware/ folder
# 2. Click Build (✓) in toolbar
# 3. Connect ESP32-C3 via USB-C
# 4. Click Upload (→) in toolbar
```

**Or via command line:**
```bash
cd firmware
pio run -t upload
```

### 4️⃣ **Klipper Setup** (Optional)

**Your display works WITHOUT any Klipper config!**

But if you had the old `knomi.cfg` causing boot issues:

```bash
# SSH to your printer
ssh pi@192.168.68.91

# Edit printer.cfg
nano ~/printer_data/config/printer.cfg

# Remove or comment out:
# [include knomi.cfg]  ← Delete this line

# Save and restart Klipper
sudo systemctl restart klipper
```

See [KLIPPER_SETUP.md](KLIPPER_SETUP.md) for details.

## 📚 Documentation

- **[FEATURES.md](FEATURES.md)** - Complete feature guide with all gestures
- **[HARDWARE.md](HARDWARE.md)** - Pinout, wiring diagrams, specifications
- **[KLIPPER_SETUP.md](KLIPPER_SETUP.md)** - Printer integration guide
- **[IMAGE_FEATURES.md](IMAGE_FEATURES.md)** - Webcam & thumbnail setup
- **[ANIMATION_README.md](ANIMATION_README.md)** - GIF animation system
- **[tools/README.md](tools/README.md)** - Image conversion tools

## 👁️ Usage

### Touch Gestures
| Gesture | Action |
|---------|--------|
| **Tap** | Cycle themes & exit manual mode |
| **Swipe Up** | Increase brightness (+25) |
| **Swipe Down** | Decrease brightness (-25) |
| **Swipe Left** | Next screen (enter manual mode) |
| **Swipe Right** | Previous screen (enter manual mode) |
| **Draw Circle** | Easter egg animation 🌈 |

### Display Modes
- **Idle** - Rolling eyes animation
- **Printing** - Progress ring with stats
- **Paused** - Dimmed pause indicator
- **Complete** - Success screen
- **Error** - Error alerts

### Themes
1. **Dark** - Black with cyan (default)
2. **Light** - White with blue
3. **Neon** - Cyberpunk magenta/cyan
4. **Minimal** - Subtle grays

See [FEATURES.md](FEATURES.md) for complete guide.

---

## 🔧 Troubleshooting

### Display Not Working
**Symptoms:** Blank screen, no backlight

**Solutions:**
1. Check USB-C power (5V/1A minimum)
2. Verify backlight PWM on GPIO3
3. Check TFT_eSPI configuration
4. Serial monitor: Look for "Display initialized"

### Touch Not Responding
**Symptoms:** No gesture detection

**Solutions:**
1. Check I2C address (0x15 for CST816D)
2. Verify SDA=4, SCL=5 wiring
3. Serial monitor: Look for "Touch initialized"
4. Try FT6236 address (0x38) if CST816D fails

### WiFi Connection Failed
**Symptoms:** "Connecting..." stuck on screen

**Solutions:**
1. Verify SSID/password in WifiConfig.h
2. Check 2.4GHz WiFi (ESP32-C3 doesn't support 5GHz)
3. Serial monitor: Check for WiFi error messages
4. Ensure router allows new devices

### Klipper Not Connecting
**Symptoms:** Display works but no printer data

**Solutions:**
1. Verify Klipper IP address
2. Check Moonraker running on port 7125
3. Test: `curl http://KLIPPER_IP:7125/printer/info`
4. Check firewall settings

### Printer Won't Boot (Klipper)
**Symptoms:** Klipper fails after adding config

**Solution:**
```bash
# Remove old knomi.cfg include
ssh pi@YOUR_PRINTER_IP
nano ~/printer_data/config/printer.cfg
# Delete: [include knomi.cfg]
sudo systemctl restart klipper
```

See [KLIPPER_SETUP.md](KLIPPER_SETUP.md) for details.

### Compilation Errors
**Solutions:**
1. Update PlatformIO: `pio upgrade`
2. Clean build: `pio run -t clean`
3. Check ESP32 platform version in platformio.ini
4. Verify all libraries installed

---

## 🛠️ Development

### Build from Source
```bash
git clone https://github.com/ratpic83/12dollar-knomi.git
cd 12dollar-knomi/firmware
pio run
```

### Add Custom Screens
Edit `firmware/src/UIManager.cpp`:
```cpp
void UIManager::drawCustomScreen(PrinterStatus& status) {
  display->clear();
  // Your custom graphics here
}
```

### Create Custom Themes
Edit `firmware/src/ThemeManager.cpp`:
```cpp
themes[4] = {
  .background = 0x0000,  // RGB565 color
  .text = 0xFFFF,
  // ...
};
```

### Add Custom Gestures
Edit `firmware/src/TouchDriver.cpp` in `detectGesture()`

---

## 📊 Performance

**Memory Usage:**
- Flash: 749KB / 1310KB (57.2%)
- RAM: 33KB / 327KB (10.1%)
- Free: ~500KB for custom features

**Frame Rate:**
- Animations: 60 FPS
- UI Updates: 1 Hz (Klipper polling)
- Touch Scan: 60 Hz

**Power:**
- Idle: ~80mA (0.4W)
- Active: ~120mA (0.6W)
- Max: ~150mA (0.75W)

## 🎉 Credits

**Created by:** Patrick Weidermann (PaWe)  
**Inspired by:** BTT Knomi  
**Hardware:** ESP32-2424S012C Round Display  
**Repository:** [github.com/ratpic83/12dollar-knomi](https://github.com/ratpic83/12dollar-knomi)  

**Special Thanks:**
- Klipper & Moonraker teams
- TFT_eSPI library by Bodmer
- ESP32 Arduino core team
- Electric Callboy for the inspiration 🌈

---

## 📝 License

MIT License - See LICENSE file for details

---

## 🔗 Links

- **GitHub:** https://github.com/ratpic83/12dollar-knomi
- **Hardware:** [ESP32-2424S012C on AliExpress](https://de.aliexpress.com/item/1005005453515690.html)
- **Klipper:** https://www.klipper3d.org/
- **Moonraker:** https://moonraker.readthedocs.io/
- **BTT Knomi:** https://bigtreetech.github.io/docs/KNOMI.html

---

## ⭐ Support

If you find this project useful:
- ⭐ Star the repository
- 🐛 Report issues
- 💡 Suggest features
- 🔧 Contribute code
- 📸 Share your build!

---

**Made with ❤️ for the 3D printing community**
