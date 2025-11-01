#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "DisplayDriver.h"
#include "UIManager.h"
#include "KlipperAPI.h"
#include "Environmental.h"
#include "TouchDriver.h"
#include "WifiConfig.h"

// Global instances
DisplayDriver display;
UIManager ui;
KlipperAPI api;
EnvironmentalSensor envSensor;
TouchDriver touchDriver;

// Theme cycling button (GPIO 9 - can connect a button here)
const int THEME_BUTTON_PIN = 9;
unsigned long lastButtonPress = 0;
const unsigned long DEBOUNCE_DELAY = 500; // 500ms debounce

void setup() {
  Serial.begin(115200);
  delay(2000); // Longer delay for serial to stabilize
  Serial.println("\n\n========================================");
  Serial.println("=== Knomi Clone v1.3 - Starting ===");
  Serial.println("========================================");
  
  Serial.print("Chip Model: ");
  Serial.println(ESP.getChipModel());
  Serial.print("Chip Revision: ");
  Serial.println(ESP.getChipRevision());
  Serial.print("Flash Size: ");
  Serial.println(ESP.getFlashChipSize());

  // Initialize WiFi FIRST before display (critical for ESP32-2424S012C)
  Serial.println("\n[1/5] Initializing WiFi FIRST...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("      Connecting to: ");
  Serial.println(WIFI_SSID);
  
  // Wait for WiFi connection BEFORE initializing display
  int wifiAttempts = 0;
  Serial.print("      ");
  while (WiFi.status() != WL_CONNECTED && wifiAttempts < 20) {
    delay(500);
    Serial.print(".");
    wifiAttempts++;
  }
  Serial.println();
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("      WiFi connected!");
    Serial.print("      IP: ");
    Serial.println(WiFi.localIP());
    // CRITICAL: Wait for WiFi to fully stabilize before SPI
    delay(1000);
    Serial.println("      WiFi stabilized");
  } else {
    Serial.println("      WiFi failed!");
    Serial.print("      Status: ");
    Serial.println(WiFi.status());
  }
  
  // Initialize theme button pin
  Serial.println("\n[2/5] Configuring button pin...");
  pinMode(THEME_BUTTON_PIN, INPUT_PULLUP);
  Serial.println("      Button pin OK");

  // Initialize display AFTER WiFi is stable
  Serial.println("[3/5] Initializing display...");
  display.init();
  Serial.println("      Display hardware OK");
  ui.init(&display);
  Serial.println("      UI Manager OK");

  // Initialize environmental sensor (BME280 on I2C) - TEMPORARILY DISABLED
  Serial.println("Skipping environmental sensor initialization");
  // if (envSensor.begin(SENSOR_BME280, 0)) {
  //   Serial.println("Environmental sensor initialized");
  // } else {
  //   Serial.println("Environmental sensor not found");
  // }

  // Initialize touch driver (FT6236 on I2C)
  Serial.println("\n[4/5] Initializing touch driver...");
  // ESP32-2424S012C pins: SDA=4, SCL=5, RST=1, INT=0
  if (touchDriver.begin(4, 5, 1, 0)) {
    Serial.println("      Touch driver initialized (FT6236)");
    touchDriver.enableGestures(true);
    Serial.println("      Gestures enabled");
  } else {
    Serial.println("      Touch driver not found (non-critical)");
  }

  // Check if device is configured
  // webConfig.begin(); // Temporarily disabled for testing
  
  if (true) { // webConfig.isConfigured()) {
    // Device is configured, proceed with normal operation
    Serial.println("Device configured, starting normal operation...");
    
    // Show boot screen
    ui.showBootScreen();
    delay(2000);

    // Check WiFi status (already connected in setup)
    display.clear();
    display.setTextColor(display.getThemeColors().text);
    
    if (WiFi.status() == WL_CONNECTED) {
      // WiFi connected - show IP briefly
      display.drawCenteredText("WiFi OK!", 80, 2);
      String ipStr = WiFi.localIP().toString();
      display.drawCenteredText(ipStr, 110, 1);
      delay(2000);
      
      // Initialize Klipper API
      Serial.println("      Initializing Klipper API...");
      api.init(KLIPPER_IP, KLIPPER_PORT);
      
      // Test connection to Klipper
      display.clear();
      display.drawCenteredText("Connecting to", 80, 1);
      display.drawCenteredText(KLIPPER_IP, 100, 1);
      String portStr = ":" + String(KLIPPER_PORT);
      display.drawCenteredText(portStr, 120, 1);
      
      // Simple connectivity test first
      Serial.println("      Testing HTTP connectivity...");
      HTTPClient http;
      http.setTimeout(5000);
      http.setConnectTimeout(5000);
      String testUrl = "http://" + String(KLIPPER_IP) + ":" + String(KLIPPER_PORT) + "/server/info";
      Serial.print("      URL: ");
      Serial.println(testUrl);
      
      bool httpBegin = http.begin(testUrl);
      Serial.print("      HTTP begin: ");
      Serial.println(httpBegin ? "OK" : "FAILED");
      
      int httpCode = -1;
      if (httpBegin) {
        httpCode = http.GET();
        Serial.print("      HTTP code: ");
        Serial.println(httpCode);
        http.end();
      }
      
      Serial.println("      Testing Klipper API...");
      bool connected = (httpCode == 200);
      
      display.clear();
      if (connected) {
        // Successfully connected to Klipper
        display.setTextColor(display.getThemeColors().accent);
        display.drawCenteredText("Connected!", 100, 2);
        Serial.println("      Connected to Klipper!");
        delay(1500);
      } else {
        // Klipper not reachable - use demo mode
        display.setTextColor(display.getThemeColors().secondary);
        display.drawCenteredText("Connection", 80, 1);
        display.drawCenteredText("Failed", 100, 2);
        display.setTextColor(display.getThemeColors().text);
        display.drawCenteredText("Using Demo Mode", 130, 1);
        Serial.println("      Failed to connect to Klipper - using demo mode");
        delay(3000);
      }
    } else {
      // WiFi failed - use demo mode
      display.drawCenteredText("WiFi FAILED", 80, 2);
      display.setTextColor(display.getThemeColors().secondary);
      display.drawCenteredText("Demo mode", 110, 1);
      Serial.println("      WiFi failed - using demo mode");
      delay(2000);
    }
  } else {
    // Device not configured, show configuration screen
    Serial.println("Device not configured, starting web portal...");
    
    // Show configuration message on display
    display.clear();
    display.setTextColor(display.getThemeColors().text);
    display.drawCenteredText("Configuration", 100, 2);
    display.drawCenteredText("Required", 130, 2);
    display.setTextColor(display.getThemeColors().secondary);
    display.drawCenteredText("Connect to:", 170, 1);
    display.drawCenteredText("Knomi-Config", 190, 1);
    display.drawCenteredText("Open: 192.168.4.1", 220, 1);
  }

  delay(2000);
}

