#include <Arduino.h>
#include <WiFi.h>
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

  // Show boot screen
  ui.showBootScreen();
  delay(2000);

  // Initialize WiFi
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

  delay(2000);
}

void loop() {
  // Check for theme button press (debounced)
  if (digitalRead(THEME_BUTTON_PIN) == LOW) {
    unsigned long currentTime = millis();
    if (currentTime - lastButtonPress > DEBOUNCE_DELAY) {
      // Cycle to next theme
      display.nextTheme();
      Serial.printf("Switched to theme: %s\n", display.getThemeName());

      // Force UI redraw with new theme
      PrinterStatus currentStatus = api.getPrinterStatus();
      ui.updateStatus(currentStatus);

      lastButtonPress = currentTime;
    }
  }

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
