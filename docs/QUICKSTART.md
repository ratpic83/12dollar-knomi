# Quick Start Guide

Get your Knomi Clone up and running in 15 minutes!

## What You Need

- ✅ ESP32-C3 round display (1.28" GC9A01)
- ✅ USB-C cable
- ✅ Computer with VS Code (recommended) or Arduino IDE
- ✅ 3D printer running Klipper
- ✅ WiFi network (2.4GHz)

## Method 1: PlatformIO (Recommended)

### Step 1: Install VS Code and PlatformIO (5 minutes)

1. **Download and Install VS Code**:
   - Visit https://code.visualstudio.com/
   - Download for your OS (Windows/Mac/Linux)
   - Install and launch VS Code

2. **Install PlatformIO Extension**:
   - Open VS Code
   - Click Extensions icon (left sidebar) or press `Ctrl+Shift+X`
   - Search for "PlatformIO IDE"
   - Click Install
   - Wait for installation to complete (may take a few minutes)
   - Restart VS Code if prompted

### Step 2: Open Project (1 minute)

1. **Download or Clone the Project**:
   - Download ZIP from GitHub or clone the repository
   - Extract if needed

2. **Open in VS Code**:
   - In VS Code: `File` → `Open Folder`
   - Navigate to and select the `firmware/` folder
   - PlatformIO will automatically detect `platformio.ini`
   - Libraries will be downloaded automatically on first build

### Step 3: Configure WiFi (1 minute)

1. In VS Code, open `src/WifiConfig.h`

2. Update these settings:
   ```cpp
   #define WIFI_SSID "YourWiFiName"        // Your WiFi network name
   #define WIFI_PASSWORD "YourPassword"    // Your WiFi password
   #define KLIPPER_IP "192.168.1.100"      // Your Klipper IP address
   ```

3. Save the file (`Ctrl+S` or `Cmd+S`)

### Step 4: Build and Upload (3 minutes)

1. **Connect ESP32-C3 display** via USB-C cable

2. **Build the firmware**:
   - Click the checkmark (✓) icon in the bottom toolbar
   - Or press `Ctrl+Alt+B`
   - Wait for build to complete (~1-2 minutes first time)

3. **Upload to device**:
   - Click the arrow (→) icon in the bottom toolbar
   - Or press `Ctrl+Alt+U`
   - Wait for upload to complete

4. **If upload fails**:
   - Hold the BOOT button on the display
   - Click Upload again
   - Release BOOT when upload starts

**How to find your Klipper IP:**
- Check your router's device list
- Or in Mainsail/Fluidd, look at the browser URL
- Or SSH to your printer and run: `hostname -I`

### Step 5: Test Display

1. Display should show:
   - Boot screen with "KNOMI Clone"
   - "Connecting to WiFi..."
   - "Connected!" with checkmark
   - Rolling eyes animation (if connected to Klipper)

2. Open Serial Monitor (plug icon in bottom toolbar)

3. You should see:
   ```
   Knomi Clone - ESP32-C3
   Initializing display...
   Connecting to WiFi...
   WiFi connected!
   IP address: 192.168.1.XXX
   Connected to Klipper!
   ```

**If you see errors:**
- "WiFi connection failed" → Check SSID/password
- "Lost connection to Klipper" → Check IP address
- Black screen → Check display connections

### Step 6: Configure Klipper (Optional, 2 minutes)

For **enhanced features** (homing/leveling status indicators - future use):

1. SSH to your Klipper host:
   ```bash
   ssh pi@[YOUR_KLIPPER_IP]
   ```

2. Create config file:
   ```bash
   cd ~/printer_data/config
   nano knomi.cfg
   ```

3. Paste contents from `klipper_config/knomi.cfg`

4. Save (Ctrl+X, Y, Enter)

5. Edit `printer.cfg`:
   ```bash
   nano printer.cfg
   ```

6. Add near the top:
   ```ini
   [include knomi.cfg]
   ```

7. Save and restart Klipper:
   ```bash
   sudo systemctl restart klipper
   ```

**Note**: This step is optional. The display works perfectly without it, but enables future features like homing/leveling status display.

---

## Method 2: Arduino IDE (Alternative)

If you prefer Arduino IDE, follow these steps:

### Step 1: Install Arduino IDE (5 minutes)

1. Download Arduino IDE 1.8.19:
   - Windows: https://downloads.arduino.cc/arduino-1.8.19-windows.zip
   - Mac: https://downloads.arduino.cc/arduino-1.8.19-macosx.zip
   - Linux: https://downloads.arduino.cc/arduino-1.8.19-linux64.tar.xz

2. Extract and run Arduino IDE

3. Add ESP32 board support:
   - Open `File` → `Preferences`
   - Add to "Additional Board Manager URLs":
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Click OK
   - Open `Tools` → `Board` → `Boards Manager`
   - Search "ESP32" and install version 2.0.11

### Step 2-7: Follow detailed Arduino IDE instructions

See [docs/BUILDING.md](BUILDING.md) for complete Arduino IDE setup instructions.

---

## You're Done! 🎉

Your Knomi Clone is now running!

### What You Should See

**When Idle:**
- Rolling eyes animation
- "Ready" status
- Current temperatures
- **Environmental data** (if sensors connected)
- **Theme selection** via GPIO 9 button OR **touch tap**
- **Touch feedback** rings when interacting

**When Printing:**
- HD progress ring with anti-aliasing
- Print percentage with smooth updates
- Temperatures (hotend/bed) in corner gauges
- Time remaining and Z height
- Filename display

**When Paused:**
- "PAUSED" text
- Current progress
- Temperatures maintained

**When Complete:**
- Checkmark icon
- "COMPLETE" text
- Total print time

## Next Steps

### Mounting
- Design or download a 3D printed mount
- Secure to printer frame or toolhead
- Ensure good viewing angle

### Customization
- Adjust brightness in `WifiConfig.h`
- Change update intervals
- Modify colors in `DisplayDriver.h`
- **Add environmental sensors** (BME280/DHT)
- **Connect theme button** to GPIO 9

### Optional Enhancements

#### 🌈 Theme Switching
Connect a button to GPIO 9 for instant theme switching:
- **Dark**: Classic black theme (default)
- **Light**: Clean white theme
- **Neon**: Cyberpunk magenta/cyan
- **Minimal**: Subtle gray tones

#### 🌡️ Environmental Monitoring
Add chamber monitoring with affordable sensors:
- **BME280**: Temp, humidity, pressure (~$5)
- **DHT11/22**: Basic temp & humidity (~$3)
- **SHT30**: High-precision sensors (~$8)

Connect via I2C or digital pins - auto-detected on boot!

#### 🔧 Web Configuration
New devices start in configuration mode:
1. WiFi network "Knomi-Config" appears
2. Connect and visit `http://192.168.4.1`
3. Configure WiFi and Klipper settings
4. No code editing required!

### Troubleshooting
- Check `docs/BUILDING.md` for detailed help
- See `docs/HARDWARE.md` for wiring issues
- Read `docs/KLIPPER_SETUP.md` for Klipper problems

## Common Issues

### Display shows "WiFi Error"
**Solution**: 
- Verify SSID and password in `WifiConfig.h`
- Ensure 2.4GHz WiFi (ESP32 doesn't support 5GHz)
- Check WiFi signal strength

### Display shows "Klipper Offline"
**Solution**:
- Verify Klipper IP address
- Test: `curl http://[KLIPPER_IP]:7125/server/info`
- Ensure Moonraker is running
- Check firewall settings

### Display is blank/black
**Solution**:
- Check USB power connection
- Verify pin configuration in `User_Setup.h`
- Try different USB cable/port
- Check backlight connection

### Temperatures not updating
**Solution**:
- Wait 10-15 seconds after boot
- Check serial monitor for errors
- Verify Moonraker API is accessible
- Ensure heaters are configured in Klipper

### Upload fails
**Solution**:
- Hold BOOT button during upload
- Try different USB cable
- Install CH340 drivers (Windows)
- Select correct COM port

## Getting Help

- 📖 Read the full documentation in `docs/`
- 🐛 Report bugs on GitHub Issues
- 💬 Ask questions in Discussions
- 📧 Check the FAQ

## Enjoy Your Knomi Clone!

You now have a working Klipper status display for a fraction of the cost of the original Knomi!

**Share your build:**
- Post photos of your setup
- Share your custom mounts
- Contribute improvements
- Help others in the community

Happy printing! 🖨️
