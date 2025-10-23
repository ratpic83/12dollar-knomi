# Touch Screen Setup Guide

## Overview

The Knomi Clone supports capacitive touch screens for intuitive interaction. Touch functionality allows users to cycle through themes, navigate menus, and interact with the display without physical buttons.

## Supported Touch Controllers

### FT6236 (Recommended)
- **Interface**: I2C
- **Resolution**: 240x240 (matches display)
- **Touch Points**: Single touch
- **Power**: 3.3V
- **Compatibility**: Most round TFT displays

## Hardware Connections

### ESP32-C3 Round Display Pinout

```
ESP32-C3 Round Display
┌─────────────────┐
│ VCC  GND  IO2  │ ← Display connections
│ IO3  IO4  IO5  │
│ IO6  IO7  RST  │ ← SPI for display
│ IO8  IO9  IO10 │ ← Available for sensors
│ IO18 IO19 IO20 │ ← I2C for touch (IO18=SDA, IO19=SCL)
└─────────────────┘
```

### FT6236 Touch Controller

```
ESP32-C3     FT6236
───────     ──────
3.3V    →   VCC
GND     →   GND
IO18    →   SDA
IO19    →   SCL
GND     →   RST (optional)
GND     →   INT (optional)
```

## Software Configuration

### Automatic Detection

The firmware automatically detects the FT6236 touch controller on boot. No configuration changes required!

1. **I2C Scan**: Searches for touch controller at addresses 0x38 and 0x39
2. **Initialization**: Sets up touch interrupts and gesture recognition
3. **Calibration**: Uses display coordinates (0,0 to 240,240)

### Touch Features

#### Gesture Recognition
- **Tap**: Quick touch and release (theme switching)
- **Double Tap**: Two quick taps (future use)
- **Swipe Left/Right**: Horizontal finger movement (navigation)
- **Swipe Up/Down**: Vertical finger movement (menus)

#### Touch Zones (Framework Ready)
- Define interactive areas on screen
- Different actions per zone
- Visual feedback for touch zones

#### Visual Feedback
- **Touch Rings**: Animated rings around screen perimeter
- **Theme Colors**: Feedback uses current theme highlight color
- **Fade Animation**: 200ms fade-out effect

## Display Integration

### Idle Screen Touch
- **Tap anywhere** to cycle through themes
- **Visual feedback** ring appears instantly
- **Theme changes** apply immediately

### Printing Screen Touch
- **Tap anywhere** to cycle themes during print
- **No interference** with print monitoring
- **Same feedback** as idle screen

### Future Touch Features
- **Menu Navigation**: Swipe to access settings
- **Print Control**: Touch to pause/resume/cancel
- **Statistics View**: Swipe to see print stats
- **Custom Actions**: User-defined touch zones

## Configuration Options

### Gesture Sensitivity

Modify in `TouchDriver.cpp`:

```cpp
// Adjust swipe threshold (pixels)
swipeThreshold = 50;  // Default: 50

// Adjust tap timeout (milliseconds)
tapThreshold = 20;    // Default: 20
```

### Touch Feedback

Customize in `UIManager.cpp`:

```cpp
// Adjust feedback duration
const unsigned long TOUCH_FEEDBACK_DURATION = 200; // milliseconds

// Modify feedback colors
uint16_t color = getThemeColors().highlight; // Use any theme color
```

### Debug Output

Enable touch debugging in serial monitor:

```cpp
// In main.cpp loop
Serial.printf("Touch: Event=%d, Point=(%d,%d)\n",
              touchEvent, touchDriver.getPoint().x, touchDriver.getPoint().y);
```

## Troubleshooting

### Touch Not Detected
- **Check I2C pins**: Ensure IO18=SDA, IO19=SCL
- **Power supply**: Verify 3.3V power to touch controller
- **I2C address**: Controller may be at 0x38 or 0x39
- **Serial output**: Check for "Touch: FT6236 found at 0x38/39"

### Unresponsive Touch
- **Calibration**: Touch coordinates should match display (0-240)
- **Grounding**: Ensure proper grounding of touch controller
- **ESD protection**: Add ESD protection if in noisy environment
- **Firmware**: Ensure latest touch driver firmware

### Erratic Touch
- **Noise filtering**: Increase tap/swipe thresholds
- **Debouncing**: Touch events are already debounced
- **Interference**: Keep touch controller away from power lines
- **Clean surface**: Ensure touch surface is clean

### Visual Feedback Issues
- **Theme colors**: Feedback uses highlight color - check theme
- **Animation timing**: 200ms may be too fast/slow for preference
- **Alpha blending**: Ensure TFT_eSPI supports alphaBlend()

## Performance Impact

- **Memory**: ~3KB additional RAM for touch libraries
- **CPU**: Minimal additional processing (<1% load)
- **Power**: Negligible additional current draw
- **Boot Time**: +0.5-1 second for touch initialization

## Advanced Configuration

### Custom Touch Zones

Add interactive areas in `UIManager.cpp`:

```cpp
// Define touch zones
touchDriver.addZone(1, 50, 50, 100, 100);    // Zone 1: top-left
touchDriver.addZone(2, 90, 90, 60, 60);      // Zone 2: center

// Check touched zone
uint8_t zone = touchDriver.getTouchedZone();
if (zone == 1) {
    // Handle zone 1 touch
} else if (zone == 2) {
    // Handle zone 2 touch
}
```

### Custom Gestures

Extend gesture recognition in `TouchDriver.cpp`:

```cpp
TouchEvent TouchDriver::detectGesture() {
    // Add custom gesture logic here
    // Return appropriate TouchEvent
}
```

## Recommended Setup

1. **Mount display** securely to prevent vibration
2. **Clean touch surface** before first use
3. **Test touch response** after installation
4. **Adjust sensitivity** if needed for your setup
5. **Use with gloves** - capacitive touch requires conductive contact

## Cost Impact

- **Touch Display**: +$2-5 vs non-touch display
- **No additional components** required
- **Same firmware** works with touch and non-touch displays
- **Automatic detection** - no configuration needed

## Compatibility

- ✅ **FT6236 controllers** (most round displays)
- ✅ **I2C interface** (standard)
- ✅ **3.3V operation** (ESP32 compatible)
- ✅ **Round displays** (circular touch zones)
- ❌ **Resistive touch** (not supported)
- ❌ **Multi-touch** (single touch only)
