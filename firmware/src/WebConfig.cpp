/*
 * Web Configuration Portal Implementation
 */

#include "WebConfig.h"
#include <EEPROM.h>

#define AP_SSID "Knomi-Config"
#define AP_PASSWORD ""  // Open network for easy access
#define DNS_PORT 53

// EEPROM addresses for saving configuration
#define EEPROM_CONFIG_FLAG 0x00
#define EEPROM_SSID_START 0x01
#define EEPROM_PASSWORD_START 0x41
#define EEPROM_IP_START 0x81
#define EEPROM_PORT_START 0xC1
#define EEPROM_CONFIG_SIZE 512

WebConfig::WebConfig() :
  server(80),
  configured(false) {
}

void WebConfig::begin() {
  Serial.println("Starting Web Configuration Portal...");
  
  // Load existing configuration from EEPROM
  loadConfiguration();
  
  if (configured) {
    Serial.println("Configuration found, skipping portal");
    return;
  }
  
  // Start Access Point
  WiFi.softAP(AP_SSID, AP_PASSWORD);
  IPAddress apIP = WiFi.softAPIP();
  
  Serial.print("AP IP address: ");
  Serial.println(apIP);
  
  // Start DNS server for captive portal
  dnsServer.start(DNS_PORT, "*", apIP);
  
  // Setup web server routes
  server.on("/", HTTP_GET, std::bind(&WebConfig::handleRoot, this));
  server.on("/config", HTTP_GET, std::bind(&WebConfig::handleConfig, this));
  server.on("/save", HTTP_POST, std::bind(&WebConfig::handleSave, this));
  server.onNotFound(std::bind(&WebConfig::handleNotFound, this));
  
  server.begin();
  
  Serial.println("Web Configuration Portal started!");
  Serial.println("Connect to WiFi network: " + String(AP_SSID));
  Serial.println("Open browser to: http://192.168.4.1");
}

void WebConfig::handleClient() {
  dnsServer.processNextRequest();
  server.handleClient();
}

bool WebConfig::isConfigured() {
  return configured;
}

void WebConfig::handleRoot() {
  server.sendHeader("Location", "/config", true);
  server.send(302, "text/plain", "");
}

void WebConfig::handleConfig() {
  String html = getHTML();
  server.send(200, "text/html", html);
}

void WebConfig::handleSave() {
  if (server.hasArg("ssid") && server.hasArg("password") && 
      server.hasArg("klipper_ip") && server.hasArg("klipper_port")) {
    
    configuredSSID = server.arg("ssid");
    configuredPassword = server.arg("password");
    configuredKlipperIP = server.arg("klipper_ip");
    configuredKlipperPort = server.arg("klipper_port").toInt();
    
    // Save configuration
    saveConfiguration();
    configured = true;
    
    // Send success response
    String response = "<!DOCTYPE html><html><head><title>Configuration Saved</title><style>";
    response += "body{font-family:Arial;text-align:center;padding:50px;}";
    response += ".success{color:#28a745;font-size:24px;margin:20px;}";
    response += ".info{color:#666;margin:10px;}";
    response += "</style></head><body>";
    response += "<div class='success'>✓ Configuration Saved!</div>";
    response += "<div class='info'>Restarting device...</div>";
    response += "<div class='info'>Please reconnect to your WiFi network.</div>";
    response += "</body></html>";
    
    server.send(200, "text/html", response);
    
    // Restart device after short delay
    delay(2000);
    ESP.restart();
  } else {
    server.send(400, "text/plain", "Missing configuration parameters");
  }
}

void WebConfig::handleNotFound() {
  // Redirect all unknown requests to config page (captive portal)
  server.sendHeader("Location", "/config", true);
  server.send(302, "text/plain", "");
}

String WebConfig::getHTML() {
  String html = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Knomi Clone Configuration</title>";
  html += getCSS();
  html += "</head><body>";
  
  html += "<div class='container'>";
  html += "<div class='header'>";
  html += "<h1>🔧 Knomi Clone Setup</h1>";
  html += "<p>Configure your display for Klipper integration</p>";
  html += "</div>";
  
  html += "<form action='/save' method='POST' class='config-form'>";
  html += "<div class='section'>";
  html += "<h2>📶 WiFi Settings</h2>";
  html += createFormField("WiFi Network", "ssid", WIFI_SSID, "text", "Enter your WiFi SSID");
  html += createFormField("WiFi Password", "password", WIFI_PASSWORD, "password", "Enter your WiFi password");
  html += "</div>";
  
  html += "<div class='section'>";
  html += "<h2>🖨️ Klipper Settings</h2>";
  html += createFormField("Klipper IP Address", "klipper_ip", KLIPPER_IP, "text", "192.168.1.100");
  html += createFormField("Klipper Port", "klipper_port", String(KLIPPER_PORT), "number", "7125");
  html += "</div>";
  
  html += "<div class='section'>";
  html += "<button type='submit' class='save-btn'>💾 Save Configuration</button>";
  html += "</div>";
  html += "</form>";
  
  html += "<div class='footer'>";
  html += "<p>After saving, the device will restart and connect to your network.</p>";
  html += "</div>";
  html += "</div>";
  
  html += getJS();
  html += "</body></html>";
  
  return html;
}

