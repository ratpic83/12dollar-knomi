# ✨ PaWe i-print Features Guide

Complete guide to all features, gestures, and customization options.

---

## 🎮 Touch Gestures

### **Tap** - Theme Switching
**Action:** Single tap anywhere on screen  
**Effect:** Cycles through 4 color themes  
**Visual Feedback:** Screen clears and redraws with new theme  

**Themes:**
1. **Dark** - Black background, cyan accents (default)
2. **Light** - White background, blue accents
3. **Neon** - Black background, magenta/cyan cyberpunk
4. **Minimal** - Gray tones, subtle colors

**Exit Manual Mode:** Tapping also exits manual screen lock mode

---

### **Swipe Up** - Increase Brightness
**Action:** Swipe finger upward  
**Effect:** Increases backlight by 25 units  
**Range:** 20-255 (prevents complete darkness)  
**Visual Feedback:** Brightness indicator overlay (200ms)  

**Technical:**
- PWM control on GPIO3
- 8-bit resolution (256 levels)
- 5kHz frequency (flicker-free)

---

### **Swipe Down** - Decrease Brightness
**Action:** Swipe finger downward  
**Effect:** Decreases backlight by 25 units  
**Range:** 20-255 (minimum 20 for visibility)  
**Visual Feedback:** Brightness indicator overlay (200ms)  

---

### **Swipe Left** - Next Screen
**Action:** Swipe finger left (→)  
**Effect:** Advances to next display mode  
**Sequence:** Idle → Printing → Paused → Complete → Idle  
**Mode:** Enters **Manual Mode** (locks screen)  

**Manual Mode:**
- Screen won't auto-switch based on printer state
- Tap to exit and return to automatic mode
- Useful for inspecting different screens

---

### **Swipe Right** - Previous Screen
**Action:** Swipe finger right (←)  
**Effect:** Goes to previous display mode  
**Sequence:** Idle → Complete → Paused → Printing → Idle  
**Mode:** Enters **Manual Mode** (locks screen)  

---

### **Draw Circle** - Easter Egg
**Action:** Draw a circle with your finger  
**Detection:** Must pass through all 4 quadrants  
**Effect:** Shows "VIVA LA ELTON JOHN" with rainbow 🌈  
**Duration:** 3 seconds, then returns to current screen  

**How to trigger:**
1. Start anywhere on screen
2. Move finger in circular motion
3. Pass through top-right, top-left, bottom-left, bottom-right
4. Release

---

## 📊 Display Modes

### **Idle Screen**
**When:** Printer is idle or standby  
**Features:**
- Rolling eyes animation (60 FPS)
- Smooth easing movements
- Current hotend temperature
- Current bed temperature
- WiFi status indicator

**Animation Details:**
- Eyes look around randomly
- Blink occasionally
- Smooth transitions
- No screen flicker

---

### **Printing Screen**
**When:** Active print job  
**Features:**
- Circular progress ring
- Print progress percentage
- Time remaining
- Hotend temp (current/target)
- Bed temp (current/target)
- Fan speed
- File name (scrolling if long)

**Progress Ring:**
- Animated fill
- Color-coded (theme-based)
- Anti-aliased edges
- Updates every second

---

### **Paused Screen**
**When:** Print is paused  
**Features:**
- Dimmed progress ring
- "PAUSED" text overlay
- Current temperatures
- Pause duration
- Resume instructions

**Visual Style:**
- Reduced brightness
- Pulsing "PAUSED" text
- Maintains print progress

---

### **Complete Screen**
**When:** Print finished successfully  
**Features:**
- 100% progress ring
- "COMPLETE" text
- Final print time
- Success animation
- Print statistics

**Celebration:**
- Green success color
- Animated checkmark
- Print summary

---

### **Error Screen**
**When:** Printer error detected  
**Features:**
- Red error indicator
- Error message
- "ERROR" text
- Troubleshooting hints
- Retry instructions

---

## 🎨 Theme System

### Dark Theme (Default)
```
Background: #000000 (Black)
Text:       #FFFFFF (White)
Primary:    #00FFFF (Cyan)
Secondary:  #808080 (Gray)
Accent:     #FF00FF (Magenta)
Success:    #00FF00 (Green)
Error:      #FF0000 (Red)
```

### Light Theme
```
Background: #FFFFFF (White)
Text:       #000000 (Black)
Primary:    #0080FF (Blue)
Secondary:  #C0C0C0 (Light Gray)
Accent:     #FF8000 (Orange)
Success:    #00C000 (Dark Green)
Error:      #C00000 (Dark Red)
```

### Neon Theme
```
Background: #000000 (Black)
Text:       #00FFFF (Cyan)
Primary:    #FF00FF (Magenta)
Secondary:  #8000FF (Purple)
Accent:     #FFFF00 (Yellow)
Success:    #00FF00 (Neon Green)
Error:      #FF0040 (Hot Pink)
```

### Minimal Theme
```
Background: #202020 (Dark Gray)
Text:       #E0E0E0 (Light Gray)
Primary:    #808080 (Gray)
Secondary:  #606060 (Medium Gray)
Accent:     #A0A0A0 (Light Gray)
Success:    #80C080 (Muted Green)
Error:      #C08080 (Muted Red)
```

---

## 🌈 Boot Logo

### Default: Electric Callboy Rainbow
**Design:**
- Rainbow arc (7 colors)
- "PaWe" text (large)
- "i-print" text (medium)
- "v1.1" version (small)
- Loading indicator

**Colors:**
- Red → Orange → Yellow → Green → Blue → Indigo → Violet

### Custom Boot Logos
See [IMAGE_FEATURES.md](IMAGE_FEATURES.md) for creating custom logos.

