#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "DisplayDriver.h"
#include "UIManager.h"
#include "KlipperAPI.h"
#include "Environmental.h"
#include "TouchDriver.h"
#include "WifiConfig.h"

// External declarations for spaceman animation (defined in spaceman_data.cpp)
extern const uint16_t* spaceman_frames[];
#define SPACEMAN_FRAME_COUNT 5
#define SPACEMAN_WIDTH 120
#define SPACEMAN_HEIGHT 120

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
    
    // Show boot screen (PAWE)
    ui.showBootScreen();
    delay(2000);
    
    // Show Spaceman animation after boot (3 seconds)
    Serial.println("[BOOT] Showing Spaceman GIF animation...");
    
    // Play GIF animation - 5 frames at 120x120, scaled 2x to 240x240, loop for 3 seconds
    unsigned long spacemanStart = millis();
    while (millis() - spacemanStart < 3000) {
      unsigned long elapsed = millis() - spacemanStart;
      int frameIndex = (elapsed / 200) % SPACEMAN_FRAME_COUNT;  // 200ms per frame = 5fps
      
      // Get the current frame data from array
      const uint16_t* frameData = spaceman_frames[frameIndex];
      
      if (frameData) {
        // Scale 120x120 to 240x240 (2x zoom) using LovyanGFX
        // pushImageRotateZoom(x, y, center_x, center_y, angle, zoom_x, zoom_y, width, height, data)
        // Position at screen center (120,120), source center at (60,60)
        display.getTFT()->pushImageRotateZoom(120, 120, 60, 60, 0, 2.0, 2.0, SPACEMAN_WIDTH, SPACEMAN_HEIGHT, frameData);
      }
      
      delay(200);  // 200ms per frame
    }
    
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
      
      // Retry connection with better error handling
      Serial.println("      Testing HTTP connectivity...");
      String testUrl = "http://" + String(KLIPPER_IP) + ":" + String(KLIPPER_PORT) + "/server/info";
      Serial.print("      URL: ");
      Serial.println(testUrl);
      
      bool connected = false;
      int maxRetries = 3;
      
      for (int attempt = 1; attempt <= maxRetries && !connected; attempt++) {
        Serial.printf("      Attempt %d/%d...\n", attempt, maxRetries);
        
        HTTPClient http;
        http.setTimeout(3000);
        http.setConnectTimeout(3000);
        
        if (http.begin(testUrl)) {
          int httpCode = http.GET();
          Serial.print("      HTTP code: ");
          Serial.println(httpCode);
          
          if (httpCode == 200) {
            connected = true;
            Serial.println("      Connection successful!");
          } else if (httpCode > 0) {
            Serial.printf("      HTTP error: %d\n", httpCode);
          } else {
            Serial.printf("      Connection error: %s\n", http.errorToString(httpCode).c_str());
          }
          http.end();
        } else {
          Serial.println("      HTTP begin failed!");
        }
        
        if (!connected && attempt < maxRetries) {
          Serial.println("      Retrying in 1 second...");
          delay(1000);
        }
      }
      
      Serial.println("      Testing Klipper API...");
      
      display.clear();
      if (connected) {
        // Successfully connected to Klipper
        display.setTextColor(display.getThemeColors().accent);
        display.drawCenteredText("Connected!", 100, 2);
        Serial.println("      Connected to Klipper!");
        delay(1500);
      } else {
        // Klipper not reachable - will retry in loop
        display.setTextColor(display.getThemeColors().warning);
        display.drawCenteredText("Connection", 80, 1);
        display.drawCenteredText("Failed", 100, 2);
        display.setTextColor(display.getThemeColors().secondary);
        display.drawCenteredText("Will retry...", 130, 1);
        Serial.println("      Failed to connect to Klipper - will retry in loop");
        delay(2000);
      }
    } else {
      // WiFi failed - show error
      display.setTextColor(display.getThemeColors().error);
      display.drawCenteredText("WiFi FAILED", 80, 2);
      display.setTextColor(display.getThemeColors().secondary);
      display.drawCenteredText("Check credentials", 110, 1);
      Serial.println("      WiFi failed - check WifiConfig.h");
      delay(3000);
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
  // Update printer status every 5 seconds (reduced from 2s to minimize network load)
  static unsigned long lastUpdate = 0;
  static uint8_t lastValidProgress = 0;  // Keep last known progress
  static unsigned long lastConnectionAttempt = 0;
  static int connectionRetries = 0;
  
  if (millis() - lastUpdate > 5000) {
    lastUpdate = millis();

    PrinterStatus status;
    
    // Check WiFi connection first
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("[ERROR] WiFi disconnected! Attempting reconnect...");
      WiFi.reconnect();
      delay(1000);
    }
    
    // Try to get printer status
    if (WiFi.status() == WL_CONNECTED) {
      status = api.getPrinterStatus();
      
      // Check if connection succeeded
      if (!status.connected) {
        connectionRetries++;
        Serial.printf("[RETRY] Connection failed (attempt %d). Retrying...\n", connectionRetries);
        
        // Show retry message on display every 3 attempts
        if (connectionRetries % 3 == 0) {
          display.clear();
          display.setTextColor(display.getThemeColors().warning);
          display.drawCenteredText("Connecting...", 100, 2);
          display.setTextColor(display.getThemeColors().secondary);
          char retryStr[32];
          sprintf(retryStr, "Attempt %d", connectionRetries);
          display.drawCenteredText(retryStr, 130, 1);
        }
        
        // Wait a bit before next attempt
        delay(1000);
        return;  // Skip this update cycle
      } else {
        // Connection successful
        if (connectionRetries > 0) {
          Serial.printf("[SUCCESS] Connected after %d retries!\n", connectionRetries);
          connectionRetries = 0;
        }
      }
      
      // Progress persistence: if we're printing and get 0%, keep last known value
      if (status.state == STATE_PRINTING && status.printProgress == 0 && lastValidProgress > 0) {
        status.printProgress = lastValidProgress;
        Serial.printf("[PERSIST] Using last known progress: %d%%\n", lastValidProgress);
      } else if (status.printProgress > 0) {
        lastValidProgress = status.printProgress;
      }
      
      // Reset progress when not printing
      if (status.state == STATE_IDLE || status.state == STATE_STANDBY || status.state == STATE_COMPLETE) {
        lastValidProgress = 0;
      }
      
      // Add environmental data
      EnvironmentalData envData = envSensor.readData();
      if (envData.valid) {
        status.chamberTemp = envData.temperature;
        status.chamberHumidity = envData.humidity;
        status.chamberPressure = envData.pressure;
      }
    } else {
      // WiFi not connected - show error
      Serial.println("[ERROR] WiFi not connected!");
      display.clear();
      display.setTextColor(display.getThemeColors().error);
      display.drawCenteredText("WiFi Error", 100, 2);
      display.setTextColor(display.getThemeColors().secondary);
      display.drawCenteredText("Reconnecting...", 130, 1);
      return;  // Skip this update cycle
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
