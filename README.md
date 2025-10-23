# Knomi Clone - ESP32-C3 Round Display

A Klipper status display for ESP32-C3 based round displays, inspired by BTT Knomi.

## Hardware

This firmware is designed for the ESP32-C3 round display available here:
https://de.aliexpress.com/item/1005005453515690.html

**Specifications:**
- ESP32-C3 (single core, 400KB RAM)
- 1.28" Round LCD Display (240x240)
- GC9A01 Display Driver
- WiFi connectivity

## Features

- **High-Definition Graphics**: Anti-aliased progress rings, smooth animations, and professional visual effects
- **Theme System**: 4 beautiful color schemes (Dark, Light, Neon, Minimal) with live switching
- **Environmental Monitoring**: Optional BME280/DHT/SHT30 sensors for chamber temperature, humidity, and pressure
- **Web Configuration Portal**: Easy setup via captive portal - no code editing required
- **WiFi Connectivity**: Direct communication with Klipper via Moonraker API
- **Real-time Monitoring**: Live temperature, progress, and printer status updates
- **Rolling Eyes Animation**: Fun idle state animation with smooth easing
- **Temperature Gauges**: Visual hotend and bed temperature displays during printing
- **OTA Ready**: Framework for over-the-air updates (future enhancement)

## Project Structure

```
knomi_clone/
├── firmware/              # PlatformIO firmware for ESP32-C3
│   ├── src/              # Source code (main.cpp, drivers, UI)
│   ├── include/          # Header files
│   ├── lib/              # Custom libraries
│   ├── platformio.ini    # PlatformIO configuration
│   └── partitions/       # Custom partition scheme
├── klipper_config/       # Klipper configuration files
├── docs/                 # Comprehensive documentation
└── README.md
```

## Installation

### Prerequisites

1. **Visual Studio Code** with PlatformIO extension
2. **USB-C cable** for programming
3. **3D printer** running Klipper with Moonraker

### Quick Start (PlatformIO - Recommended)

1. **Install VS Code and PlatformIO**
   - Download VS Code: https://code.visualstudio.com/
   - Install PlatformIO IDE extension from marketplace

2. **Open Project**
   - Open the `firmware/` folder in VS Code
   - PlatformIO will auto-detect `platformio.ini`

3. **Configure WiFi and Klipper IP**
   - Edit `firmware/src/WifiConfig.h`
   - Set your WiFi SSID, password, and Klipper IP address

4. **Build and Upload**
   - Click **Build** (✓) in PlatformIO toolbar
   - Connect ESP32-C3 via USB-C
   - Click **Upload** (→) in PlatformIO toolbar

5. **Configure Klipper** (Optional)
   - Copy `klipper_config/knomi.cfg` to your Klipper config directory
   - Add `[include knomi.cfg]` to your printer.cfg
   - Restart Klipper

### Alternative: Arduino IDE

For Arduino IDE instructions, see [docs/BUILDING.md](docs/BUILDING.md)

## Configuration

### WiFi Settings

Edit `firmware/src/WifiConfig.h`:

```cpp
#define WIFI_SSID "YourWiFiSSID"
#define WIFI_PASSWORD "YourWiFiPassword"
#define KLIPPER_IP "192.168.1.100"  // Your Klipper IP
#define KLIPPER_PORT 7125
```

### Display Settings

Display configuration is in `platformio.ini` as build flags. Pin assignments and driver settings are pre-configured for the standard ESP32-C3 round display.

### Theme Selection

Themes are switched via GPIO 9 button (connect a button to cycle through themes):
- **Dark**: Classic black background with cyan accents
- **Light**: Clean white background with blue accents  
- **Neon**: Cyberpunk style with magenta/cyan on black
- **Minimal**: Subtle gray tones for understated look

### Environmental Sensors (Optional)

Connect a BME280, DHT11/22, or SHT30 sensor for chamber monitoring:

```cpp
// In main.cpp - automatically detects and initializes
envSensor.begin(SENSOR_BME280, 0); // I2C address 0x76
```

Displays chamber temperature, humidity, and pressure on the idle screen.

## Klipper Integration

The display communicates with Klipper via the Moonraker API over HTTP. It polls for:
- Printer state (idle, printing, paused, error)
- Temperature data (hotend, bed, chamber)
- Print progress
- File information

## Troubleshooting

### Display not connecting
- Check WiFi credentials
- Verify Klipper IP address
- Ensure Moonraker is running on port 7125

### Display shows error
- Check serial monitor for debug messages
- Verify ESP32-C3 has sufficient power (5V/1A minimum)

### Compilation errors
- Ensure all libraries are installed
- Check partition scheme is selected correctly
- Verify ESP32 board package version

## Development

### Building from Source

See [docs/BUILDING.md](docs/BUILDING.md) for detailed build instructions.

### Custom Screens

You can add custom display screens by modifying the UI classes in `firmware/src/UIManager.cpp`

## Credits

Based on the BTT Knomi concept and adapted for ESP32-C3 hardware.

Original Knomi documentation: https://bigtreetech.github.io/docs/KNOMI.html

## License

MIT License - See LICENSE file for details
