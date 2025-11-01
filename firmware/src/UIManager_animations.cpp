/*
 * UI Manager - Animation Screen Implementations
 * 
 * These functions draw animated versions of screens
 */

#include "UIManager.h"

// Idle animation - Rolling eyes with minimal data overlay
void UIManager::drawIdleAnimation(PrinterStatus& status) {
  // Draw rolling eyes animation
  updateRollingEyes();
  
  // Overlay minimal temperature data at bottom
  display->setTextColor(display->getThemeColors().secondary);
  char tempStr[32];
  sprintf(tempStr, "E:%.0f° B:%.0f°", status.hotendTemp, status.bedTemp);
  display->drawCenteredText(tempStr, 220, 1);
}

// Printing animation - Show progress with animation
void UIManager::drawPrintingAnimation(PrinterStatus& status) {
  // Clear for animation
  display->clear();
  
  // Draw animated progress ring (pulsing)
  unsigned long currentTime = millis();
  float pulse = sin((currentTime % 2000) / 2000.0 * 2 * PI) * 0.2 + 0.8;
  
  // Draw pulsing progress circle
  int16_t centerX = SCREEN_WIDTH / 2;
  int16_t centerY = SCREEN_HEIGHT / 2;
  int16_t radius = 80;
  
  // Background circle
  display->drawCircle(centerX, centerY, radius, display->getThemeColors().secondary);
  
  // Progress arc with pulse effect
  int16_t pulseRadius = radius * pulse;
  float progressAngle = (status.printProgress / 100.0) * 360.0;
  
  for (int angle = 0; angle < progressAngle; angle += 2) {
    float rad = (angle - 90) * PI / 180.0;
    int16_t x1 = centerX + cos(rad) * (pulseRadius - 5);
    int16_t y1 = centerY + sin(rad) * (pulseRadius - 5);
    int16_t x2 = centerX + cos(rad) * (pulseRadius + 5);
    int16_t y2 = centerY + sin(rad) * (pulseRadius + 5);
    display->drawLine(x1, y1, x2, y2, display->getThemeColors().accent);
  }
  
  // Draw progress percentage in center
  char progressStr[8];
  sprintf(progressStr, "%d%%", status.printProgress);
  display->setTextColor(display->getThemeColors().text);
  display->drawCenteredText(progressStr, centerY, 3);
  
  // Draw rotating "PRINTING" text
  int rotation = (currentTime / 100) % 8;
  const char* printStates[] = {
    "PRINTING.",
    "PRINTING..",
    "PRINTING...",
    "PRINTING",
    "PRINTING.",
    "PRINTING..",
    "PRINTING...",
    "PRINTING"
  };
  display->setTextColor(display->getThemeColors().accent);
  display->drawCenteredText(printStates[rotation], 30, 1);
  
  // Draw temps at bottom
  display->setTextColor(display->getThemeColors().secondary);
  char tempStr[32];
  sprintf(tempStr, "E:%.0f° B:%.0f°", status.hotendTemp, status.bedTemp);
  display->drawCenteredText(tempStr, 220, 1);
}
