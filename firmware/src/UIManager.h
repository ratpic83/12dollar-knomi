/*
 * UI Manager
 * 
 * Manages different display screens and animations
 */

#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <Arduino.h>
#include "DisplayDriver.h"
#include "KlipperAPI.h"

// Screen types
enum ScreenType {
  SCREEN_BOOT,
  SCREEN_CONNECTING,
  SCREEN_CONNECTED,
  SCREEN_IDLE,
  SCREEN_PRINTING,
  SCREEN_PAUSED,
  SCREEN_COMPLETE,
  SCREEN_ERROR,
  SCREEN_WIFI_ERROR,
  SCREEN_KLIPPER_ERROR
};

class UIManager {
public:
  UIManager();
  
  // Initialization
  void init(DisplayDriver* display);
  
  // Screen management
  void showBootScreen();
  void showConnectingScreen();
  void showConnectedScreen();
  void showWiFiError();
  void showKlipperError();
  
  // Update with printer status
  void updateStatus(PrinterStatus& status);
  
  // Animation update (call in loop)
  void update();
  
private:
  DisplayDriver* display;
  ScreenType currentScreen;
  unsigned long lastAnimationUpdate;
  int animationFrame;
  PrinterStatus lastStatus;
  
  // Screen drawing functions
  void drawIdleScreen(PrinterStatus& status);
  void drawPrintingScreen(PrinterStatus& status);
  void drawPausedScreen(PrinterStatus& status);
  void drawCompleteScreen(PrinterStatus& status);
  void drawErrorScreen();
  
  // UI elements
  void drawStatusBar(PrinterStatus& status);
  void drawTemperatureDisplay(float temp, float target, const char* label, int16_t y);
  void drawProgressCircle(uint8_t progress);
  void drawPrintInfo(PrinterStatus& status);
  void drawTemperatureGauges(PrinterStatus& status);
  
  // Animations
  void updateRollingEyes();
  void updatePrintingAnimation();
  
  // Helper functions
  String formatTime(uint32_t seconds);
  String formatTemperature(float temp, float target);
  bool shouldRedraw(PrinterStatus& status);
};

#endif // UI_MANAGER_H
