# Klipper Configuration Guide

This guide explains how to configure Klipper to work with the Knomi Clone display.

## Overview

The Knomi Clone communicates with Klipper via the Moonraker HTTP API. It polls for printer status, temperatures, and print progress at regular intervals.

**Basic functionality works without any Klipper configuration!** The optional Klipper config adds enhanced features like homing/leveling status tracking.

## Prerequisites

- Klipper installed and running
- Moonraker installed and configured
- Network connectivity between display and Klipper host
- Moonraker API accessible on port 7125

## Basic Setup (Required)

The display works out-of-the-box with standard Klipper installations. Just configure the IP address in `WifiConfig.h`.

## Enhanced Setup (Optional)

The optional Klipper configuration enables future features like homing/leveling status indicators.

### Installation Steps

#### 1. Verify Moonraker is Running

Test Moonraker API access:

```bash
curl http://[YOUR_KLIPPER_IP]:7125/server/info
```

You should see JSON response with server information.

#### 2. Copy Configuration File

Copy the Knomi configuration to your Klipper config directory:

```bash
# SSH into your Klipper host
ssh pi@[YOUR_KLIPPER_IP]

# Navigate to config directory
cd ~/printer_data/config

# Download or copy knomi.cfg
wget https://raw.githubusercontent.com/[your-repo]/knomi_clone/main/klipper_config/knomi.cfg

# Or manually create the file
nano knomi.cfg
# Paste contents from klipper_config/knomi.cfg
```

#### 3. Include in printer.cfg

Add this line to your `printer.cfg`:

```ini
[include knomi.cfg]
```

Place it near the top with other includes.

#### 4. Restart Klipper

```bash
# Via Mainsail/Fluidd web interface
# Or via command line:
sudo systemctl restart klipper
```

#### 5. Verify Configuration

Check that macros are loaded:

```gcode
# In Mainsail/Fluidd console, run:
KNOMI_STATUS MSG="Test Message"
```

## Features Enabled by Klipper Config

### Current Features
- **Print status messages**: Custom notifications during print jobs
- **Enhanced macro integration**: Print start/end notifications

### Future Features (Framework Ready)
- **Homing status**: Show "Homing..." when G28 is running
- **Leveling status**: Show "Leveling..." during bed mesh calibration
- **Custom notifications**: User-defined status messages
- **Error reporting**: Automatic error notifications

### Macro Variables

The configuration adds two macro variables for tracking printer state:

```ini
[gcode_macro BedLevelVar]
variable_leveling: False

[gcode_macro HomeSetVar]
variable_homing: False
```

These are used by the display to show homing/leveling status.

### Modified G-Codes

The configuration overrides standard G-codes to update variables:

- **G28** (Homing): Sets `homing` variable during operation
- **BED_MESH_CALIBRATE**: Sets `leveling` variable during bed leveling

### Status Macro

```ini
[gcode_macro KNOMI_STATUS]
gcode:
  {% set msg = params.MSG|default("Status Update") %}
  M117 {msg}
```

Use this to send custom messages to the display.

## Moonraker Configuration

### CORS Settings

Ensure Moonraker allows API access. Edit `moonraker.conf`:

```ini
[authorization]
cors_domains:
    *
    # Or specifically allow your display's IP:
    # http://192.168.1.XXX
```

### API Access

The display queries these endpoints:

- `/printer/objects/query` - Main status query
- `/server/info` - Connection test
- `/printer/print/pause` - Pause print (future)
- `/printer/print/resume` - Resume print (future)
- `/printer/print/cancel` - Cancel print (future)

## Queried Objects

The display requests these Klipper objects:

```
heater_bed          - Bed temperature
extruder            - Hotend temperature
print_stats         - Print state and progress
gcode_move          - Position and speeds
fan                 - Part cooling fan
display_status      - Print progress
HomeSetVar          - Homing status (custom)
BedLevelVar         - Leveling status (custom)
```

## Custom Macros Integration

### Print Start/End

If you use custom print start/end macros, integrate Knomi status:

```ini
[gcode_macro PRINT_START]
gcode:
  KNOMI_STATUS MSG="Heating..."
  M140 S{BED_TEMP}
  M104 S{EXTRUDER_TEMP}
  
  KNOMI_STATUS MSG="Homing..."
  G28
  
  KNOMI_STATUS MSG="Bed Leveling..."
  BED_MESH_CALIBRATE
  
  KNOMI_STATUS MSG="Printing..."
  # Your print start code here
```

