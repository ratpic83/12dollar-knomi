# Hardware Guide

## Supported Display

This firmware is designed for ESP32-C3 based round displays with the following specifications:

### Display Specifications
- **Size**: 1.28 inch round LCD
- **Resolution**: 240x240 pixels
- **Driver**: GC9A01
- **MCU**: ESP32-C3 (single core, 400KB RAM)
- **Interface**: SPI
- **Power**: 5V via USB-C or 3.3V via pins

### Purchase Link
https://de.aliexpress.com/item/1005005453515690.html

**Price**: ~12€ (significantly cheaper than BTT Knomi)

## Differences from BTT Knomi

| Feature | BTT Knomi | This Clone |
|---------|-----------|------------|
| MCU | ESP32 Wroom (dual core, 520KB RAM) | ESP32-C3 (single core, 400KB RAM) |
| Price | ~40€ | ~12€ |
| Display | 1.28" Round LCD | 1.28" Round LCD |
| Mounting | Stealthburner specific | Universal |
| Firmware | Closed source | Open source |

## Pin Connections

### Default Pin Configuration

```
ESP32-C3 Pin | Function | Display Pin
-------------|----------|------------
GPIO 7       | MOSI     | SDA
GPIO 6       | SCK      | SCL
GPIO 10      | CS       | CS
GPIO 2       | DC       | DC
GPIO 3       | RST      | RES
GPIO 11      | BL       | BL (Backlight)
3.3V         | Power    | VCC
GND          | Ground   | GND
```

**Note**: Pin numbers may vary depending on your specific display module. Check the PCB markings or documentation.

### Power Requirements

- **Voltage**: 5V via USB-C or 3.3V via pins
- **Current**: ~200mA typical, ~500mA max
- **Backlight**: PWM controlled (if supported)

### Wiring to Printer

You have several options for powering the display:

#### Option 1: USB Power (Recommended)
- Connect USB-C cable to display
- Connect to printer's USB port or external power supply
- Simplest and safest option

#### Option 2: Direct Wiring
- Connect 5V and GND from printer board
- Ensure stable 5V supply (not from hotend heater circuit!)
- Add capacitor (100µF) near display for stability

#### Option 3: Toolhead Board
- If using CAN toolhead board, connect to 5V output
- Shorter wires, cleaner installation
- Ensure board can supply sufficient current

## Mounting Options

### 3D Printed Mounts

The display can be mounted in various locations:

1. **Stealthburner Mount** (if using Voron)
   - Replaces or supplements existing display
   - STL files available in community repositories

2. **Frame Mount**
   - Mount on printer frame
   - Good visibility
   - Easy access

3. **Toolhead Mount**
   - Direct view of print
   - Requires flexible cable management
   - May need longer wires

### Mounting Considerations

- **Vibration**: Secure firmly to prevent damage
- **Heat**: Keep away from heated bed and hotend
- **Cables**: Use strain relief to prevent wire fatigue
- **Viewing Angle**: Position for easy visibility

## Cable Management

### Wire Gauge
- **Power (5V, GND)**: 22-24 AWG
- **Signal (SPI)**: 26-28 AWG

### Cable Length
- **Maximum recommended**: 1 meter for SPI signals
- **Shorter is better** for signal integrity
- Use shielded cable if experiencing interference

### Cable Routing
1. Secure cables to prevent snagging
2. Use cable chains for moving parts
3. Avoid routing near stepper motors (EMI)
4. Keep away from heated components

## Troubleshooting Hardware Issues

### Display Not Powering On

**Check**:
- USB cable connection
- Power supply voltage (should be 5V ±0.25V)
- Backlight enable (some displays need BL pin high)

**Solutions**:
- Try different USB cable/port
- Measure voltage at display pins
- Check for shorts with multimeter

### Display Shows Garbage/Static

**Check**:
- SPI pin connections
- Cable length (too long?)
- Interference from stepper motors

**Solutions**:
- Verify pin configuration in firmware
- Shorten cables
- Add ferrite beads to cables
- Route cables away from motors

### Display Flickers

**Check**:
- Power supply stability
- Backlight PWM frequency
- Loose connections

**Solutions**:
- Add capacitor (100µF) near display
- Check all solder joints
- Use thicker power wires
- Adjust backlight settings in firmware

### Touch Not Working (if applicable)

Some displays include touch capability:

**Check**:
- Touch pins connected
- Touch library installed
- Calibration needed

**Note**: Current firmware doesn't implement touch. This is a future feature.

## Hardware Modifications

### Adding External Antenna (Optional)

For better WiFi range:
1. Some ESP32-C3 modules have U.FL connector
2. Connect external 2.4GHz antenna
3. May require resistor change on PCB (check module docs)

### Backlight Control

If your display supports PWM backlight:
1. Connect BL pin to PWM-capable GPIO
2. Modify firmware to enable PWM control
3. Add brightness adjustment feature

### Adding Buttons (Future Feature)

Physical buttons can be added:
1. Connect to available GPIO pins
2. Use internal pull-ups
3. Implement button handling in firmware
4. Add menu navigation

## Safety Notes

⚠️ **Important Safety Information**:

1. **Power Supply**
   - Never connect 12V/24V directly to display
   - Use proper voltage regulator if needed
   - Add fuse for protection

2. **Static Electricity**
   - ESP32-C3 is sensitive to ESD
   - Ground yourself before handling
   - Use ESD-safe workspace

3. **Heat**
   - Keep display below 60°C
   - Don't mount near heated bed
   - Ensure adequate ventilation

4. **Wiring**
   - Double-check polarity before powering
   - Use proper connectors (not loose wires)
   - Secure all connections

## Recommended Tools

- Soldering iron (if modifying)
- Multimeter (for testing)
- Wire strippers
- Heat shrink tubing
- Dupont connectors
- USB-C cable

## Additional Resources

- ESP32-C3 Datasheet: https://www.espressif.com/sites/default/files/documentation/esp32-c3_datasheet_en.pdf
- GC9A01 Display Driver: https://www.buydisplay.com/download/ic/GC9A01A.pdf
- TFT_eSPI Library: https://github.com/Bodmer/TFT_eSPI
