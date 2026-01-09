# 🔧 Troubleshooting Guide

## Common Issues & Solutions

### 🖥️ Display Issues

#### Black Screen / No Display Output

**Symptoms:**
- Display stays black after upload
- Backlight may be on but no graphics
- Serial monitor shows successful boot

**Root Cause:**
This project **requires LovyanGFX**. TFT_eSPI is incompatible with the ESP32-2424S012C display.

**Solution:**
1. Verify `platformio.ini` contains:
   ```ini
   lib_deps = 
       lovyan03/LovyanGFX@^1.1.16
   ```

2. Check `DisplayDriver.h` uses LovyanGFX:
   ```cpp
   #define LGFX_USE_V1
   #include <LovyanGFX.hpp>
   ```

3. Verify correct pins in `DisplayDriver.h`:
   ```cpp
   cfg.pin_rst = -1;      // NOT USED!
   cfg.freq_write = 80000000;  // 80MHz, not 40MHz
   ```

4. Backlight must be on GPIO3:
   ```cpp
   pinMode(3, OUTPUT);
   digitalWrite(3, HIGH);
   ```

---

#### Boot Loop / Store Access Fault

**Symptoms:**
```
Guru Meditation Error: Core 0 panic'ed (Store Access Fault)
MCAUSE: 0x00000007
```

**Causes:**
1. Wrong display library (TFT_eSPI instead of LovyanGFX)
2. Incorrect SPI frequency
3. Missing ESP32-C3 build flags

**Solution:**
1. Switch to LovyanGFX (see above)

2. Add critical build flags to `platformio.ini`:
   ```ini
   build_flags = 
       -DCORE_DEBUG_LEVEL=5
       -DBOARD_HAS_PSRAM
       -DARDUINO_USB_MODE=1
       -DARDUINO_USB_CDC_ON_BOOT=1
   ```

3. Clean build cache:
   ```bash
   rm -rf .pio
   pio run --target upload
   ```

---

#### Display Stuck on "Complete" Screen

**Symptoms:**
- Display shows "Print Complete" and doesn't return to idle
- Klipper reports PAUSED state with 100% progress

**Solution:**
This is fixed in `main.cpp`:
```cpp
// Treat PAUSED with 100% progress as IDLE
if (status.state == STATE_PAUSED && status.printProgress >= 100) {
  status.state = STATE_IDLE;
  lastValidProgress = 0;
}
```

If you don't have this code, update from the latest version.

---

### 📡 WiFi Issues

#### WiFi Won't Connect

**Symptoms:**
- Display shows "WiFi Error"
- Serial monitor shows AUTH_FAIL

**Solution:**
1. Check credentials in `WifiConfig.h`:
   ```cpp
   #define WIFI_SSID "your-network"
   #define WIFI_PASSWORD "your-password"
   ```

