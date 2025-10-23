/*
 * TFT_eSPI User Setup for GC9A01 Round Display
 * 
 * This file should be copied to the TFT_eSPI library folder
 * Location: Arduino/libraries/TFT_eSPI/User_Setup.h
 * 
 * Or create User_Setup_Select.h and include this file
 */

// Driver selection
#define GC9A01_DRIVER

// Display size
#define TFT_WIDTH  240
#define TFT_HEIGHT 240

// ESP32-C3 Pin Configuration
// Adjust these pins according to your hardware
#define TFT_MOSI 7   // SDA
#define TFT_SCLK 6   // SCL
#define TFT_CS   10  // Chip select
#define TFT_DC   2   // Data/Command
#define TFT_RST  3   // Reset
#define TFT_BL   11  // Backlight (optional)

// SPI Frequency
#define SPI_FREQUENCY  40000000  // 40MHz
#define SPI_READ_FREQUENCY  20000000

// Optional: Touch screen (if your display has touch)
// #define TOUCH_CS 16

// Font settings
#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

// Smooth fonts
#define SMOOTH_FONT

// Color depth
#define TFT_RGB_ORDER TFT_RGB  // Colour order Red-Green-Blue
// #define TFT_RGB_ORDER TFT_BGR  // Colour order Blue-Green-Red

// Other options
#define SPI_FREQUENCY  40000000
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY  2500000
