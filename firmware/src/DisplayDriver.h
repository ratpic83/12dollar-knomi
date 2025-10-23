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

// Display dimensions
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 240
#define SCREEN_RADIUS 120

// Colors (RGB565 format)
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

// Custom colors for UI
#define COLOR_BG          0x0000      // Black background
#define COLOR_TEXT        0xFFFF      // White text
#define COLOR_ACCENT      0x07FF      // Cyan accent
#define COLOR_WARNING     0xFD20      // Orange warning
#define COLOR_ERROR       0xF800      // Red error
#define COLOR_SUCCESS     0x07E0      // Green success

class DisplayDriver {
public:
  DisplayDriver();
  
  // Initialization
  void init();
  void setBrightness(uint8_t brightness);
  
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
  void drawTemperatureGauge(int16_t x, int16_t y, int16_t r, float temp, float target, uint16_t color);
  
  // Eye animations
  void drawEye(int16_t x, int16_t y, int16_t size, int16_t pupilX, int16_t pupilY, bool blinking);
  void drawRollingEyes(int16_t frame);
  
  // Icons and symbols
  void drawPrinterIcon(int16_t x, int16_t y, uint16_t color);
  void drawTemperatureIcon(int16_t x, int16_t y, uint16_t color);
  void drawWiFiIcon(int16_t x, int16_t y, uint16_t color, int8_t strength);
  void drawErrorIcon(int16_t x, int16_t y, uint16_t color);
  void drawCheckIcon(int16_t x, int16_t y, uint16_t color);
  
  // Get display object for advanced operations
  TFT_eSPI* getTFT() { return &tft; }
  
private:
  TFT_eSPI tft;
  uint8_t currentBrightness;
  
  // Helper functions
  int16_t getTextWidth(const char* text, uint8_t size);
  int16_t getTextWidth(String text, uint8_t size);
};

#endif // DISPLAY_DRIVER_H
