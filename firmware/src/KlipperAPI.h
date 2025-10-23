/*
 * Klipper API Client
 * 
 * Handles communication with Klipper via Moonraker HTTP API
 */

#ifndef KLIPPER_API_H
#define KLIPPER_API_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Printer states
enum PrinterState {
  STATE_UNKNOWN,
  STATE_IDLE,
  STATE_PRINTING,
  STATE_PAUSED,
  STATE_COMPLETE,
  STATE_ERROR,
  STATE_STANDBY
};

// Printer status structure
struct PrinterStatus {
  bool connected;
  PrinterState state;
  
  // Temperatures
  float hotendTemp;
  float hotendTarget;
  float bedTemp;
  float bedTarget;
  
  // Print progress
  uint8_t printProgress;
  String fileName;
  uint32_t printTime;
  uint32_t printTimeLeft;
  
  // Environmental data (optional sensors)
  float chamberTemp;
  float chamberHumidity;
  float chamberPressure;
  
  // Position
  float posX;
  float posY;
  float posZ;
  
  // Speeds
  uint16_t feedrate;
  uint16_t flowrate;
  
  // Fans
  uint8_t partFanSpeed;
  
  // Flags
  bool homing;
  bool leveling;
};

class KlipperAPI {
public:
  KlipperAPI();
  
  // Initialization
  void init(const char* ip, uint16_t port);
  
  // API calls
  PrinterStatus getPrinterStatus();
  bool testConnection();
  
  // Individual queries
  bool getPrinterInfo();
  bool getTemperatures(PrinterStatus& status);
  bool getPrintStatus(PrinterStatus& status);
  bool getPosition(PrinterStatus& status);
  
  // Control commands (future expansion)
  bool pausePrint();
  bool resumePrint();
  bool cancelPrint();
  bool emergencyStop();
  
private:
  String baseURL;
  const char* klipperIP;
  uint16_t klipperPort;
  HTTPClient http;
  
  // Helper functions
  bool makeRequest(const char* endpoint, JsonDocument& doc);
  PrinterState parseState(const char* stateStr);
  float getJsonFloat(JsonDocument& doc, const char* key, float defaultValue = 0.0);
  int getJsonInt(JsonDocument& doc, const char* key, int defaultValue = 0);
  const char* getJsonString(JsonDocument& doc, const char* key, const char* defaultValue = "");
};

#endif // KLIPPER_API_H