### Filament Change

```ini
[gcode_macro M600]
gcode:
  KNOMI_STATUS MSG="Filament Change"
  PAUSE
  # Your filament change code here
```

### Error Handling

```ini
[gcode_macro HANDLE_ERROR]
gcode:
  KNOMI_STATUS MSG="ERROR!"
  # Your error handling code here
```

## Advanced Configuration

### Update Intervals

Adjust how often the display polls Klipper by editing `WifiConfig.h`:

```cpp
#define STATUS_UPDATE_INTERVAL 1000   // 1 second (default)
#define TEMP_UPDATE_INTERVAL 2000     // 2 seconds
```

Lower values = more responsive but more network traffic.

### Custom Objects

To add custom sensor data to the display:

1. Add sensor to Klipper config
2. Query it in `KlipperAPI.cpp`
3. Display it in `UIManager.cpp`

Example - Chamber temperature:

```cpp
// In KlipperAPI.cpp
if (result.containsKey("temperature_sensor chamber")) {
  status.chamberTemp = result["temperature_sensor chamber"]["temperature"] | 0.0;
}
```

## Troubleshooting

### Display Shows "Klipper Offline"

**Check**:
1. Moonraker is running: `systemctl status moonraker`
2. Port 7125 is accessible: `netstat -tulpn | grep 7125`
3. Firewall allows connections: `sudo ufw status`
4. IP address is correct in display config

**Solutions**:
```bash
# Restart Moonraker
sudo systemctl restart moonraker

# Check Moonraker logs
tail -f ~/printer_data/logs/moonraker.log

# Test API manually
curl http://localhost:7125/server/info
```

### Variables Not Updating

**Check**:
1. Macros are loaded: Check Mainsail/Fluidd interface
2. No syntax errors: Check Klipper logs
3. Variables are being set: Add debug output

**Solutions**:
```bash
# Check Klipper logs
tail -f ~/printer_data/logs/klippy.log

# Verify macro exists
# In Mainsail/Fluidd console:
SET_GCODE_VARIABLE MACRO=HomeSetVar VARIABLE=homing VALUE=True
```

### Temperatures Not Showing

**Check**:
1. Heater names match your config
2. Sensors are configured in Klipper
3. API query includes correct objects

**Solutions**:
- Edit `KlipperAPI.cpp` to match your heater names
- Common variations: `extruder`, `extruder0`, `heater_bed`, etc.

### Print Progress Stuck at 0%

**Check**:
1. `display_status` is configured in Klipper
2. Slicer includes progress comments (M73)
3. Virtual SD card is being used

**Solutions**:
```ini
# Add to printer.cfg if missing
[display_status]

[virtual_sdcard]
path: ~/printer_data/gcodes
```

## Network Optimization

### Reduce API Load

If you have multiple displays or high network traffic:

1. Increase update intervals
2. Use local caching
3. Implement delta updates (only send changes)

### WiFi Optimization

For better reliability:

1. Use 2.4GHz WiFi (better range)
2. Place router closer to printer
3. Use WiFi extender if needed
4. Assign static IP to display

```bash
# In your router, reserve IP for display MAC address
# Or configure static IP in WifiConfig.h
```

## Security Considerations

### API Access

Moonraker API is unauthenticated by default. To secure:

```ini
# In moonraker.conf
[authorization]
enabled: True
api_key_file: ~/.moonraker_api_key

# Generate API key
moonraker-api-key generate
```

Then update display firmware to include API key in requests.

### Network Isolation

Consider placing printer on separate VLAN:
- Isolates from main network
- Prevents unauthorized access
- Reduces attack surface

## Performance Tips

1. **Reduce polling frequency** for non-critical data
2. **Use WebSocket** instead of polling (future feature)
3. **Cache static data** (printer info, capabilities)
4. **Batch API requests** when possible

## Additional Resources

- Klipper Documentation: https://www.klipper3d.org/
- Moonraker API: https://moonraker.readthedocs.io/
- G-Code Reference: https://www.klipper3d.org/G-Codes.html
