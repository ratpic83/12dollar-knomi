/*
 * Display Driver Implementation
 */

#include "DisplayDriver.h"
#include <math.h>

DisplayDriver::DisplayDriver() : currentBrightness(255) {
}

void DisplayDriver::init() {
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(COLOR_BLACK);
  setBrightness(200);
  
  // Initialize theme system
  themeManager.init();
}

void DisplayDriver::setBrightness(uint8_t brightness) {
  currentBrightness = brightness;
  // Note: Brightness control depends on hardware
  // Some displays use PWM on backlight pin
  // This is a placeholder - adjust for your hardware
}

void DisplayDriver::clear() {
  tft.fillScreen(getThemeColors().bg);
}

void DisplayDriver::fillScreen(uint16_t color) {
  tft.fillScreen(color);
}

void DisplayDriver::drawPixel(int16_t x, int16_t y, uint16_t color) {
  tft.drawPixel(x, y, color);
}

void DisplayDriver::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
  tft.drawLine(x0, y0, x1, y1, color);
}

void DisplayDriver::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  tft.drawRect(x, y, w, h, color);
}

void DisplayDriver::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  tft.fillRect(x, y, w, h, color);
}

void DisplayDriver::drawCircle(int16_t x, int16_t y, int16_t r, uint16_t color) {
  tft.drawCircle(x, y, r, color);
}

void DisplayDriver::drawTouchFeedbackRing(uint8_t alpha) {
  // Draw a fading ring around the screen perimeter
  uint16_t color = getThemeColors().highlight; // Use theme highlight color
  
  // Draw outer ring
  tft.drawCircle(120, 120, 118, tft.alphaBlend(alpha, color, TFT_BLACK));
  tft.drawCircle(120, 120, 117, tft.alphaBlend(alpha, color, TFT_BLACK));
  
  // Draw inner ring
  tft.drawCircle(120, 120, 115, tft.alphaBlend(alpha, color, TFT_BLACK));
  tft.drawCircle(120, 120, 116, tft.alphaBlend(alpha, color, TFT_BLACK));
}

void DisplayDriver::fillCircle(int16_t x, int16_t y, int16_t r, uint16_t color) {
  tft.fillCircle(x, y, r, color);
}

void DisplayDriver::drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color) {
  tft.drawRoundRect(x, y, w, h, r, color);
}

void DisplayDriver::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color) {
  tft.fillRoundRect(x, y, w, h, r, color);
}

void DisplayDriver::setTextColor(uint16_t color) {
  tft.setTextColor(color);
}

void DisplayDriver::setTextColor(uint16_t color, uint16_t bg) {
  tft.setTextColor(color, bg);
}

void DisplayDriver::setTextSize(uint8_t size) {
  tft.setTextSize(size);
}

void DisplayDriver::setCursor(int16_t x, int16_t y) {
  tft.setCursor(x, y);
}

void DisplayDriver::print(const char* text) {
  tft.print(text);
}

void DisplayDriver::print(String text) {
  tft.print(text);
}

void DisplayDriver::println(const char* text) {
  tft.println(text);
}

void DisplayDriver::println(String text) {
  tft.println(text);
}

int16_t DisplayDriver::getTextWidth(const char* text, uint8_t size) {
  tft.setTextSize(size);
  return tft.textWidth(text);
}

int16_t DisplayDriver::getTextWidth(String text, uint8_t size) {
  return getTextWidth(text.c_str(), size);
}

void DisplayDriver::drawCenteredText(const char* text, int16_t y, uint8_t size) {
  setTextSize(size);
  int16_t w = getTextWidth(text, size);
  int16_t x = (SCREEN_WIDTH - w) / 2;
  setCursor(x, y);
  print(text);
}

void DisplayDriver::drawCenteredText(String text, int16_t y, uint8_t size) {
  drawCenteredText(text.c_str(), y, size);
}

void DisplayDriver::drawArc(int16_t x, int16_t y, int16_t r, int16_t startAngle, int16_t endAngle, uint16_t color) {
  // Draw arc using line segments
  for (int angle = startAngle; angle < endAngle; angle += 2) {
    float rad1 = angle * PI / 180.0;
    float rad2 = (angle + 2) * PI / 180.0;
    
    int16_t x1 = x + r * cos(rad1);
    int16_t y1 = y + r * sin(rad1);
    int16_t x2 = x + r * cos(rad2);
    int16_t y2 = y + r * sin(rad2);
    
    drawLine(x1, y1, x2, y2, color);
  }
}

void DisplayDriver::drawProgressRing(int16_t x, int16_t y, int16_t r, int16_t thickness, uint8_t progress, uint16_t color) {
  // Draw background ring with anti-aliasing
  for (int i = 0; i < thickness; i++) {
    int16_t innerR = r - i;
    int16_t outerR = r - i + 1;
    
    // Draw full background circle with subtle gradient
    fillCircle(x, y, innerR, COLOR_DARK_GRAY);
    
    // Add subtle inner glow for depth
    if (i == 0) {
      drawCircle(x, y, innerR + 1, COLOR_LIGHT_GRAY);
    }
  }
  
  // Calculate progress arc with anti-aliasing
  int endAngle = -90 + (360 * progress / 100);
  
  // Draw progress arc with multiple thickness layers for anti-aliasing effect
  for (int i = 0; i < thickness; i++) {
    int16_t arcR = r - i;
    drawArc(x, y, arcR, -90, endAngle, color);
    
    // Add inner highlight for 3D effect
    if (i == 0 && progress > 5) {
      int16_t highlightR = r - thickness/2;
      drawArc(x, y, highlightR, -90, endAngle - 10, COLOR_WHITE);
    }
  }
  
  // Draw center dot if progress > 0
  if (progress > 0) {
    fillCircle(x, y, 3, color);
    fillCircle(x, y, 1, COLOR_WHITE);
  }
}

