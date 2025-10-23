/*
 * Theme Manager Implementation
 */

#include "Theme.h"
#include <EEPROM.h>

// EEPROM address for theme storage
#define THEME_EEPROM_ADDR 0
#define THEME_MAGIC 0x4B4E  // "KN" in hex

ThemeManager::ThemeManager() :
  currentTheme(THEME_DARK),
  initialized(false) {
}

void ThemeManager::init() {
  if (!initialized) {
    // EEPROM.begin(512);  // Disabled for testing
    loadTheme();
    initialized = true;
  }
}

void ThemeManager::setTheme(ThemeType theme) {
  if (theme >= 0 && theme < THEME_COUNT) {
    currentTheme = theme;
    // saveTheme(); // Disabled for testing
  }
}

void ThemeManager::nextTheme() {
  currentTheme = (ThemeType)((currentTheme + 1) % THEME_COUNT);
  // saveTheme(); // Disabled for testing
}

void ThemeManager::previousTheme() {
  currentTheme = (ThemeType)((currentTheme + THEME_COUNT - 1) % THEME_COUNT);
  // saveTheme(); // Disabled for testing
}

void ThemeManager::saveTheme() {
  if (!initialized) return;

  // EEPROM.write(THEME_EEPROM_ADDR, THEME_MAGIC & 0xFF);
  // EEPROM.write(THEME_EEPROM_ADDR + 1, (THEME_MAGIC >> 8) & 0xFF);
  // EEPROM.write(THEME_EEPROM_ADDR + 2, currentTheme);
  // EEPROM.commit();
}

void ThemeManager::loadTheme() {
  if (!initialized) return;

  // Read magic number
  // uint16_t magic = EEPROM.read(THEME_EEPROM_ADDR) | (EEPROM.read(THEME_EEPROM_ADDR + 1) << 8);

  // if (magic == THEME_MAGIC) {
  //   // Valid magic, read theme
  //   uint8_t savedTheme = EEPROM.read(THEME_EEPROM_ADDR + 2);
  //   if (savedTheme < THEME_COUNT) {
  //     currentTheme = (ThemeType)savedTheme;
  //   }
  // } else {
    // First run or corrupted, use default
    currentTheme = THEME_DARK;
    // saveTheme();
  // }
}
