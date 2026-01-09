/*
 * Theme System for Knomi Clone
 *
 * Provides different color schemes and theme management
 */

#ifndef THEME_H
#define THEME_H

#include <Arduino.h>

// Theme types
enum ThemeType {
  THEME_DARK,         // Classic dark theme
  THEME_LIGHT,        // Light theme
  THEME_NEON,         // Neon/cyberpunk theme (cyan/magenta)
  THEME_NEON_PINK,    // Hot pink neon theme
  THEME_NEON_BLUE,    // Electric blue neon theme
  THEME_NEON_GREEN,   // Toxic green neon theme
  THEME_NEON_PURPLE,  // Purple/violet neon theme
  THEME_MINIMAL,      // Minimalist theme
  THEME_COUNT         // Number of themes
};

// Color palette structure
struct ThemeColors {
  uint16_t bg;           // Background
  uint16_t text;         // Primary text
  uint16_t accent;       // Accent/progress color
  uint16_t warning;      // Warning color
  uint16_t error;        // Error color
  uint16_t success;      // Success color
  uint16_t secondary;    // Secondary text/icons
  uint16_t highlight;    // Highlight color
  uint16_t dimmed;       // Dimmed elements
};

// Predefined themes
const ThemeColors THEMES[THEME_COUNT] = {
  // THEME_DARK - Classic black background
  {
    0x0000,  // bg: black
    0xFFFF,  // text: white
    0x07FF,  // accent: cyan
    0xFD20,  // warning: orange
    0xF800,  // error: red
    0x07E0,  // success: green
    0xC618,  // secondary: light gray
    0xFFE0,  // highlight: yellow
    0x4208   // dimmed: dark gray
  },

  // THEME_LIGHT - White/light background
  {
    0xFFFF,  // bg: white
    0x0000,  // text: black
    0x001F,  // accent: blue
    0xFD20,  // warning: orange
    0xF800,  // error: red
    0x07E0,  // success: green
    0x8410,  // secondary: gray
    0xFFE0,  // highlight: yellow
    0xC618   // dimmed: light gray
  },

  // THEME_NEON - Enhanced Cyberpunk neon theme (cyan/magenta)
  {
    0x0010,  // bg: deep black with blue tint
    0x07FF,  // text: bright cyan
    0xF81F,  // accent: hot magenta
    0xFFE0,  // warning: bright yellow
    0xF800,  // error: bright red
    0x07E0,  // success: bright green
    0x051F,  // secondary: electric blue
    0xFD20,  // highlight: neon orange
    0x2104   // dimmed: dark blue-gray
  },

  // THEME_NEON_PINK - Hot pink neon theme
  {
    0x0000,  // bg: pure black
    0xF81F,  // text: hot pink/magenta
    0xFC1F,  // accent: bright pink
    0xFFE0,  // warning: yellow
    0xF800,  // error: red
    0x07FF,  // success: cyan
    0xF99E,  // secondary: light pink
    0xFD20,  // highlight: orange-pink
    0x4208   // dimmed: dark gray
  },

  // THEME_NEON_BLUE - Electric blue neon theme
  {
    0x0008,  // bg: black with blue tint
    0x07FF,  // text: bright cyan
    0x001F,  // accent: deep blue
    0xFFE0,  // warning: yellow
    0xF800,  // error: red
    0x07E0,  // success: green
    0x051F,  // secondary: electric blue
    0x07FF,  // highlight: cyan
    0x2104   // dimmed: dark blue
  },

  // THEME_NEON_GREEN - Toxic green neon theme
  {
    0x0200,  // bg: black with green tint
    0x07E0,  // text: bright green
    0xAFE5,  // accent: lime green
    0xFFE0,  // warning: yellow
    0xF800,  // error: red
    0x07FF,  // success: cyan
    0x87E0,  // secondary: yellow-green
    0xFFE0,  // highlight: yellow
    0x2945   // dimmed: dark green
  },

  // THEME_NEON_PURPLE - Purple/violet neon theme
  {
    0x2004,  // bg: black with purple tint
    0xF81F,  // text: magenta
    0x781F,  // accent: violet
    0xFFE0,  // warning: yellow
    0xF800,  // error: red
    0x07FF,  // success: cyan
    0xA01F,  // secondary: purple
    0xFC1F,  // highlight: bright pink
    0x4010   // dimmed: dark purple
  },

  // THEME_MINIMAL - Clean minimal theme
  {
    0x0000,  // bg: black
    0xFFFF,  // text: white
    0xFFFF,  // accent: white
    0x8410,  // warning: gray
    0x8410,  // error: gray
    0xC618,  // success: light gray
    0x8410,  // secondary: gray
    0xFFFF,  // highlight: white
    0x4208   // dimmed: dark gray
  }
};

// Theme names for display
const char* const THEME_NAMES[THEME_COUNT] = {
  "Dark",
  "Light",
  "Neon",
  "Neon Pink",
  "Neon Blue",
  "Neon Green",
  "Neon Purple",
  "Minimal"
};

class ThemeManager {
public:
  ThemeManager();

  // Initialize theme system
  void init();

  // Theme management
  void setTheme(ThemeType theme);
  ThemeType getCurrentTheme() const { return currentTheme; }
  const ThemeColors& getColors() const { return THEMES[currentTheme]; }
  const char* getThemeName() const { return THEME_NAMES[currentTheme]; }

  // Theme navigation
  void nextTheme();
  void previousTheme();

  // Persistence (EEPROM/flash)
  void saveTheme();
  void loadTheme();

private:
  ThemeType currentTheme;
  bool initialized;
};

#endif // THEME_H
