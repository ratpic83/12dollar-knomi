# Features & Roadmap

## Current Features ✅

### Display Screens
- **Boot Screen**: Shows logo and version on startup
- **Connecting Screen**: WiFi connection status
- **Idle Screen**: Rolling eyes animation when printer is ready
- **Printing Screen**: 
  - Circular progress indicator
  - Current/target temperatures (hotend & bed)
  - Print progress percentage
  - Estimated time remaining
  - Current Z height
  - Filename display
- **Paused Screen**: Shows paused state with current progress
- **Complete Screen**: Success indicator when print finishes
- **Error Screens**: WiFi and Klipper connection errors

### Monitoring
- Real-time temperature monitoring (hotend, bed, chamber)
- Print progress tracking
- Position monitoring (X, Y, Z)
- Feed rate and flow rate display
- Fan speed monitoring
- Connection status indicators

### Animations
- Rolling eyes when idle (like original Knomi)
- Smooth progress ring animation
- Blinking eyes animation
- Status transitions

### Communication
- WiFi connectivity (2.4GHz)
- Moonraker HTTP API integration
- Automatic reconnection on connection loss
- Configurable update intervals
- Error recovery

### Configuration
- Easy WiFi setup via header file
- Adjustable display brightness
- Configurable update intervals
- Debug output options
- Custom partition scheme for ESP32-C3

## Planned Features 🚧

### Short Term (Next Release)

#### Touch Support
- [ ] Touch screen calibration
- [ ] Basic menu navigation
- [ ] Brightness adjustment via touch
- [ ] Screen switching (swipe gestures)

#### Enhanced UI
- [ ] Multiple screen layouts
- [ ] Customizable themes/colors
- [ ] Graph view for temperature history
- [ ] Layer progress indicator
- [ ] Filament usage display

#### Better Animations
- [ ] Smooth transitions between screens
- [ ] Custom boot animation
- [ ] Print completion celebration animation
- [ ] Error shake animation

### Medium Term

#### Web Configuration
- [ ] Web-based WiFi setup (captive portal)
- [ ] OTA firmware updates
- [ ] Remote configuration interface
- [ ] Display settings via web UI
- [ ] Theme editor

#### Advanced Monitoring
- [ ] Multi-extruder support
- [ ] Chamber temperature display
- [ ] Webcam preview (low-res)
- [ ] Print time estimation improvements
- [ ] Filament sensor status

#### Klipper Integration
- [ ] WebSocket support (instead of polling)
- [ ] Direct control buttons (pause/resume/cancel)
- [ ] Emergency stop button
- [ ] Macro execution from display
- [ ] Custom G-code commands

#### Notifications
- [ ] Print completion alerts
- [ ] Error notifications
- [ ] Temperature alerts
- [ ] Filament runout warnings

### Long Term

#### Hardware Enhancements
- [ ] External button support
- [ ] Rotary encoder support
- [ ] RGB LED integration
- [ ] Buzzer/speaker support
- [ ] Multiple display support

#### Smart Features
- [ ] Print failure detection
- [ ] Automatic bed leveling visualization
- [ ] Spaghetti detection integration
- [ ] Power loss recovery status
- [ ] Print queue management

#### Advanced UI
- [ ] Menu system
- [ ] Settings editor
- [ ] File browser
- [ ] Statistics dashboard
- [ ] Print history

#### Connectivity
- [ ] MQTT support
- [ ] Home Assistant integration
- [ ] Octoprint support
- [ ] Multiple printer support
- [ ] Cloud connectivity (optional)

#### Developer Features
- [ ] Plugin system
- [ ] Custom screen API
- [ ] Theme SDK
- [ ] Simulator for testing
- [ ] Documentation generator

## Feature Requests

Want a feature not listed here? 

1. Check existing issues on GitHub
2. Create a new feature request
3. Describe your use case
4. Include mockups if possible

## Contributing

We welcome contributions! See areas where you can help:

### Easy (Good First Issues)
- Add new color themes
- Improve documentation
- Create 3D printable mounts
- Test on different hardware
- Report bugs

### Medium
- Implement touch support
- Add new screen layouts
- Improve animations
- Optimize performance
- Add unit tests

### Advanced
- WebSocket implementation
- OTA update system
- Web configuration interface
- Multi-printer support
- Plugin architecture

## Performance Targets

### Current Performance
- **Update Rate**: 1 Hz (1 second intervals)
- **Boot Time**: ~3-5 seconds
- **WiFi Connect**: ~5-10 seconds
- **Memory Usage**: ~60% of available RAM
- **Flash Usage**: ~90% of app partition

### Optimization Goals
- Reduce memory usage to <50%
- Faster boot time (<2 seconds)
- Higher update rate (2-5 Hz)
- Smoother animations (60 FPS)
- Lower power consumption

## Known Limitations

### Hardware
- ESP32-C3 has only 400KB RAM (vs 520KB on ESP32)
- Single core CPU (vs dual core on ESP32)
- Limited GPIO pins
- No Bluetooth support in current implementation

### Software
- Polling-based updates (WebSocket would be better)
- No OTA updates yet
- Limited error recovery
- Basic UI (no menus yet)

### Display
- 240x240 resolution limits detail
- Round display wastes some pixels
- No touch support yet
- Fixed orientation

## Compatibility

### Tested With
- ✅ Klipper (latest)
- ✅ Moonraker (latest)
- ✅ Mainsail
- ✅ Fluidd
- ⚠️ Octoprint (untested, should work)

### Printer Types
- ✅ Voron (all models)
- ✅ Ender 3 (with Klipper)
- ✅ Prusa (with Klipper)
- ✅ CoreXY printers
- ✅ Cartesian printers
- ✅ Delta printers (untested)

### Display Hardware
- ✅ ESP32-C3 + GC9A01 (primary target)
- ⚠️ ESP32 + GC9A01 (should work with pin changes)
- ⚠️ Other round displays (may need driver changes)

## Version History

### v1.1.0 (Current)
- PlatformIO-based build system
- Improved project structure
- Better IDE integration
- Automatic dependency management
- All v1.0.0 features maintained

### v1.0.0
- Initial release
- Basic display functionality
- WiFi connectivity
- Klipper integration
- Rolling eyes animation
- Temperature monitoring
- Print progress tracking

### Planned Releases

**v1.2.0** - Touch & UI Improvements
- Touch screen support
- Enhanced animations
- Multiple screen layouts

**v1.3.0** - Web Configuration
- Captive portal setup
- OTA updates
- Web-based settings

**v2.0.0** - Advanced Features
- WebSocket support
- Menu system
- Direct printer control
- Plugin support

## Feedback

Your feedback helps prioritize features!

- ⭐ Star features you want
- 💬 Comment on implementation ideas
- 🐛 Report bugs you encounter
- 📝 Suggest improvements

Join the discussion on GitHub!