**Quick Example:**
```bash
cd tools
source venv/bin/activate
python image_to_header.py my_logo.png ../firmware/src/custom_logo.h
```

---

## 🔧 Advanced Features

### Manual/Auto Mode
**Auto Mode (Default):**
- Screen automatically switches based on printer state
- Idle → shows eyes
- Printing → shows progress
- Paused → shows pause screen

**Manual Mode:**
- Activated by swiping left/right
- Screen stays locked to selected mode
- Useful for:
  - Inspecting different screens
  - Keeping progress visible when idle
  - Testing UI elements
- Exit by tapping screen

---

### Brightness Control
**Range:** 20-255 (8-bit PWM)  
**Default:** 200 (~78%)  
**Step Size:** 25 units per swipe  
**Persistence:** Resets to default on reboot  

**Tips:**
- Lower brightness saves power
- Minimum 20 prevents complete darkness
- Adjust for ambient lighting

---

### Gesture Sensitivity
**Swipe Threshold:** 50 pixels  
**Tap Threshold:** 20 pixels  
**Tap Duration:** < 500ms  
**Circle Detection:** All 4 quadrants  

**Tuning (in TouchDriver.h):**
```cpp
swipeThreshold = 50;   // Increase for less sensitive swipes
tapThreshold = 20;     // Decrease for more precise taps
```

---

## 📡 Connectivity Features

### Moonraker API
**Endpoint:** `http://KLIPPER_IP:7125`  
**Poll Rate:** Every 1 second  
**Data Retrieved:**
- Printer state
- Temperatures (hotend, bed, chamber)
- Print progress
- File name
- Fan speeds
- Print statistics

**No Klipper Config Required!**

---

### Webcam Support
**Feature:** Fetch and display live webcam snapshots  
**Format:** JPEG, PNG  
**Max Size:** 100KB  
**Update Rate:** Configurable (default: 5 seconds)  

**Setup:**
```cpp
#include "ImageFetcher.h"
ImageFetcher fetcher;
fetcher.fetchWebcam("http://192.168.68.91/webcam/?action=snapshot", 20, 20);
```

See [IMAGE_FEATURES.md](IMAGE_FEATURES.md) for details.

---

### Print Thumbnails
**Feature:** Display gcode preview images  
**Source:** Klipper embedded thumbnails  
**Format:** JPEG from slicer  
**Size:** Auto-scaled to fit  

**Slicer Setup:**
- PrusaSlicer: Enable thumbnails in printer settings
- Cura: Install "Cura Thumbnail" plugin

---

## 🎬 Animation System

### GIF Support
**Feature:** Play frame-by-frame animations  
**Format:** Converted to C arrays  
**Memory:** ~2KB per frame (50x50 pixels)  

**Convert GIF:**
```bash
cd tools
source venv/bin/activate
python gif_to_animation.py input.gif output.h
```

See [ANIMATION_README.md](ANIMATION_README.md) for details.

---

## 🎯 Status Messages

### M117 Commands
Your existing Klipper macros with `M117` commands will display on screen!

**Examples:**
```gcode
M117 Heating bed...
M117 Leveling...
M117 Printing!
M117 Complete!
```

**Display:**
- Shows in status bar
- Auto-scrolls if too long
- Updates in real-time

---

## 💾 Memory Usage

**Flash:** 749,204 bytes / 1,310,720 (57.2%)  
**RAM:** 33,032 bytes / 327,680 (10.1%)  
**Free Flash:** ~500KB for custom features  

**Optimization:**
- Removed unused screen modes
- Removed touch zones
- Removed double-tap gesture
- Optimized drawing routines

---

## 🔋 Power Management

**Idle Timeout:** 10 minutes (configurable)  
**Sleep Mode:** Not implemented (always on)  
**Power Consumption:**
- Idle: ~80mA (0.4W)
- Active: ~120mA (0.6W)
- Max: ~150mA (0.75W)

---

## 🎨 Customization

### Add Your Own Screens
Edit `UIManager.cpp`:
```cpp
void UIManager::drawCustomScreen(PrinterStatus& status) {
  display->clear();
  display->setTextColor(display->getThemeColors().text);
  display->drawCenteredText("Custom Screen", 120, 3);
  // Add your custom graphics here
}
```

### Add Custom Gestures
Edit `TouchDriver.cpp`:
```cpp
// In detectGesture()
if (/* your gesture condition */) {
  return TOUCH_GESTURE_CUSTOM;
}
```

### Create Custom Themes
Edit `ThemeManager.cpp`:
```cpp
themes[4] = {
  .background = 0x0000,
  .text = 0xFFFF,
  .primary = 0x07FF,
  // ... more colors
};
```

---

## 📚 Related Documentation

- [HARDWARE.md](HARDWARE.md) - Pinout and wiring
- [KLIPPER_SETUP.md](KLIPPER_SETUP.md) - Printer integration
- [IMAGE_FEATURES.md](IMAGE_FEATURES.md) - Webcam and thumbnails
- [ANIMATION_README.md](ANIMATION_README.md) - GIF animations
- [tools/README.md](tools/README.md) - Image conversion tools

---

## 🎉 Tips & Tricks

1. **Quick Theme Switch:** Tap rapidly to cycle through all themes
2. **Lock Screen:** Swipe left/right to keep current view
3. **Brightness Presets:** Swipe up 3x for max, down 7x for minimum
4. **Easter Egg:** Draw circles during idle for fun animation
5. **Manual Mode:** Great for showing off different screens
6. **Status Messages:** Use M117 in macros for custom messages
7. **Custom Boot Logo:** Make it personal with your own image
8. **Webcam View:** Add live camera feed during prints
9. **Print Preview:** See your model before printing starts
10. **Theme Matching:** Match display theme to your printer's LED colors

---

**Enjoy your PaWe i-print!** 🚀