void DisplayDriver::drawTemperatureGauge(int16_t x, int16_t y, int16_t r, float temp, float target, uint16_t color) {
  // Draw gauge background
  drawCircle(x, y, r, COLOR_GRAY);
  
  // Calculate angle based on temperature (0-300°C range)
  float maxTemp = 300.0;
  float tempAngle = -135 + (270 * temp / maxTemp);
  float targetAngle = -135 + (270 * target / maxTemp);
  
  // Draw target marker
  if (target > 0) {
    float rad = targetAngle * PI / 180.0;
    int16_t x1 = x + (r - 10) * cos(rad);
    int16_t y1 = y + (r - 10) * sin(rad);
    int16_t x2 = x + r * cos(rad);
    int16_t y2 = y + r * sin(rad);
    drawLine(x1, y1, x2, y2, COLOR_YELLOW);
  }
  
  // Draw temperature needle
  float rad = tempAngle * PI / 180.0;
  int16_t x1 = x;
  int16_t y1 = y;
  int16_t x2 = x + (r - 5) * cos(rad);
  int16_t y2 = y + (r - 5) * sin(rad);
  drawLine(x1, y1, x2, y2, color);
  fillCircle(x, y, 3, color);
}

void DisplayDriver::drawEye(int16_t x, int16_t y, int16_t size, int16_t pupilX, int16_t pupilY, bool blinking) {
  // Draw eye white
  fillCircle(x, y, size, COLOR_WHITE);
  drawCircle(x, y, size, COLOR_BLACK);
  
  if (!blinking) {
    // Draw pupil
    int16_t pupilSize = size / 2;
    fillCircle(x + pupilX, y + pupilY, pupilSize, COLOR_BLACK);
    
    // Draw highlight
    fillCircle(x + pupilX - pupilSize/3, y + pupilY - pupilSize/3, pupilSize/4, COLOR_WHITE);
  } else {
    // Draw closed eye (horizontal line)
    drawLine(x - size, y, x + size, y, COLOR_BLACK);
  }
}

void DisplayDriver::drawRollingEyes(int16_t frame) {
  clear();
  
  // Calculate pupil position with smooth easing (sinusoidal)
  float t = (frame % 360) / 360.0; // 0 to 1 over 360 frames
  float easedT = (sin(t * 2 * PI - PI/2) + 1) / 2; // Smooth sine wave
  
  int16_t pupilOffset = 6 + (int16_t)(4 * sin(t * 2 * PI)); // Dynamic range 6-10
  int16_t pupilX = pupilOffset * cos(easedT * 2 * PI);
  int16_t pupilY = pupilOffset * sin(easedT * 2 * PI);
  
  // Blink with varying duration
  bool blinking = ((frame % 120) < 8) || ((frame % 180) < 5); // Occasional blinks
  
  // Draw two eyes with enhanced details
  int16_t eyeSize = 35;
  int16_t eyeSpacing = 55;
  
  drawEye(SCREEN_WIDTH/2 - eyeSpacing, SCREEN_HEIGHT/2, eyeSize, pupilX, pupilY, blinking);
  drawEye(SCREEN_WIDTH/2 + eyeSpacing, SCREEN_HEIGHT/2, eyeSize, pupilX, pupilY, blinking);
  
  // Add subtle background breathing effect
  if (!blinking) {
    uint8_t breath = 50 + (uint8_t)(30 * sin(frame * 0.02)); // Slow breathing
    for (int r = 110; r < 115; r++) {
      drawCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, r, tft.color565(breath, breath, breath));
    }
  }
}

void DisplayDriver::drawPrinterIcon(int16_t x, int16_t y, uint16_t color) {
  // Simple printer icon
  fillRect(x, y, 30, 20, color);
  fillRect(x + 5, y - 10, 20, 10, color);
  fillRect(x + 10, y + 20, 10, 5, color);
}

void DisplayDriver::drawTemperatureIcon(int16_t x, int16_t y, uint16_t color) {
  // Thermometer icon
  drawCircle(x, y + 15, 5, color);
  fillCircle(x, y + 15, 3, color);
  drawRect(x - 2, y, 4, 15, color);
  drawLine(x + 4, y + 3, x + 7, y + 3, color);
  drawLine(x + 4, y + 7, x + 7, y + 7, color);
  drawLine(x + 4, y + 11, x + 7, y + 11, color);
}

void DisplayDriver::drawWiFiIcon(int16_t x, int16_t y, uint16_t color, int8_t strength) {
  // WiFi signal icon with strength bars
  drawArc(x, y, 15, -60, 60, color);
  if (strength > 1) drawArc(x, y, 10, -50, 50, color);
  if (strength > 2) drawArc(x, y, 5, -40, 40, color);
  fillCircle(x, y, 2, color);
}

void DisplayDriver::drawErrorIcon(int16_t x, int16_t y, uint16_t color) {
  // X icon for errors
  drawLine(x - 10, y - 10, x + 10, y + 10, color);
  drawLine(x + 10, y - 10, x - 10, y + 10, color);
  drawCircle(x, y, 15, color);
}

void DisplayDriver::drawCheckIcon(int16_t x, int16_t y, uint16_t color) {
  // Checkmark icon
  drawLine(x - 8, y, x - 3, y + 8, color);
  drawLine(x - 3, y + 8, x + 10, y - 8, color);
  drawCircle(x, y, 15, color);
}
