/*
 * WiFi Configuration Example
 * 
 * Copy this file to firmware/KnomiC3/WifiConfig.h and update with your settings
 * 
 * IMPORTANT: Never commit WifiConfig.h with real credentials to version control!
 */

#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

// ============================================================================
// WiFi Settings
// ============================================================================

// Your WiFi network name (SSID)
// Note: ESP32 only supports 2.4GHz WiFi, not 5GHz
#define WIFI_SSID "YourWiFiSSID"

// Your WiFi password
#define WIFI_PASSWORD "YourWiFiPassword"

// ============================================================================
// Klipper/Moonraker Settings
// ============================================================================

// IP address of your Klipper host (Raspberry Pi, etc.)
// Find this in your router, or run: hostname -I
#define KLIPPER_IP "192.168.1.100"

// Moonraker API port (default is 7125, usually no need to change)
#define KLIPPER_PORT 7125

// ============================================================================
// Update Intervals (milliseconds)
// ============================================================================

// How often to fetch printer status
// Lower = more responsive, but more network traffic
// Recommended: 1000-2000ms
#define STATUS_UPDATE_INTERVAL 1000

// How often to fetch temperature data
// Can be slower than status updates
// Recommended: 2000-5000ms
#define TEMP_UPDATE_INTERVAL 2000

// ============================================================================
// Display Settings
// ============================================================================

// Display brightness (0-255)
// 0 = off, 255 = maximum brightness
// Recommended: 150-200 for normal use
#define DISPLAY_BRIGHTNESS 200

// Display rotation (0, 1, 2, or 3)
// Each increment rotates 90 degrees clockwise
// 0 = normal, 1 = 90°, 2 = 180°, 3 = 270°
#define DISPLAY_ROTATION 0

// ============================================================================
// Animation Settings
// ============================================================================

// Animation frame delay in milliseconds
// Lower = smoother but more CPU usage
// Recommended: 30-50ms
#define ANIMATION_SPEED 50

// How often the eyes blink when idle (milliseconds)
// Recommended: 3000-5000ms
#define EYE_BLINK_INTERVAL 3000

// ============================================================================
// Debug Settings
// ============================================================================

// Enable serial debug output
// Set to true for troubleshooting, false for production
#define DEBUG_SERIAL true

// Enable detailed API debug output
// Shows all API requests and responses
// Warning: Very verbose! Only enable when debugging API issues
#define DEBUG_API false

// ============================================================================
// Advanced Settings (usually no need to change)
// ============================================================================

// WiFi connection timeout (seconds)
// How long to wait for WiFi connection before giving up
#define WIFI_TIMEOUT 30

// API request timeout (milliseconds)
// How long to wait for Klipper response
#define API_TIMEOUT 5000

// Maximum WiFi reconnection attempts
// 0 = infinite retries
#define WIFI_MAX_RETRIES 0

// Enable WiFi power saving
// Set to false if experiencing connection issues
#define WIFI_POWER_SAVE false

#endif // WIFI_CONFIG_H

/*
 * Configuration Examples:
 * 
 * Example 1: Home Network
 * #define WIFI_SSID "HomeNetwork"
 * #define WIFI_PASSWORD "MySecurePassword123"
 * #define KLIPPER_IP "192.168.1.100"
 * 
 * Example 2: Printer on Separate Network
 * #define WIFI_SSID "PrinterNetwork"
 * #define WIFI_PASSWORD "PrinterPass"
 * #define KLIPPER_IP "10.0.0.50"
 * 
 * Example 3: Fast Updates for Responsive Display
 * #define STATUS_UPDATE_INTERVAL 500
 * #define TEMP_UPDATE_INTERVAL 1000
 * 
 * Example 4: Power Saving Mode
 * #define STATUS_UPDATE_INTERVAL 2000
 * #define TEMP_UPDATE_INTERVAL 5000
 * #define DISPLAY_BRIGHTNESS 100
 * 
 * Example 5: Debug Mode
 * #define DEBUG_SERIAL true
 * #define DEBUG_API true
 * 
 * Troubleshooting:
 * 
 * WiFi won't connect:
 * - Verify SSID and password are correct
 * - Ensure using 2.4GHz WiFi (not 5GHz)
 * - Check WiFi signal strength
 * - Try increasing WIFI_TIMEOUT
 * 
 * Can't connect to Klipper:
 * - Verify KLIPPER_IP is correct
 * - Test: curl http://[IP]:7125/server/info
 * - Check firewall settings
 * - Ensure Moonraker is running
 * 
 * Display is slow/laggy:
 * - Decrease STATUS_UPDATE_INTERVAL
 * - Check WiFi signal strength
 * - Reduce ANIMATION_SPEED
 * 
 * High network traffic:
 * - Increase STATUS_UPDATE_INTERVAL
 * - Increase TEMP_UPDATE_INTERVAL
 * - Disable DEBUG_API
 */
