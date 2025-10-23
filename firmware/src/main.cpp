#include <Arduino.h>
#include "DisplayDriver.h"
#include "UIManager.h"
#include "KlipperAPI.h"
#include "WifiConfig.h"

// Global instances
DisplayDriver display;
UIManager ui;
KlipperAPI api;

// Theme cycling button (GPIO 9 - can connect a button here)
const int THEME_BUTTON_PIN = 9;
unsigned long lastButtonPress = 0;
const unsigned long DEBOUNCE_DELAY = 500; // 500ms debounce

void setup() {
  Serial.begin(115200);
  Serial.println("Knomi Clone v1.1 - Starting...");

  // Initialize theme button pin
  pinMode(THEME_BUTTON_PIN, INPUT_PULLUP);

  // Initialize display
  display.init();
  ui.init(&display);

  // Check if device is configured
  // webConfig.begin(); // Temporarily disabled for testing
  
  if (true) { // webConfig.isConfigured()) {
    // Device is configured, proceed with normal operation
    Serial.println("Device configured, starting normal operation...");
    
    // Show boot screen
    ui.showBootScreen();
    delay(2000);

    // Initialize WiFi with configured settings
    ui.showConnectingScreen();
    api.init(KLIPPER_IP, KLIPPER_PORT);

    // Wait for connection
    int attempts = 0;
    while (!api.testConnection() && attempts < 30) {
      delay(500);
      attempts++;
    }

    if (api.testConnection()) {
      ui.showConnectedScreen();
      Serial.println("Connected to Klipper!");
    } else {
      ui.showWiFiError();
      Serial.println("Failed to connect to Klipper");
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
  if (millis() - lastUpdate > 2000) {
    lastUpdate = millis();

    PrinterStatus status = api.getPrinterStatus();
    ui.updateStatus(status);

    // Debug output
    Serial.printf("State: %d, Hotend: %.1f/%.1f, Bed: %.1f/%.1f\n",
                  status.state, status.hotendTemp, status.hotendTarget,
                  status.bedTemp, status.bedTarget);
  }

  // Update UI animations
  ui.update();

  delay(10);
}
