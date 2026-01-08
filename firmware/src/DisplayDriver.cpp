/*
 * Display Driver Implementation
 */

#include "DisplayDriver.h"
#include <math.h>

DisplayDriver::DisplayDriver() : currentBrightness(255) {
}

void DisplayDriver::init() {
  // Configure PWM for backlight control on GPIO3
  // ledcSetup(channel, freq, resolution_bits)
  ledcSetup(0, 5000, 8);  // Channel 0, 5kHz, 8-bit (0-255)
  ledcAttachPin(3, 0);    // Attach GPIO3 to channel 0
  
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(COLOR_BLACK);
  setBrightness(200);  // This will now use PWM
  
  // Initialize theme system
  themeManager.init();
}

void DisplayDriver::setBrightness(uint8_t brightness) {
  currentBrightness = brightness;
  // Control backlight with PWM on GPIO3
  // ESP32 ledcWrite uses 0-255 range, same as our brightness
  ledcWrite(0, brightness);  // Channel 0, value 0-255
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
  if (!blinking) {
    // Draw eye white with subtle gradient
    fillCircle(x, y, size, COLOR_WHITE);
    
    // Add subtle shadow for depth
    for (int i = 0; i < 3; i++) {
      uint16_t shadowColor = color565(200 - i * 20, 200 - i * 20, 200 - i * 20);
      drawCircle(x, y + 1, size - i, shadowColor);
    }
    
    // Draw outer eye border with thickness
    drawCircle(x, y, size, COLOR_BLACK);
    drawCircle(x, y, size + 1, COLOR_BLACK);
    
    // Draw iris with color gradient
    int16_t irisSize = size / 2 + 2;
    uint16_t irisColor = getThemeColors().accent;
    fillCircle(x + pupilX, y + pupilY, irisSize, irisColor);
    
    // Add iris detail rings
    for (int i = 1; i < 3; i++) {
      uint16_t ringColor = dimColor(irisColor, 150 + i * 30);
      drawCircle(x + pupilX, y + pupilY, irisSize - i * 2, ringColor);
    }
    
    // Draw pupil
    int16_t pupilSize = size / 3;
    fillCircle(x + pupilX, y + pupilY, pupilSize, COLOR_BLACK);
    
    // Draw enhanced highlight with glow
    int16_t highlightX = x + pupilX - pupilSize/2;
    int16_t highlightY = y + pupilY - pupilSize/2;
    fillCircle(highlightX, highlightY, pupilSize/3 + 1, COLOR_WHITE);
    fillCircle(highlightX + 1, highlightY + 1, pupilSize/4, COLOR_WHITE);
    
    // Add secondary smaller highlight
    fillCircle(x + pupilX + pupilSize/3, y + pupilY + pupilSize/3, 2, color565(200, 200, 200));
  } else {
    // Draw closed eye with curved line and thickness
    for (int i = 0; i < 3; i++) {
      drawLine(x - size, y + i - 1, x + size, y + i - 1, COLOR_BLACK);
    }
    
    // Add subtle eyelash effect
    for (int lash = -size; lash <= size; lash += size/2) {
      drawLine(x + lash, y, x + lash, y - 4, COLOR_BLACK);
    }
  }
}

