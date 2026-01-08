/*
 * Display Driver for GC9A01 Round LCD
 * 
 * Handles low-level display operations for 240x240 round LCD
 */

#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "Theme.h"

// Display dimensions
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 240
#define SCREEN_RADIUS 120

// Legacy color defines for backward compatibility (now use theme colors)
#define COLOR_BLACK       0x0000
#define COLOR_WHITE       0xFFFF
#define COLOR_RED         0xF800
#define COLOR_GREEN       0x07E0
#define COLOR_BLUE        0x001F
#define COLOR_YELLOW      0xFFE0
#define COLOR_ORANGE      0xFD20
#define COLOR_CYAN        0x07FF
#define COLOR_MAGENTA     0xF81F
#define COLOR_GRAY        0x8410
#define COLOR_DARK_GRAY   0x4208
#define COLOR_LIGHT_GRAY  0xC618

// Theme-based color access (preferred)

class DisplayDriver {
public:
  DisplayDriver();
  
  // Initialization
  void init();
  void setBrightness(uint8_t brightness);
  uint8_t getBrightness() const { return currentBrightness; }
  
  // Theme management
  void setTheme(ThemeType theme) { themeManager.setTheme(theme); }
  ThemeType getCurrentTheme() const { return themeManager.getCurrentTheme(); }
  const ThemeColors& getThemeColors() const { return themeManager.getColors(); }
  const char* getThemeName() const { return themeManager.getThemeName(); }
  void nextTheme() { themeManager.nextTheme(); }
  void previousTheme() { themeManager.previousTheme(); }
  
  // Basic drawing
  void clear();
  void fillScreen(uint16_t color);
  void drawPixel(int16_t x, int16_t y, uint16_t color);
  void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
  void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  void drawCircle(int16_t x, int16_t y, int16_t r, uint16_t color);
  void fillCircle(int16_t x, int16_t y, int16_t r, uint16_t color);
  void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
  void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
  
  // Text drawing
  void setTextColor(uint16_t color);
  void setTextColor(uint16_t color, uint16_t bg);
  void setTextSize(uint8_t size);
  void setCursor(int16_t x, int16_t y);
  void print(const char* text);
  void print(String text);
  void println(const char* text);
  void println(String text);
  void drawCenteredText(const char* text, int16_t y, uint8_t size);
  void drawCenteredText(String text, int16_t y, uint8_t size);
  
  // Advanced drawing
  void drawArc(int16_t x, int16_t y, int16_t r, int16_t startAngle, int16_t endAngle, uint16_t color);
  void drawProgressRing(int16_t x, int16_t y, int16_t r, int16_t thickness, uint8_t progress, uint16_t color);
  void drawProgressRingNeon(int16_t x, int16_t y, int16_t r, int16_t thickness, uint8_t progress, uint16_t color);
  void drawTemperatureGauge(int16_t x, int16_t y, int16_t r, float temp, float target, uint16_t color);
  
  // NEON effects
  void drawGlowText(const char* text, int16_t x, int16_t y, uint8_t size, uint16_t color);
  void drawGlowCircle(int16_t x, int16_t y, int16_t r, uint16_t color, uint8_t intensity);
  void drawNeonLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color, uint8_t thickness);
  
  // Eye animations
  void drawEye(int16_t x, int16_t y, int16_t size, int16_t pupilX, int16_t pupilY, bool blinking);
  void drawRollingEyes(int16_t frame);
  
  // Icons and symbols (enhanced with better resolution)
  void drawPrinterIcon(int16_t x, int16_t y, uint16_t color);
  void drawTemperatureIcon(int16_t x, int16_t y, uint16_t color);
  void drawWiFiIcon(int16_t x, int16_t y, uint16_t color, int8_t strength);
  void drawErrorIcon(int16_t x, int16_t y, uint16_t color);
  void drawCheckIcon(int16_t x, int16_t y, uint16_t color);
  void drawTouchFeedbackRing(uint8_t alpha);
  
  // Enhanced icons with NEON glow
  void drawPrinterIconNeon(int16_t x, int16_t y, uint16_t color);
  void drawTemperatureIconNeon(int16_t x, int16_t y, uint16_t color);
  
  // Get display object for advanced operations
  TFT_eSPI* getTFT() { return &tft; }
  
  // Color conversion
  uint16_t color565(uint8_t r, uint8_t g, uint8_t b) { return tft.color565(r, g, b); }
  
  // Helper functions
  int16_t getTextWidth(const char* text, uint8_t size);
  int16_t getTextWidth(String text, uint8_t size);
  uint16_t blendColor(uint16_t color1, uint16_t color2, uint8_t alpha);
  uint16_t dimColor(uint16_t color, uint8_t amount);
  
private:
  TFT_eSPI tft;
  uint8_t currentBrightness;
  ThemeManager themeManager;
};

#endif // DISPLAY_DRIVER_H
