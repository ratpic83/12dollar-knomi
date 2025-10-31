/*
 * Web Configuration Portal
 *
 * Provides a web interface for configuring WiFi and Klipper settings
 */

#ifndef WEB_CONFIG_H
#define WEB_CONFIG_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include "WifiConfig.h"

class WebConfig {
public:
  WebConfig();
  
  // Initialize web configuration portal
  void begin();
  
  // Handle client requests
  void handleClient();
  
  // Check if configuration is complete
  bool isConfigured();
  
  // Configuration persistence
  void saveConfiguration();
  void loadConfiguration();
  
  // Get current configuration
  String getSSID() { return configuredSSID; }
  String getPassword() { return configuredPassword; }
  String getKlipperIP() { return configuredKlipperIP; }
  uint16_t getKlipperPort() { return configuredKlipperPort; }

private:
  WebServer server;
  DNSServer dnsServer;
  
  // Configuration state
  bool configured;
  String configuredSSID;
  String configuredPassword;
  String configuredKlipperIP;
  uint16_t configuredKlipperPort;
  
  // HTML content
  String getHTML();
  String getCSS();
  String getJS();
  
  // Request handlers
  void handleRoot();
  void handleConfig();
  void handleSave();
  void handleNotFound();
  
  // Helper functions
  String createFormField(const char* label, const char* name, const char* value, const char* type = "text", const char* placeholder = "");
};

#endif // WEB_CONFIG_H