void DisplayDriver::drawRollingEyes(int16_t frame) {
  // Calculate pupil position with smooth easing (sinusoidal)
  float t = (frame % 360) / 360.0;
  float easedT = (sin(t * 2 * PI - PI/2) + 1) / 2;
  
  // Dynamic pupil offset with more natural movement
  int16_t pupilOffset = 7 + (int16_t)(5 * sin(t * 2 * PI));
  int16_t pupilX = pupilOffset * cos(easedT * 2 * PI);
  int16_t pupilY = pupilOffset * sin(easedT * 2 * PI);
  
  // More natural blinking pattern
  bool blinking = ((frame % 150) < 10) || ((frame % 230) < 6);
  
  // Enhanced eye parameters
  int16_t eyeSize = 40;
  int16_t eyeSpacing = 60;
  
  // Add ambient glow effect around eyes when not blinking
  if (!blinking) {
    uint8_t glowPulse = 40 + (uint8_t)(30 * sin(frame * 0.015));
    uint16_t glowColor = dimColor(getThemeColors().accent, glowPulse);
    
    // Glow around left eye
    for (int i = 3; i > 0; i--) {
      uint16_t layerColor = dimColor(glowColor, glowPulse / i);
      drawCircle(SCREEN_WIDTH/2 - eyeSpacing, SCREEN_HEIGHT/2, eyeSize + 5 + i * 2, layerColor);
    }
    
    // Glow around right eye
    for (int i = 3; i > 0; i--) {
      uint16_t layerColor = dimColor(glowColor, glowPulse / i);
      drawCircle(SCREEN_WIDTH/2 + eyeSpacing, SCREEN_HEIGHT/2, eyeSize + 5 + i * 2, layerColor);
    }
  }
  
  // Draw two eyes with enhanced details
  drawEye(SCREEN_WIDTH/2 - eyeSpacing, SCREEN_HEIGHT/2, eyeSize, pupilX, pupilY, blinking);
  drawEye(SCREEN_WIDTH/2 + eyeSpacing, SCREEN_HEIGHT/2, eyeSize, pupilX, pupilY, blinking);
  
  // Add subtle background breathing effect with NEON colors
  if (!blinking) {
    uint8_t breath = 30 + (uint8_t)(25 * sin(frame * 0.012));
    uint16_t breathColor = dimColor(getThemeColors().secondary, breath);
    
    for (int r = 112; r < 118; r++) {
      drawCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, r, breathColor);
    }
    
    // Add corner accent dots that pulse
    uint8_t cornerPulse = 100 + (uint8_t)(100 * sin(frame * 0.01));
    uint16_t cornerColor = dimColor(getThemeColors().highlight, cornerPulse);
    fillCircle(20, 20, 3, cornerColor);
    fillCircle(SCREEN_WIDTH - 20, 20, 3, cornerColor);
    fillCircle(20, SCREEN_HEIGHT - 20, 3, cornerColor);
    fillCircle(SCREEN_WIDTH - 20, SCREEN_HEIGHT - 20, 3, cornerColor);
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
  // Checkmark icon with thicker lines
  drawLine(x - 8, y, x - 3, y + 8, color);
  drawLine(x - 8, y + 1, x - 3, y + 9, color);
  drawLine(x - 3, y + 8, x + 10, y - 8, color);
  drawLine(x - 3, y + 9, x + 10, y - 7, color);
  drawCircle(x, y, 15, color);
  drawCircle(x, y, 16, color);
}

// Helper function to blend two colors
uint16_t DisplayDriver::blendColor(uint16_t color1, uint16_t color2, uint8_t alpha) {
  // Extract RGB components from color1
  uint8_t r1 = (color1 >> 11) & 0x1F;
  uint8_t g1 = (color1 >> 5) & 0x3F;
  uint8_t b1 = color1 & 0x1F;
  
  // Extract RGB components from color2
  uint8_t r2 = (color2 >> 11) & 0x1F;
  uint8_t g2 = (color2 >> 5) & 0x3F;
  uint8_t b2 = color2 & 0x1F;
  
  // Blend components
  uint8_t r = (r1 * alpha + r2 * (255 - alpha)) / 255;
  uint8_t g = (g1 * alpha + g2 * (255 - alpha)) / 255;
  uint8_t b = (b1 * alpha + b2 * (255 - alpha)) / 255;
  
  return (r << 11) | (g << 5) | b;
}

// Helper function to dim a color
uint16_t DisplayDriver::dimColor(uint16_t color, uint8_t amount) {
  uint8_t r = ((color >> 11) & 0x1F) * amount / 255;
  uint8_t g = ((color >> 5) & 0x3F) * amount / 255;
  uint8_t b = (color & 0x1F) * amount / 255;
  return (r << 11) | (g << 5) | b;
}

