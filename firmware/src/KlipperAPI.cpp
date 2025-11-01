/*
 * Klipper API Implementation
 */

#include "KlipperAPI.h"
#include "WifiConfig.h"
#include <WiFi.h>

KlipperAPI::KlipperAPI() {
}

void KlipperAPI::init(const char* ip, uint16_t port) {
  klipperIP = ip;
  klipperPort = port;
  baseURL = "http://" + String(ip) + ":" + String(port);
  
  #if DEBUG_API
  Serial.print("Klipper API initialized: ");
  Serial.println(baseURL);
  #endif
}

bool KlipperAPI::testConnection() {
  StaticJsonDocument<512> doc;
  return makeRequest("/server/info", doc);
}

PrinterStatus KlipperAPI::getPrinterStatus() {
  PrinterStatus status;
  status.connected = false;
  status.state = STATE_UNKNOWN;
  
  // Initialize all values
  status.hotendTemp = 0;
  status.hotendTarget = 0;
  status.bedTemp = 0;
  status.bedTarget = 0;
  status.chamberTemp = 0;
  status.chamberHumidity = 0;
  status.chamberPressure = 0;
  status.printProgress = 0;
  status.fileName = "";
  status.printTime = 0;
  status.printTimeLeft = 0;
  status.posX = 0;
  status.posY = 0;
  status.posZ = 0;
  status.feedrate = 100;
  status.flowrate = 100;
  status.partFanSpeed = 0;
  status.homing = false;
  status.leveling = false;
  
  // Query printer status with minimal data
  StaticJsonDocument<1024> doc;
  
  // Query with display_status for progress
  if (!makeRequest("/printer/objects/query?extruder&heater_bed&print_stats&display_status", doc)) {
    return status;
  }
  
  status.connected = true;
  
  // Parse response
  JsonObject result = doc["result"]["status"];
  
  // Temperatures
  if (result.containsKey("extruder")) {
    status.hotendTemp = result["extruder"]["temperature"] | 0.0;
    status.hotendTarget = result["extruder"]["target"] | 0.0;
  }
  
  if (result.containsKey("heater_bed")) {
    status.bedTemp = result["heater_bed"]["temperature"] | 0.0;
    status.bedTarget = result["heater_bed"]["target"] | 0.0;
  }
  
  // Print status
  if (result.containsKey("print_stats")) {
    const char* stateStr = result["print_stats"]["state"] | "unknown";
    status.state = parseState(stateStr);
    status.fileName = result["print_stats"]["filename"] | "";
    status.printTime = result["print_stats"]["print_duration"] | 0;
  }
  
  // Display status (progress)
  if (result.containsKey("display_status")) {
    float progress = result["display_status"]["progress"] | 0.0;
    status.printProgress = (uint8_t)(progress * 100);
    #if DEBUG_API
    Serial.printf("Progress from display_status: %.2f -> %d%%\n", progress, status.printProgress);
    #endif
  } else {
    #if DEBUG_API
    Serial.println("Warning: display_status not in response!");
    #endif
  }
  
  // Position
  if (result.containsKey("gcode_move")) {
    JsonArray position = result["gcode_move"]["gcode_position"];
    if (position.size() >= 3) {
      status.posX = position[0] | 0.0;
      status.posY = position[1] | 0.0;
      status.posZ = position[2] | 0.0;
    }
    
    status.feedrate = result["gcode_move"]["speed_factor"] | 1.0;
    status.feedrate *= 100;
    status.flowrate = result["gcode_move"]["extrude_factor"] | 1.0;
    status.flowrate *= 100;
  }
  
  // Fan speed
  if (result.containsKey("fan")) {
    float fanSpeed = result["fan"]["speed"] | 0.0;
    status.partFanSpeed = (uint8_t)(fanSpeed * 100);
  }
  
  // Check for homing/leveling from custom macros
  JsonObject variables = doc["result"]["status"];
  if (variables.containsKey("HomeSetVar")) {
    status.homing = variables["HomeSetVar"]["homing"] | false;
  }
  if (variables.containsKey("BedLevelVar")) {
    status.leveling = variables["BedLevelVar"]["leveling"] | false;
  }
  
  #if DEBUG_API
  Serial.printf("State: %d, Hotend: %.1f/%.1f, Bed: %.1f/%.1f, Progress: %d%%\n",
                status.state, status.hotendTemp, status.hotendTarget,
                status.bedTemp, status.bedTarget, status.printProgress);
  #endif
  
  return status;
}

bool KlipperAPI::makeRequest(const char* endpoint, JsonDocument& doc) {
  #if DEBUG_API
    Serial.print("API Request to: ");
    Serial.print(klipperIP);
    Serial.print(":");
    Serial.print(klipperPort);
    Serial.println(endpoint);
  #endif
  
  // Use HTTPClient for better timeout handling
  HTTPClient http;
  http.setTimeout(3000);  // 3 second timeout
  http.setConnectTimeout(3000);  // 3 second connection timeout
  
  String url = baseURL + String(endpoint);
  
  #if DEBUG_API
    Serial.print("URL: ");
    Serial.println(url);
  #endif
  
  if (!http.begin(url)) {
    #if DEBUG_API
      Serial.println("HTTP begin failed!");
    #endif
    return false;
  }
  
  http.addHeader("Accept", "application/json");
  
  int httpCode = http.GET();
  
  #if DEBUG_API
    Serial.print("HTTP Code: ");
    Serial.println(httpCode);
  #endif
  
  if (httpCode != HTTP_CODE_OK) {
    #if DEBUG_API
      Serial.println("HTTP request failed!");
    #endif
    http.end();
    return false;
  }
  
  String payload = http.getString();
  http.end();
  
  #if DEBUG_API
    Serial.print("Response length: ");
    Serial.println(payload.length());
  #endif
  
  // Parse JSON
  DeserializationError error = deserializeJson(doc, payload);
  
  if (error) {
    #if DEBUG_API
      Serial.print("JSON parse error: ");
      Serial.println(error.c_str());
    #endif
    return false;
  }
  
  return true;
}

PrinterState KlipperAPI::parseState(const char* stateStr) {
  if (strcmp(stateStr, "standby") == 0) return STATE_STANDBY;
  if (strcmp(stateStr, "ready") == 0) return STATE_IDLE;
  if (strcmp(stateStr, "printing") == 0) return STATE_PRINTING;
  if (strcmp(stateStr, "paused") == 0) return STATE_PAUSED;
  if (strcmp(stateStr, "complete") == 0) return STATE_COMPLETE;
  if (strcmp(stateStr, "error") == 0) return STATE_ERROR;
  return STATE_UNKNOWN;
}

bool KlipperAPI::pausePrint() {
  DynamicJsonDocument doc(256);
  return makeRequest("/printer/print/pause", doc);
}

bool KlipperAPI::resumePrint() {
  DynamicJsonDocument doc(256);
  return makeRequest("/printer/print/resume", doc);
}

bool KlipperAPI::cancelPrint() {
  DynamicJsonDocument doc(256);
  return makeRequest("/printer/print/cancel", doc);
}

bool KlipperAPI::emergencyStop() {
  DynamicJsonDocument doc(256);
  return makeRequest("/printer/emergency_stop", doc);
}
