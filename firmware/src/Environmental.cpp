/*
 * Environmental Sensor Implementation
 */

#include "Environmental.h"

// Include sensor libraries
#include <DHT.h>
#include <Adafruit_BME280.h>
#include <Adafruit_SHT31.h>

// Sensor instances
DHT* dhtSensor = nullptr;
Adafruit_BME280* bmeSensor = nullptr;
Adafruit_SHT31* shtSensor = nullptr;

EnvironmentalSensor::EnvironmentalSensor() :
  sensorType(SENSOR_NONE),
  sensorPin(0) {
  lastData.valid = false;
  lastData.temperature = 0.0;
  lastData.humidity = 0.0;
  lastData.pressure = 0.0;
  lastData.lastUpdate = 0;
}

bool EnvironmentalSensor::begin(SensorType type, uint8_t pin) {
  sensorType = type;
  sensorPin = pin;
  
  switch (type) {
    case SENSOR_DHT11:
    case SENSOR_DHT22:
      return initDHT(pin);
      
    case SENSOR_BME280:
      return initBME280();
      
    case SENSOR_SHT30:
      return initSHT30();
      
    default:
      return false;
  }
}

EnvironmentalData EnvironmentalSensor::readData() {
  if (sensorType == SENSOR_NONE) {
    return lastData;
  }
  
  // Cache readings for 30 seconds to avoid overwhelming sensors
  if (millis() - lastData.lastUpdate < 30000 && lastData.valid) {
    return lastData;
  }
  
  EnvironmentalData data;
  data.lastUpdate = millis();
  
  switch (sensorType) {
    case SENSOR_DHT11:
    case SENSOR_DHT22:
      data = readDHT();
      break;
      
    case SENSOR_BME280:
      data = readBME280();
      break;
      
    case SENSOR_SHT30:
      data = readSHT30();
      break;
      
    default:
      data.valid = false;
      break;
  }
  
  lastData = data;
  return data;
}

const char* EnvironmentalSensor::getSensorName() const {
  switch (sensorType) {
    case SENSOR_DHT11: return "DHT11";
    case SENSOR_DHT22: return "DHT22";
    case SENSOR_BME280: return "BME280";
    case SENSOR_SHT30: return "SHT30";
    default: return "None";
  }
}

// DHT sensor implementation
bool EnvironmentalSensor::initDHT(uint8_t pin) {
  if (dhtSensor) delete dhtSensor;
  
  uint8_t dhtType = (sensorType == SENSOR_DHT11) ? DHT11 : DHT22;
  dhtSensor = new DHT(pin, dhtType);
  dhtSensor->begin();
  
  // Test reading
  delay(2000); // DHT sensors need 2 seconds to stabilize
  float temp = dhtSensor->readTemperature();
  return !isnan(temp);
}

EnvironmentalData EnvironmentalSensor::readDHT() {
  EnvironmentalData data;
  data.valid = false;
  
  if (!dhtSensor) return data;
  
  float temp = dhtSensor->readTemperature();
  float humidity = dhtSensor->readHumidity();
  
  if (!isnan(temp) && !isnan(humidity)) {
    data.valid = true;
    data.temperature = temp;
    data.humidity = humidity;
    data.pressure = 0.0; // DHT doesn't provide pressure
  }
  
  data.lastUpdate = millis();
  return data;
}

// BME280 sensor implementation
bool EnvironmentalSensor::initBME280() {
  if (bmeSensor) delete bmeSensor;
  
  bmeSensor = new Adafruit_BME280();
  return bmeSensor->begin(0x76); // Default I2C address
}

EnvironmentalData EnvironmentalSensor::readBME280() {
  EnvironmentalData data;
  data.valid = false;
  
  if (!bmeSensor) return data;
  
  data.valid = true;
  data.temperature = bmeSensor->readTemperature();
  data.humidity = bmeSensor->readHumidity();
  data.pressure = bmeSensor->readPressure() / 100.0; // Convert to hPa
  
  data.lastUpdate = millis();
  return data;
}

// SHT30 sensor implementation
bool EnvironmentalSensor::initSHT30() {
  if (shtSensor) delete shtSensor;
  
  shtSensor = new Adafruit_SHT31();
  return shtSensor->begin(0x44); // Default I2C address
}

EnvironmentalData EnvironmentalSensor::readSHT30() {
  EnvironmentalData data;
  data.valid = false;
  
  if (!shtSensor) return data;
  
  float temp = shtSensor->readTemperature();
  float humidity = shtSensor->readHumidity();
  
  if (!isnan(temp) && !isnan(humidity)) {
    data.valid = true;
    data.temperature = temp;
    data.humidity = humidity;
    data.pressure = 0.0; // SHT30 doesn't provide pressure
  }
  
  data.lastUpdate = millis();
  return data;
}
