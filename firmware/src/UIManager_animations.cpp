/*
 * UI Manager - Animation Screen Implementations
 * 
 * Enhanced animations with NEON effects, smooth transitions,
 * particle systems, and sophisticated visual effects
 */

#include "UIManager.h"

// Idle animation - Rolling eyes with enhanced NEON overlay
void UIManager::drawIdleAnimation(PrinterStatus& status) {
  // Draw rolling eyes animation
  updateRollingEyes();
  
  // Add pulsing ambient glow around screen edge
  unsigned long currentTime = millis();
  uint8_t glowIntensity = 30 + (uint8_t)(20 * sin(currentTime * 0.002));
  
  // Draw subtle pulsing ring
  for (int i = 0; i < 3; i++) {
    uint16_t glowColor = display->dimColor(display->getThemeColors().accent, glowIntensity - i * 10);
    display->drawCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 115 - i, glowColor);
  }
  
  // Overlay temperature data with NEON glow effect
  char tempStr[32];
  sprintf(tempStr, "E:%.0f° B:%.0f°", status.hotendTemp, status.bedTemp);
  
  // Draw glow behind text
  int16_t textWidth = display->getTextWidth(tempStr, 1);
  int16_t textX = (SCREEN_WIDTH - textWidth) / 2;
  for (int offset = 2; offset > 0; offset--) {
    uint16_t glowColor = display->dimColor(display->getThemeColors().secondary, 40 / offset);
    display->setTextColor(glowColor);
    display->setCursor(textX - offset, 220); display->print(tempStr);
    display->setCursor(textX + offset, 220); display->print(tempStr);
  }
  
  // Draw main text
  display->setTextColor(display->getThemeColors().secondary);
  display->drawCenteredText(tempStr, 220, 1);
}

// Printing animation - Enhanced with NEON effects and particle system
void UIManager::drawPrintingAnimation(PrinterStatus& status) {
  // Clear for animation
  display->clear();
  
  unsigned long currentTime = millis();
  int16_t centerX = SCREEN_WIDTH / 2;
  int16_t centerY = SCREEN_HEIGHT / 2;
  int16_t radius = 80;
  
  // Draw NEON progress ring with glow
  display->drawProgressRingNeon(centerX, centerY, radius, 10, status.printProgress, display->getThemeColors().accent);
  
  // Add rotating particles around the ring
  float particleSpeed = currentTime * 0.003;
  for (int i = 0; i < 6; i++) {
    float angle = (particleSpeed + i * 60) * PI / 180.0;
    int16_t px = centerX + cos(angle) * (radius + 15);
    int16_t py = centerY + sin(angle) * (radius + 15);
    
    // Particle with glow
    uint8_t particleGlow = 3 + (i % 2);
    display->drawGlowCircle(px, py, 2, display->getThemeColors().highlight, particleGlow);
  }
  
  // Draw progress percentage with enhanced glow
  char progressStr[8];
  sprintf(progressStr, "%d%%", status.printProgress);
  
  // Multi-layer glow for text
  int16_t textWidth = display->getTextWidth(progressStr, 3);
  int16_t textX = (SCREEN_WIDTH - textWidth) / 2;
  int16_t textY = centerY - 12;
  
  for (int offset = 3; offset > 0; offset--) {
    uint16_t glowColor = display->dimColor(display->getThemeColors().text, 50 / offset);
    display->setTextColor(glowColor);
    display->setTextSize(3);
    display->setCursor(textX - offset, textY); display->print(progressStr);
    display->setCursor(textX + offset, textY); display->print(progressStr);
    display->setCursor(textX, textY - offset); display->print(progressStr);
    display->setCursor(textX, textY + offset); display->print(progressStr);
  }
  
  display->setTextColor(display->getThemeColors().text);
  display->drawCenteredText(progressStr, centerY, 3);
  
  // Draw animated "PRINTING" text with wave effect
  int rotation = (currentTime / 150) % 4;
  const char* printStates[] = {
    "PRINTING.",
    "PRINTING..",
    "PRINTING...",
    "PRINTING"
  };
  
  // Add wave animation to text position
  int16_t waveOffset = (int16_t)(3 * sin(currentTime * 0.005));
  
  // Draw with glow
  const char* currentText = printStates[rotation];
  int16_t printTextWidth = display->getTextWidth(currentText, 2);
  int16_t printTextX = (SCREEN_WIDTH - printTextWidth) / 2;
  
  for (int offset = 2; offset > 0; offset--) {
    uint16_t glowColor = display->dimColor(display->getThemeColors().accent, 60 / offset);
    display->setTextColor(glowColor);
    display->setTextSize(2);
    display->setCursor(printTextX - offset, 25 + waveOffset); display->print(currentText);
    display->setCursor(printTextX + offset, 25 + waveOffset); display->print(currentText);
  }
  
  display->setTextColor(display->getThemeColors().accent);
  display->drawCenteredText(currentText, 25 + waveOffset, 2);
  
  // Draw temps at bottom with subtle pulse
  uint8_t tempPulse = 200 + (uint8_t)(55 * sin(currentTime * 0.003));
  uint16_t tempColor = display->dimColor(display->getThemeColors().secondary, tempPulse);
  display->setTextColor(tempColor);
  char tempStr[32];
  sprintf(tempStr, "E:%.0f° B:%.0f°", status.hotendTemp, status.bedTemp);
  display->drawCenteredText(tempStr, 215, 1);
  
  // Add corner indicators for activity
  uint8_t cornerBrightness = 100 + (uint8_t)(100 * sin(currentTime * 0.004));
  uint16_t cornerColor = display->dimColor(display->getThemeColors().highlight, cornerBrightness);
  display->fillCircle(15, 15, 3, cornerColor);
  display->fillCircle(SCREEN_WIDTH - 15, 15, 3, cornerColor);
}