2. Verify 2.4GHz network (ESP32-C3 doesn't support 5GHz)

3. Check WiFi signal strength - move closer to router

---

#### Klipper Connection Failed

**Symptoms:**
- WiFi connected but no printer data
- "Connecting to Klipper..." message

**Solution:**
1. Verify Klipper IP in `WifiConfig.h`:
   ```cpp
   #define KLIPPER_HOST "192.168.1.100"
   #define KLIPPER_PORT 7125
   ```

2. Test Moonraker API manually:
   ```bash
   curl http://192.168.1.100:7125/printer/info
   ```

3. Check Moonraker CORS settings in `moonraker.conf`:
   ```ini
   [authorization]
   cors_domains:
       *
   ```

---

### 🔨 Build Issues

#### PlatformIO Build Fails

**Symptoms:**
```
FileNotFoundError: .sconsign39.dblite: No such file or directory
```

**Solution:**
Clean build cache and rebuild:
```bash
cd firmware
rm -rf .pio
pio run --target upload
```

---

#### Library Version Conflicts

**Symptoms:**
- LVGL installation fails
- Incompatible library versions

**Solution:**
Use exact versions from `platformio.ini`:
```ini
lib_deps = 
    lovyan03/LovyanGFX@^1.1.16
    lvgl/lvgl@8.3.11  # NOT 8.4.0!
```

---

### 🎨 Theme & Animation Issues

#### Themes Don't Switch

**Symptoms:**
- Tapping display doesn't change theme
- Stuck on one theme

**Solution:**
1. Verify touch is working (check serial monitor for touch events)

2. Check theme button pin in `main.cpp`:
   ```cpp
   #define THEME_BUTTON_PIN 9
   pinMode(THEME_BUTTON_PIN, INPUT_PULLUP);
   ```

3. Try manual theme switch via serial commands (if implemented)

---

#### Animations Laggy / Slow

**Symptoms:**
- Eye animations stutter
- Progress rings update slowly

**Solution:**
1. Verify SPI frequency is 80MHz:
   ```cpp
   cfg.freq_write = 80000000;
   ```

2. Check CPU isn't overloaded:
   - Reduce API polling frequency in `WifiConfig.h`
   - Disable debug logging

---

### 🔌 Hardware Issues

#### Touch Not Working

**Symptoms:**
- Can't switch themes by tapping
- No touch response

**Solution:**
1. Check I2C pins in `TouchDriver.cpp`:
   ```cpp
   Wire.begin(4, 5);  // SDA=4, SCL=5
   ```

2. Scan I2C bus for touch controller:
   ```cpp
   // Should find CST816D at 0x15
   ```

3. Some boards use FT6236 at 0x38 - firmware auto-detects

---

#### Backlight Not Working

**Symptoms:**
- Display is very dim or black
- Graphics work but hard to see

**Solution:**
Backlight MUST be on GPIO3:
```cpp
// In DisplayDriver.cpp init()
ledcSetup(0, 5000, 8);
ledcAttachPin(3, 0);  // GPIO3, not GPIO8!
ledcWrite(0, 200);    // Brightness 0-255
```

---

## 🆘 Emergency Recovery

### Complete Reset Procedure

If nothing works, follow these steps:

1. **Clean everything:**
   ```bash
   cd firmware
   rm -rf .pio
   rm -rf .vscode
   ```

2. **Verify platformio.ini:**
   - Check it matches the repository version
   - Ensure LovyanGFX is listed
   - Verify all build flags are present

3. **Flash factory firmware:**
   - Use factory examples from `/factory` folder
   - Confirm hardware works with factory code
   - Then flash Knomi firmware

4. **Check serial output:**
   ```bash
   pio device monitor --baud 115200
   ```
   - Look for initialization messages
   - Check for error codes
   - Verify WiFi connection

5. **Still not working?**
   - Check GitHub issues
   - Provide serial monitor output
   - Include platformio.ini and hardware details

---

## 📊 Diagnostic Checklist

Before asking for help, verify:

- [ ] Using LovyanGFX (not TFT_eSPI)
- [ ] Backlight on GPIO3
- [ ] RST pin set to -1
- [ ] SPI frequency 80MHz
- [ ] All ESP32-C3 build flags present
- [ ] Clean build (deleted .pio folder)
- [ ] Correct WiFi credentials
- [ ] Klipper IP is reachable
- [ ] Serial monitor shows no errors
- [ ] Factory firmware works on same hardware

---

## 🔍 Debug Mode

Enable verbose logging in `platformio.ini`:
```ini
build_flags = 
    -DCORE_DEBUG_LEVEL=5  # Maximum debug output
```

Monitor serial output:
```bash
pio device monitor --baud 115200 --filter direct
```

Look for:
- Display initialization messages
- WiFi connection status
- Klipper API responses
- Touch events
- Theme changes

---

## 📚 Additional Resources

- **Hardware Specs:** See `HARDWARE.md`
- **Klipper Setup:** See `KLIPPER_SETUP.md`
- **Features:** See `FEATURES.md`
- **Factory Examples:** Check `/factory` folder for working reference code

---

## 💡 Pro Tips

1. **Always clean build after major changes:**
   ```bash
   rm -rf .pio && pio run --target upload
   ```

2. **Test with factory firmware first** to confirm hardware works

3. **Use serial monitor** to see what's actually happening

4. **Check factory examples** - they use the correct LovyanGFX configuration

5. **Don't mix TFT_eSPI and LovyanGFX** - they're incompatible!

---

**Last Updated:** January 2026
**Firmware Version:** v2.0 (LovyanGFX)
