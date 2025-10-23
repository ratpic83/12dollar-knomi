/*
 * Environmental Sensor Integration
 *
 * Adds support for temperature/humidity sensors and environmental monitoring
 */

#ifndef ENVIRONMENTAL_H
#define ENVIRONMENTAL_H

#include <Arduino.h>

// Supported sensor types
enum SensorType {
  SENSOR_NONE,
  SENSOR_DHT11,
  SENSOR_DHT22,
  SENSOR_BME280,
  SENSOR_SHT30
};

// Environmental data structure
struct EnvironmentalData {
  bool valid;
  float temperature;      // Chamber temperature (°C)
  float humidity;         // Chamber humidity (%)
  float pressure;         // Atmospheric pressure (hPa) - if available
  unsigned long lastUpdate;
};

class EnvironmentalSensor {
public:
  EnvironmentalSensor();
  
  // Initialize sensor
  bool begin(SensorType type, uint8_t pin);
  
  // Read sensor data
  EnvironmentalData readData();
  
  // Check if sensor is available
  bool isAvailable() const { return sensorType != SENSOR_NONE; }
  
  // Get sensor type name
  const char* getSensorName() const;

private:
  SensorType sensorType;
  uint8_t sensorPin;
  EnvironmentalData lastData;
  
  // Sensor-specific implementations
  bool initDHT(uint8_t pin);
  bool initBME280();
  bool initSHT30();
  
  EnvironmentalData readDHT();
  EnvironmentalData readBME280();
  EnvironmentalData readSHT30();
};

#endif // ENVIRONMENTAL_H
