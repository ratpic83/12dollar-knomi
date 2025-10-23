# Frequently Asked Questions (FAQ)

## General Questions

### What is Knomi Clone?

Knomi Clone is an open-source firmware for ESP32-C3 based round displays that replicates the functionality of the BTT Knomi display for 3D printers running Klipper. It shows real-time printer status, temperatures, and print progress.

### How is this different from BTT Knomi?

| Feature | BTT Knomi | Knomi Clone |
|---------|-----------|-------------|
| Price | ~€40 | ~€12 (hardware only) |
| MCU | ESP32 (dual core) | ESP32-C3 (single core) |
| Firmware | Closed source | Open source |
| Mounting | Stealthburner specific | Universal |
| Customization | Limited | Fully customizable |

### Is this legal?

Yes! This is a clean-room implementation inspired by the Knomi concept but written from scratch. It's not a copy of BTT's firmware. The project is released under MIT license.

### Will this work with my printer?

If your printer runs Klipper with Moonraker, yes! It works with:
- Voron (all models)
- Ender 3 (with Klipper)
- Prusa (with Klipper)
- Any CoreXY, Cartesian, or Delta printer running Klipper

### Do I need the original BTT Knomi hardware?

No! This firmware is designed for cheap ESP32-C3 round displays available on AliExpress for ~€12. See the hardware guide for the specific model.

## Hardware Questions

### Where can I buy the display?

AliExpress: https://de.aliexpress.com/item/1005005453515690.html

Search for: "ESP32-C3 1.28 inch round LCD GC9A01"

### What are the minimum requirements?

- ESP32-C3 microcontroller
- 1.28" round LCD with GC9A01 driver
- 240x240 resolution
- WiFi capability
- USB-C for programming

### Can I use a different display?

Maybe. The firmware is written for GC9A01 driver. Other displays would need:
- Code modifications for different driver
- Pin configuration changes
- Possibly different libraries

### Does it work with ESP32 (not C3)?

The code should work with regular ESP32 with minor modifications:
- Change board selection in Arduino IDE
- Adjust partition scheme
- Update pin definitions
- More RAM available (easier)

### Do I need to solder anything?

No! The display comes fully assembled. Just connect via USB-C for programming and power.

### Can I power it from my printer?

Yes, you have options:
1. USB-C cable to printer's USB port (easiest)
2. 5V/GND wires from printer board
3. Connection to toolhead board (if using CAN)

See the hardware guide for details.

## Software Questions

### What software do I need?

- **VS Code with PlatformIO** (recommended) or Arduino IDE 1.8.19
- Libraries are automatically managed by PlatformIO
- For Arduino IDE: ESP32 board support, TFT_eSPI, ArduinoJson

### Can I use Arduino IDE 2.0?

The project is now primarily designed for PlatformIO. Arduino IDE 1.8.19 can still be used but requires manual file reorganization. Arduino IDE 2.0 is not officially supported.

### Can I use PlatformIO instead?

Yes! PlatformIO is now the **recommended** build method. It provides automatic dependency management, better IDE integration, and easier setup.

### Do I need to know programming?

No! Just follow the quick start guide. You only need to:
1. Install VS Code and PlatformIO
2. Edit WiFi credentials in `src/WifiConfig.h`
3. Click Build and Upload
4. Configure Klipper (optional)

### Can I customize the display?

Yes! The code is open source. You can:
- Change colors in `DisplayDriver.h`
- Modify layouts in `UIManager.cpp`
- Add new screens
- Adjust animations
- Create themes
- All source files are in `firmware/src/`

## Setup Questions

### How do I find my Klipper IP address?

Several ways:
1. Check your router's device list
2. Look at Mainsail/Fluidd URL in browser
3. SSH to printer: `hostname -I`
4. Check your printer's display (if it has one)

### What if I don't know my WiFi password?

- Check your router (often printed on label)
- Ask your network administrator
- Check saved passwords on your computer
- Use a WiFi password recovery tool

### Do I need to configure Klipper?

Basic functionality works without Klipper changes. But for enhanced features (homing/leveling status), you should add the included `knomi.cfg` file.

### Can I use it without WiFi?

No, WiFi is required to communicate with Klipper. The display needs network access to the Moonraker API.

### Does it work with Octoprint?

Not currently. It's designed for Klipper/Moonraker API. Octoprint support could be added but would require code changes.

## Troubleshooting Questions

### Why is my display blank?

Common causes:
1. No power - check USB connection
2. Wrong pin configuration
3. Backlight not enabled
4. Firmware upload failed

**Solution**: Check serial monitor for errors, verify connections.

### Why does it show "WiFi Error"?

Common causes:
1. Wrong SSID or password
2. 5GHz WiFi (ESP32 only supports 2.4GHz)
3. Weak signal
4. MAC filtering on router

**Solution**: Double-check credentials, ensure 2.4GHz network.

### Why does it show "Klipper Offline"?

Common causes:
1. Wrong IP address
2. Moonraker not running
3. Firewall blocking port 7125
4. Network connectivity issue

