# Environmental Sensor Setup Guide

## Overview

The Knomi Clone supports environmental monitoring with affordable sensors to track chamber conditions including temperature, humidity, and atmospheric pressure.

## Supported Sensors

### BME280 (Recommended - ~$5)
- **Temperature**: -40°C to +85°C (±1°C accuracy)
- **Humidity**: 0-100% RH (±3% accuracy)
- **Pressure**: 300-1100 hPa (±1 hPa accuracy)
- **Interface**: I2C (0x76 or 0x77)
- **Power**: 3.3V

### DHT11 (~$3)
- **Temperature**: 0°C to +50°C (±2°C accuracy)
- **Humidity**: 20-90% RH (±5% accuracy)
- **Interface**: Digital pin
- **Power**: 3.3-5V

### DHT22 (~$4)
- **Temperature**: -40°C to +80°C (±0.5°C accuracy)
- **Humidity**: 0-100% RH (±2% accuracy)
- **Interface**: Digital pin
- **Power**: 3.3-5V

### SHT30 (~$8)
- **Temperature**: -40°C to +125°C (±0.2°C accuracy)
- **Humidity**: 0-100% RH (±2% accuracy)
- **Interface**: I2C (0x44 or 0x45)
- **Power**: 3.3V

## Hardware Connections

### ESP32-C3 Round Display Pinout

```
ESP32-C3 Round Display
┌─────────────────┐
│ VCC  GND  IO2  │ ← Display connections
│ IO3  IO4  IO5  │
│ IO6  IO7  RST  │ ← SPI for display
│ IO8  IO9  IO10 │ ← Available for sensors
│ IO18 IO19 IO20 │ ← I2C available
└─────────────────┘
```

### BME280/SHT30 (I2C)

```
ESP32-C3     BME280/SHT30
───────     ───────────
3.3V    →   VCC
GND     →   GND
IO18    →   SDA
IO19    →   SCL
```

### DHT11/DHT22 (Digital)

```
ESP32-C3     DHT11/DHT22
───────     ──────────
3.3V    →   VCC
GND     →   GND
IO9     →   DATA (with 4.7K pullup resistor)
```

## Software Configuration

### Automatic Detection

The firmware automatically detects sensors on boot. No configuration changes required!

1. **BME280/SHT30**: Connected to I2C bus (IO18/IO19)
2. **DHT**: Connected to IO9 pin

### Display Integration

Environmental data appears on the **idle screen**:

```
25.3°C  45%     ← Chamber temp & humidity
```

- Shows temperature in °C
- Shows humidity as percentage
- Theme-aware colors (uses secondary theme color)
- Only displays available data

## Troubleshooting

### Sensor Not Detected
- **Check connections**: Ensure correct I2C/digital pins
- **Power supply**: Verify 3.3V power (not 5V for I2C sensors)
- **I2C address**: BME280 can be 0x76 or 0x77
- **Serial output**: Check boot messages for sensor detection

### Incorrect Readings
- **Calibration**: Some sensors need calibration in extreme conditions
- **Placement**: Avoid placing near heat sources
- **Ventilation**: Ensure proper air circulation around sensor

### Data Not Displaying
- **Sensor validity**: Check serial output for "Environmental sensor initialized"
- **Theme colors**: Data uses theme secondary colors
- **Cache timing**: Readings update every 30 seconds

## Advanced Configuration

### Custom Pin Assignments

To use different pins, modify `src/main.cpp`:

```cpp
// For I2C sensors (BME280/SHT30)
envSensor.begin(SENSOR_BME280, 0); // Uses default I2C pins (IO18/IO19)

// For DHT sensors
// Note: Currently fixed to IO9, modify Environmental.cpp to change
```

### Sensor Calibration

Some sensors may need offset calibration:

```cpp
// In Environmental.cpp, add offsets if needed
data.temperature += CALIBRATION_OFFSET_TEMP;
data.humidity += CALIBRATION_OFFSET_HUMIDITY;
```

## Performance Impact

- **Memory**: ~2KB additional RAM for sensor libraries
- **Power**: Minimal additional current draw
- **Update Rate**: 30-second intervals to protect sensors
- **Boot Time**: +1-2 seconds for sensor initialization

## Recommended Setup

1. **Mount sensor** away from heat sources
2. **Ensure ventilation** around the sensor
3. **Use appropriate wire lengths** (keep I2C short)
4. **Power from 3.3V** pin on display
5. **Test readings** with serial monitor

## Cost Breakdown

- **BME280**: $4.50 - Best value for full environmental monitoring
- **DHT22**: $3.50 - Good basic temperature/humidity
- **SHT30**: $7.50 - High precision for demanding applications

Total additional cost: $3-8 for environmental monitoring!