// NEON progress ring with glow effect
void DisplayDriver::drawProgressRingNeon(int16_t x, int16_t y, int16_t r, int16_t thickness, uint8_t progress, uint16_t color) {
  // Draw outer glow layers for NEON effect
  for (int glow = 3; glow > 0; glow--) {
    uint8_t glowAlpha = 80 / glow;
    uint16_t glowColor = dimColor(color, glowAlpha);
    int16_t glowR = r + glow * 2;
    int endAngle = -90 + (360 * progress / 100);
    drawArc(x, y, glowR, -90, endAngle, glowColor);
  }
  
  // Draw background ring
  for (int i = 0; i < thickness; i++) {
    drawCircle(x, y, r - i, dimColor(color, 30));
  }
  
  // Draw main progress arc with multiple layers
  int endAngle = -90 + (360 * progress / 100);
  for (int i = 0; i < thickness; i++) {
    int16_t arcR = r - i;
    drawArc(x, y, arcR, -90, endAngle, color);
  }
  
  // Add bright highlight on inner edge
  if (progress > 0) {
    drawArc(x, y, r - thickness + 2, -90, endAngle, COLOR_WHITE);
  }
}

// Draw text with NEON glow effect
void DisplayDriver::drawGlowText(const char* text, int16_t x, int16_t y, uint8_t size, uint16_t color) {
  setTextSize(size);
  
  // Draw glow layers
  for (int offset = 2; offset > 0; offset--) {
    uint16_t glowColor = dimColor(color, 60 / offset);
    setTextColor(glowColor);
    
    // Draw in 8 directions for glow
    setCursor(x - offset, y); print(text);
    setCursor(x + offset, y); print(text);
    setCursor(x, y - offset); print(text);
    setCursor(x, y + offset); print(text);
  }
  
  // Draw main text
  setTextColor(color);
  setCursor(x, y);
  print(text);
}

// Draw circle with NEON glow
void DisplayDriver::drawGlowCircle(int16_t x, int16_t y, int16_t r, uint16_t color, uint8_t intensity) {
  // Draw outer glow layers
  for (int i = intensity; i > 0; i--) {
    uint8_t alpha = (255 * i) / intensity / 3;
    uint16_t glowColor = dimColor(color, alpha);
    drawCircle(x, y, r + i, glowColor);
  }
  
  // Draw main circle
  fillCircle(x, y, r, color);
  
  // Add bright center highlight
  fillCircle(x, y, r / 3, COLOR_WHITE);
}

// Draw NEON line with thickness and glow
void DisplayDriver::drawNeonLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color, uint8_t thickness) {
  // Draw glow layers
  for (int glow = thickness + 2; glow > thickness; glow--) {
    uint16_t glowColor = dimColor(color, 50);
    for (int t = 0; t < glow; t++) {
      drawLine(x0, y0 + t - glow/2, x1, y1 + t - glow/2, glowColor);
    }
  }
  
  // Draw main line with thickness
  for (int t = 0; t < thickness; t++) {
    drawLine(x0, y0 + t - thickness/2, x1, y1 + t - thickness/2, color);
  }
}

// Enhanced printer icon with NEON glow
void DisplayDriver::drawPrinterIconNeon(int16_t x, int16_t y, uint16_t color) {
  // Draw glow
  uint16_t glowColor = dimColor(color, 80);
  fillRect(x - 1, y - 1, 32, 22, glowColor);
  fillRect(x + 4, y - 11, 22, 12, glowColor);
  
  // Draw main icon with better detail
  fillRect(x, y, 30, 20, color);
  fillRect(x + 5, y - 10, 20, 10, color);
  fillRect(x + 10, y + 20, 10, 5, color);
  
  // Add details
  fillRect(x + 3, y + 3, 24, 2, COLOR_WHITE);
  fillRect(x + 3, y + 8, 24, 2, COLOR_WHITE);
  fillRect(x + 3, y + 13, 24, 2, COLOR_WHITE);
}

// Enhanced temperature icon with NEON glow
void DisplayDriver::drawTemperatureIconNeon(int16_t x, int16_t y, uint16_t color) {
  // Draw glow
  uint16_t glowColor = dimColor(color, 80);
  drawCircle(x, y + 15, 7, glowColor);
  fillCircle(x, y + 15, 6, glowColor);
  
  // Draw main thermometer
  drawCircle(x, y + 15, 6, color);
  fillCircle(x, y + 15, 5, color);
  fillRect(x - 3, y, 6, 16, color);
  
  // Add mercury level
  fillRect(x - 2, y + 5, 4, 11, COLOR_RED);
  
  // Add tick marks
  drawLine(x + 4, y + 3, x + 8, y + 3, color);
  drawLine(x + 4, y + 7, x + 8, y + 7, color);
  drawLine(x + 4, y + 11, x + 8, y + 11, color);
}