**Solution**: Test with `curl http://[IP]:7125/server/info`

### Temperatures show as 0°C

Common causes:
1. Heater names don't match your config
2. API query timeout
3. Moonraker not responding

**Solution**: Check serial monitor, verify heater names in code.

### Print progress stuck at 0%

Common causes:
1. `display_status` not configured in Klipper
2. Slicer doesn't include progress info
3. Not printing from virtual SD card

**Solution**: Add `[display_status]` to printer.cfg

### Upload fails in Arduino IDE

Common causes:
1. Wrong COM port selected
2. Driver not installed (CH340)
3. USB cable is charge-only
4. Need to hold BOOT button

**Solution**: Hold BOOT button during upload, try different cable.

### Display is very dim

Common causes:
1. Brightness set too low
2. Backlight not connected
3. Power supply insufficient

**Solution**: Increase brightness in `WifiConfig.h`, check power.

## Feature Questions

### Does it have touch screen support?

Not yet. Touch support is planned for v1.1.0. The hardware supports it, but firmware doesn't implement it yet.

### Can I control my printer from the display?

Not yet. Direct control (pause/resume/cancel) is planned for v2.0.0. Currently it's display-only.

### Can I see the webcam feed?

No, and unlikely to be added. The display is too small and low-resolution for useful webcam preview.

### Does it support multiple extruders?

Basic support is there (reads extruder temperature). Full multi-extruder UI is planned for future release.

### Can I customize the screens?

Yes! Edit the code in `UIManager.cpp`. You can:
- Change layouts
- Add new information
- Modify colors
- Create custom screens

### Does it work offline?

No, it requires network connection to Klipper. It doesn't have local storage of print files.

### Can I update firmware wirelessly?

Not yet. OTA (Over-The-Air) updates are planned for v1.2.0. Currently requires USB connection.

## Performance Questions

### How often does it update?

Default: Every 1 second

You can adjust in `WifiConfig.h`:
```cpp
#define STATUS_UPDATE_INTERVAL 1000  // milliseconds
```

### Does it slow down my printer?

No. It only reads data via API, doesn't interfere with printing. Network traffic is minimal.

### How much power does it use?

Approximately:
- Idle: ~100mA @ 5V (0.5W)
- Active: ~200mA @ 5V (1W)
- Peak: ~500mA @ 5V (2.5W)

### Will it work with slow WiFi?

Yes, but updates may be delayed. Minimum recommended: 1 Mbps. The display uses very little bandwidth (~1-2 KB/s).

### Does it store any data?

No. It's purely a display device. All data comes from Klipper in real-time.

## Development Questions

### Can I contribute to the project?

Yes! Contributions are welcome:
- Bug reports
- Feature requests
- Code improvements
- Documentation
- 3D printable mounts
- Testing on different hardware

### What programming language is it?

C++ with Arduino framework. Uses:
- Arduino core for ESP32
- TFT_eSPI for display
- ArduinoJson for API parsing
- Standard C++ libraries

### How can I add a new screen?

1. Create new screen type in `UIManager.h`
2. Add drawing function in `UIManager.cpp`
3. Add logic to switch to new screen
4. Update state machine

See the code comments for details.

### Can I use this commercially?

Yes! MIT license allows commercial use. You can:
- Sell pre-configured displays
- Include in printer kits
- Offer as a service

Just include the license and give credit.

### How do I debug issues?

1. Enable debug output in `WifiConfig.h`:
   ```cpp
   #define DEBUG_SERIAL true
   #define DEBUG_API true
   ```
2. Open Serial Monitor (115200 baud)
3. Watch for error messages
4. Check Klipper/Moonraker logs

## Compatibility Questions

### What Klipper version do I need?

Any recent version (2022+). The Moonraker API is stable.

### Does it work with Mainsail?

Yes! Mainsail uses Moonraker, which is what the display connects to.

### Does it work with Fluidd?

Yes! Fluidd also uses Moonraker.

### Does it work with MainsailOS?

Yes! MainsailOS includes Klipper and Moonraker.

### What about Raspberry Pi alternatives?

Works with any Klipper host:
- Raspberry Pi (all models)
- Orange Pi
- BTT CB1
- x86 Linux PC
- Any system running Klipper + Moonraker

## Future Questions

### When will touch support be added?

Planned for v1.1.0 (next major release). No specific date yet.

### Will there be a web configuration interface?

Yes, planned for v1.2.0. Will allow WiFi setup without editing code.

### Can it support multiple printers?

Planned for v2.0.0. Would allow switching between multiple Klipper instances.

### Will you support other display sizes?

Possibly. The code is modular enough to support different displays. Community contributions welcome!

### What about E-ink displays?

Interesting idea! E-ink would need:
- Different driver
- Slower refresh rate
- Different UI design (no animations)
- Lower power consumption

Could be a future variant.

## Still Have Questions?

- 📖 Check the documentation in `docs/`
- 🔍 Search existing GitHub issues
- 💬 Ask in GitHub Discussions
- 🐛 Report bugs in GitHub Issues

We're here to help! 🚀