String WebConfig::getCSS() {
  String css = "<style>";
  css += "body{font-family:'Segoe UI',Arial,sans-serif;margin:0;padding:20px;background:linear-gradient(135deg,#667eea 0%,#764ba2 100%);min-height:100vh;}";
  css += ".container{max-width:400px;margin:0 auto;background:white;border-radius:12px;padding:30px;box-shadow:0 10px 30px rgba(0,0,0,0.2);}";
  css += ".header{text-align:center;margin-bottom:30px;}";
  css += ".header h1{color:#333;margin:0;font-size:28px;}";
  css += ".header p{color:#666;margin:10px 0;}";
  css += ".section{margin-bottom:25px;}";
  css += ".section h2{color:#444;font-size:18px;margin:0 0 15px 0;padding-bottom:5px;border-bottom:2px solid #667eea;}";
  css += ".form-group{margin-bottom:15px;}";
  css += ".form-group label{display:block;color:#555;font-weight:500;margin-bottom:5px;}";
  css += ".form-group input{width:100%;padding:12px;border:2px solid #ddd;border-radius:6px;font-size:16px;transition:border-color 0.3s;}";
  css += ".form-group input:focus{border-color:#667eea;outline:none;}";
  css += ".save-btn{background:linear-gradient(135deg,#28a745 0%,#20c997 100%);color:white;border:none;padding:15px 30px;border-radius:8px;font-size:16px;font-weight:600;cursor:pointer;width:100%;transition:transform 0.2s;}";
  css += ".save-btn:hover{transform:translateY(-2px);}";
  css += ".footer{text-align:center;color:#888;font-size:14px;margin-top:20px;}";
  css += "</style>";
  return css;
}

String WebConfig::getJS() {
  String js = "<script>";
  js += "function validateForm(){";
  js += "var ssid=document.querySelector('[name=ssid]').value;";
  js += "var password=document.querySelector('[name=password]').value;";
  js += "var ip=document.querySelector('[name=klipper_ip]').value;";
  js += "if(!ssid.trim()){alert('WiFi SSID is required');return false;}";
  js += "if(!password.trim()){alert('WiFi password is required');return false;}";
  js += "if(!ip.trim()){alert('Klipper IP address is required');return false;}";
  js += "return confirm('Save configuration and restart device?');";
  js += "}";
  js += "document.querySelector('form').onsubmit=validateForm;";
  js += "</script>";
  return js;
}

String WebConfig::createFormField(const char* label, const char* name, const char* value, const char* type, const char* placeholder) {
  String field = "<div class='form-group'>";
  field += "<label for='" + String(name) + "'>" + String(label) + "</label>";
  field += "<input type='" + String(type) + "' name='" + String(name) + "' value='" + String(value) + "' placeholder='" + String(placeholder) + "'>";
  field += "</div>";
  return field;
}

void WebConfig::saveConfiguration() {
  EEPROM.begin(EEPROM_CONFIG_SIZE);
  
  // Write configuration flag
  EEPROM.write(EEPROM_CONFIG_FLAG, 0xAA);
  
  // Write SSID
  for (int i = 0; i < configuredSSID.length() && i < 64; i++) {
    EEPROM.write(EEPROM_SSID_START + i, configuredSSID[i]);
  }
  EEPROM.write(EEPROM_SSID_START + configuredSSID.length(), 0);
  
  // Write password
  for (int i = 0; i < configuredPassword.length() && i < 64; i++) {
    EEPROM.write(EEPROM_PASSWORD_START + i, configuredPassword[i]);
  }
  EEPROM.write(EEPROM_PASSWORD_START + configuredPassword.length(), 0);
  
  // Write IP
  for (int i = 0; i < configuredKlipperIP.length() && i < 64; i++) {
    EEPROM.write(EEPROM_IP_START + i, configuredKlipperIP[i]);
  }
  EEPROM.write(EEPROM_IP_START + configuredKlipperIP.length(), 0);
  
  // Write port (2 bytes)
  EEPROM.write(EEPROM_PORT_START, configuredKlipperPort & 0xFF);
  EEPROM.write(EEPROM_PORT_START + 1, (configuredKlipperPort >> 8) & 0xFF);
  
  EEPROM.commit();
  Serial.println("Configuration saved to EEPROM");
}

void WebConfig::loadConfiguration() {
  EEPROM.begin(EEPROM_CONFIG_SIZE);
  
  // Check configuration flag
  if (EEPROM.read(EEPROM_CONFIG_FLAG) != 0xAA) {
    configured = false;
    return;
  }
  
  // Read SSID
  configuredSSID = "";
  for (int i = 0; i < 64; i++) {
    char c = EEPROM.read(EEPROM_SSID_START + i);
    if (c == 0) break;
    configuredSSID += c;
  }
  
  // Read password
  configuredPassword = "";
  for (int i = 0; i < 64; i++) {
    char c = EEPROM.read(EEPROM_PASSWORD_START + i);
    if (c == 0) break;
    configuredPassword += c;
  }
  
  // Read IP
  configuredKlipperIP = "";
  for (int i = 0; i < 64; i++) {
    char c = EEPROM.read(EEPROM_IP_START + i);
    if (c == 0) break;
    configuredKlipperIP += c;
  }
  
  // Read port
  configuredKlipperPort = EEPROM.read(EEPROM_PORT_START) | (EEPROM.read(EEPROM_PORT_START + 1) << 8);
  
  configured = (configuredSSID.length() > 0 && configuredKlipperIP.length() > 0);
  
  if (configured) {
    Serial.println("Configuration loaded from EEPROM:");
    Serial.println("SSID: " + configuredSSID);
    Serial.println("IP: " + configuredKlipperIP);
    Serial.println("Port: " + String(configuredKlipperPort));
  }
}
