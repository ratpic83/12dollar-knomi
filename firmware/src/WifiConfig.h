/*
 * WiFi and Klipper Configuration
 * 
 * IMPORTANT: Update these settings with your network and printer information!
 */

#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

// WiFi Settings
#define WIFI_SSID "azazel"           // Your WiFi network name
#define WIFI_PASSWORD "fritzipupa"   // Your WiFi password

// Klipper/Moonraker Settings
#define KLIPPER_IP "192.168.68.91"         // IP address of your Klipper host
#define KLIPPER_PORT 7125                   // Moonraker port (default: 7125)

// Update intervals (milliseconds)
#define STATUS_UPDATE_INTERVAL 1000         // How often to fetch status (1 second)
#define TEMP_UPDATE_INTERVAL 2000           // How often to fetch temperatures (2 seconds)

// Display Settings
#define DISPLAY_BRIGHTNESS 200              // 0-255, default brightness
#define DISPLAY_ROTATION 0                  // 0, 1, 2, or 3 (90° increments)

// Animation Settings
#define ANIMATION_SPEED 50                  // Animation frame delay (ms)
#define EYE_BLINK_INTERVAL 3000            // How often eyes blink (ms)

// Debug Settings
#define DEBUG_SERIAL true                   // Enable serial debug output
#define DEBUG_API true                      // Enable API debug output

#endif // WIFI_CONFIG_H
