# Changelog

All notable changes to the Knomi Clone project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2024-10-21

### Added
- Initial release of Knomi Clone firmware
- ESP32-C3 support for GC9A01 round display
- WiFi connectivity with automatic reconnection
- Klipper/Moonraker API integration
- Multiple display screens:
  - Boot screen with logo
  - WiFi connection status
  - Idle screen with rolling eyes animation
  - Printing screen with progress ring
  - Paused screen
  - Completion screen
  - Error screens (WiFi, Klipper)
- Real-time monitoring:
  - Hotend temperature (current/target)
  - Bed temperature (current/target)
  - Chamber temperature support
  - Print progress percentage
  - Z height display
  - Print time remaining
  - Filename display
- Animated UI elements:
  - Rolling eyes when idle
  - Blinking animation
  - Smooth progress ring
  - Status transitions
- Configuration system:
  - WiFi credentials via header file
  - Adjustable update intervals
  - Display brightness control
  - Debug output options
- Klipper integration:
  - Macro variables for homing/leveling status
  - G-code overrides for state tracking
  - Custom status messages
  - Print control macros
- Documentation:
  - Comprehensive README
  - Quick start guide
  - Building instructions
  - Hardware guide
  - Klipper setup guide
  - Features and roadmap
- Development tools:
  - PlatformIO configuration
  - Custom partition scheme for ESP32-C3
  - TFT_eSPI configuration
  - Example configurations
- License: MIT

### Technical Details
- Memory optimized for ESP32-C3 (400KB RAM)
- Custom partition scheme (3.3MB app / 1MB SPIFFS)
- HTTP polling for Klipper status (1 second interval)
- Modular code architecture
- JSON parsing for API responses
- Error recovery and reconnection logic

### Known Issues
- No touch screen support yet
- No OTA updates
- Polling-based updates (not WebSocket)
- Limited to 2.4GHz WiFi
- No web configuration interface

## [1.3.0] - 2025-10-23

### Added
- **Touch Screen Support**: Complete capacitive touch implementation
  - FT6236 touch controller support
  - Gesture recognition (tap, double-tap, swipe)
  - Touch zones for UI interaction
  - Visual touch feedback animations
- **Touch Interactions**: Intuitive touch controls
  - Tap to cycle through themes (idle/printing screens)
  - Swipe gestures for navigation (framework ready)
  - Touch feedback rings with theme-aware colors
  - Capacitive touch detection with noise filtering
- **Enhanced User Experience**: Professional touch interface
  - 200ms touch feedback animations
  - Gesture threshold configuration
  - Touch event debugging output
  - Theme-aware touch feedback colors

### Improved
- **UI Responsiveness**: Instant theme switching via touch
- **Hardware Integration**: I2C touch controller auto-detection
- **Performance**: Optimized touch event processing
- **Documentation**: Touch setup and usage guides

### Technical Details
- Added `TouchDriver.h/cpp` for capacitive touch handling
- Added `handleTouchEvent()` method to UIManager
- Added `drawTouchFeedbackRing()` to DisplayDriver
- Integrated touch updates in main application loop
- Touch library: `adafruit/Adafruit_FT6236@^1.1.1`

## [1.2.0] - 2025-10-23

### Added
- **Theme System**: 4 beautiful color schemes (Dark, Light, Neon, Minimal)
  - Live theme switching via GPIO 9 button
  - EEPROM persistence of theme selection
  - Theme-aware color palette throughout UI
- **Environmental Monitoring**: Chamber sensor integration
  - Support for BME280, DHT11/22, SHT30 sensors
  - Real-time temperature, humidity, pressure display
  - Automatic sensor detection and initialization
  - Environmental data shown on idle screen
- **Web Configuration Portal**: Framework for captive portal setup
  - WiFi AP mode for first-time configuration
  - HTML/CSS interface with form validation
  - EEPROM storage of configuration
  - Automatic device restart after setup
- **HD Graphics Enhancements**:
  - Anti-aliased progress rings with 3D depth effects
  - Smooth sinusoidal animations for eye movements
  - Temperature gauges positioned in screen corners
  - Enhanced visual effects and transitions

### Improved
- **UI/UX**: Professional polish with theme support
- **Performance**: Optimized sensor reading with caching
- **Documentation**: Comprehensive guides for all features
- **Build System**: Additional sensor libraries in PlatformIO

### Technical Details
- Added `Theme.h/cpp` for color management
- Added `Environmental.h/cpp` for sensor integration
- Added `WebConfig.h/cpp` for web portal functionality
- Enhanced `DisplayDriver` and `UIManager` with theme support
- Extended `PrinterStatus` with environmental data fields

## [Unreleased]

### Planned for v1.2.0
- Touch screen support
- Multiple screen layouts
- Enhanced animations
- Temperature history graphs
- Swipe gestures for screen navigation

### Planned for v1.3.0
- Web-based configuration portal
- OTA firmware updates
- Theme customization
- Settings editor

### Planned for v2.0.0
- WebSocket support for real-time updates
- Menu system
- Direct printer control (pause/resume/cancel)
- Plugin architecture
- Multi-printer support

## Development Notes

### Version Numbering
- **Major version** (X.0.0): Breaking changes, major features
- **Minor version** (0.X.0): New features, backwards compatible
- **Patch version** (0.0.X): Bug fixes, minor improvements

### Release Process
1. Update version in code
2. Update CHANGELOG.md
3. Create git tag
4. Build and test firmware
5. Create GitHub release
6. Update documentation

### Contributing
See CONTRIBUTING.md for guidelines on submitting changes.

---

## Version History Summary

| Version | Date | Highlights |
|---------|------|------------|
| 1.3.0 | 2025-10-23 | Touch screen support, gesture recognition, visual feedback |
| 1.2.0 | 2025-10-23 | Theme system, environmental sensors, web portal, HD graphics |
| 1.1.0 | 2025-10-23 | PlatformIO migration, improved build system |
| 1.0.0 | 2024-10-21 | Initial release, basic functionality |

---

**Legend:**
- `Added` - New features
- `Changed` - Changes in existing functionality
- `Deprecated` - Soon-to-be removed features
- `Removed` - Removed features
- `Fixed` - Bug fixes
- `Security` - Security improvements