void loop() {
  // Update printer status every 2 seconds
  static unsigned long lastUpdate = 0;
  static float demoProgress = 0;
  
  if (millis() - lastUpdate > 2000) {
    lastUpdate = millis();

    PrinterStatus status;
    
    // Check if WiFi is connected (real data) or demo mode (fake data)
    if (WiFi.status() == WL_CONNECTED) {
      // Real data from Klipper
      status = api.getPrinterStatus();
      
      // Add environmental data
      EnvironmentalData envData = envSensor.readData();
      if (envData.valid) {
        status.chamberTemp = envData.temperature;
        status.chamberHumidity = envData.humidity;
        status.chamberPressure = envData.pressure;
      }
    } else {
      // DEMO MODE - Generate fake printer data
      status.connected = true;
      status.state = STATE_PRINTING;
      status.hotendTemp = 210.5 + (rand() % 10) / 10.0;
      status.hotendTarget = 215.0;
      status.bedTemp = 60.2 + (rand() % 10) / 10.0;
      status.bedTarget = 60.0;
      status.chamberTemp = 28.5;
      status.chamberHumidity = 45.0;
      status.chamberPressure = 1013.0;
      status.printProgress = (int)demoProgress;
      status.printTimeLeft = 3600 - (demoProgress * 36);
      status.partFanSpeed = 100;
      status.flowrate = 100;
      status.feedrate = 100;
      
      // Increment demo progress
      demoProgress += 0.5;
      if (demoProgress > 100) demoProgress = 0;
    }
    
    ui.updateStatus(status);

    // Debug output with state name
    const char* stateNames[] = {"UNKNOWN", "IDLE", "STANDBY", "PRINTING", "PAUSED", "COMPLETE", "ERROR"};
    Serial.printf("[UPDATE] State: %s (%d), Progress: %d%%, Hotend: %.1f/%.1f, Bed: %.1f/%.1f\n",
                  stateNames[status.state], status.state, status.printProgress,
                  status.hotendTemp, status.hotendTarget,
                  status.bedTemp, status.bedTarget);
  }

  // Update UI animations
  ui.update();

  // Update touch driver and handle touch events
  touchDriver.update();
  
  // Handle touch gestures
  TouchEvent touchEvent = touchDriver.getEvent();
  if (touchEvent != TOUCH_NONE) {
    ui.handleTouchEvent(touchEvent, touchDriver.getPoint());
    
    // Debug touch events
    Serial.printf("Touch: Event=%d, Point=(%d,%d)\n", 
                  touchEvent, touchDriver.getPoint().x, touchDriver.getPoint().y);
  }

  delay(10);
}
