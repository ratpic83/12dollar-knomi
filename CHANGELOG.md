# Changelog

All notable changes to this project will be documented in this file.

## [2.0.0] - 2026-01-09

### 🔥 BREAKING CHANGES

**Display Library Changed: TFT_eSPI → LovyanGFX**

The ESP32-2424S012C display is **incompatible with TFT_eSPI**. This version switches to LovyanGFX for proper hardware support.

**Migration Required:**
- Clean build: `rm -rf .pio && pio run --target upload`
- No code changes needed for end users
- See [TROUBLESHOOTING.md](TROUBLESHOOTING.md) for details

### ✨ Added

- **LovyanGFX Support** - Proper display library for ESP32-2424S012C
- **NEON Glow Effects** - Enhanced visual effects in NEON theme
  - Glowing eye animations with outer rings
  - Enhanced progress rings with glow effects
  - Multiple highlight layers for depth
- **Improved Eye Animations** - Higher resolution, smoother movements
- **Enhanced Progress Rings** - Multi-layered with theme-based colors
- **Comprehensive Documentation**
  - New `TROUBLESHOOTING.md` with complete issue resolution guide
  - Updated `HARDWARE.md` with critical LovyanGFX configuration
  - Enhanced `README.md` with display library warnings
  - Detailed comments in `platformio.ini`

### 🔧 Fixed

- **Black Screen Issue** - Switched from TFT_eSPI to LovyanGFX
- **Boot Loop / Store Access Fault** - Correct library and pin configuration
- **Stuck on Complete Screen** - PAUSED with 100% progress now treated as IDLE
- **Backlight Pin** - Corrected to GPIO3 (was incorrectly GPIO8)
- **SPI Frequency** - Corrected to 80MHz (was incorrectly 40MHz)
- **Reset Pin** - Properly disabled (RST=-1, not GPIO3)

### 🎨 Changed

- **Display Driver** - Complete rewrite using LovyanGFX
- **Eye Animation** - Enhanced with theme-based colors and glow effects
- **Progress Rings** - Improved visual quality with NEON glow support
- **Theme System** - Better integration with visual effects

### 📚 Documentation

- Added `TROUBLESHOOTING.md` - Complete troubleshooting guide
- Updated `HARDWARE.md` - Critical LovyanGFX information
- Updated `README.md` - Display library warnings and links
- Enhanced `platformio.ini` - Comprehensive comments explaining settings

### ⚙️ Technical Details

**Display Configuration:**
```cpp
Library: LovyanGFX v1.2.7
Driver: GC9A01
Resolution: 240x240
SPI Frequency: 80MHz (write), 20MHz (read)
Backlight: GPIO3 (PWM controlled)
Reset: -1 (not used)
```

**Critical Pins:**
- MOSI: GPIO7
- SCLK: GPIO6
- CS: GPIO10
- DC: GPIO2
- RST: -1 (disabled)
- BL: GPIO3 (not GPIO8!)

---

## [1.1.0] - Previous Version

### Features
- TFT_eSPI based display driver (incompatible with hardware)
- Basic theme system
- Klipper integration
- Touch gestures

### Known Issues
- Black screen on ESP32-2424S012C
- Boot loops with Store Access Fault
- Display stuck on complete screen

---

## Migration Guide: v1.x → v2.0

### For Users

1. **Clean Build Required:**
   ```bash
   cd firmware
   rm -rf .pio
   pio run --target upload
   ```

2. **No Configuration Changes Needed**
   - WiFi settings remain the same
   - Klipper configuration unchanged
   - Themes and features work as before

### For Developers

1. **Display Code Changes:**
   - Replace `TFT_eSPI` includes with `LovyanGFX.hpp`
   - Update display initialization (see `DisplayDriver.h`)
   - Backlight control moved to GPIO3

2. **Build Configuration:**
   - Verify `platformio.ini` has LovyanGFX dependency
   - Ensure ESP32-C3 build flags are present
   - Check pin definitions match hardware

3. **Testing:**
   - Test with factory firmware first
   - Verify serial monitor output
   - Check all themes and animations

---

## Known Issues

### Current Version (v2.0.0)

None reported.

### Reporting Issues

Please check [TROUBLESHOOTING.md](TROUBLESHOOTING.md) first, then:

1. Verify you're using LovyanGFX (not TFT_eSPI)
2. Clean build: `rm -rf .pio && pio run`
3. Check serial monitor output
4. Provide hardware details and error messages

---

**Full Changelog:** https://github.com/yourusername/knomi_clone/compare/v1.1.0...v2.0.0
