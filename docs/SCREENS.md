# Display Screens Reference

This document shows what each screen looks like and when it appears.

## Screen Overview

The Knomi Clone has 7 different screens that automatically switch based on printer state.

---

## 1. Boot Screen

**When shown**: On power-up/reset

**Duration**: 2 seconds

```
┌─────────────────────┐
│                     │
│                     │
│      KNOMI          │
│      Clone          │
│                     │
│       v1.0          │
│                     │
│        ○            │
│                     │
└─────────────────────┘
```

**Elements**:
- Large "KNOMI" text (cyan)
- "Clone" subtitle (white)
- Version number (gray)
- Loading indicator circle (cyan)

---

## 2. Connecting Screen

**When shown**: While connecting to WiFi

**Duration**: Until connected or timeout

```
┌─────────────────────┐
│                     │
│                     │
│   Connecting        │
│   to WiFi...        │
│                     │
│       📶            │
│                     │
│                     │
│                     │
└─────────────────────┘
```

**Elements**:
- "Connecting" text (white)
- "to WiFi..." subtitle (gray)
- WiFi icon (cyan)
- Animated dots

---

## 3. Connected Screen

**When shown**: After successful WiFi connection

**Duration**: 1 second

```
┌─────────────────────┐
│                     │
│                     │
│   Connected!        │
│                     │
│                     │
│       ✓             │
│                     │
│                     │
│                     │
└─────────────────────┘
```

**Elements**:
- "Connected!" text (green)
- Checkmark icon (green)

---

## 4. Idle Screen (Rolling Eyes)

**When shown**: Printer is ready/idle

**Duration**: Continuous until state changes

```
┌─────────────────────┐
│                     │
│                     │
│    ◉      ◉         │
│   👁️     👁️         │
│                     │
│                     │
│      Ready          │
│    210/210          │
│                     │
└─────────────────────┘
```

**Elements**:
- Two animated eyes (white with black pupils)
- Pupils move in circular pattern
- Eyes blink periodically
- "Ready" status text (white)
- Current hotend temp (orange)

**Animation**:
- Pupils rotate 360° every ~6 seconds
- Blink every 3 seconds
- Smooth movement

---

## 5. Printing Screen

**When shown**: Active print job

**Duration**: Throughout print

```
┌─────────────────────┐
│                     │
│      ╱───╲          │
│     │  45% │        │
│     │     │         │
│      ╲───╱          │
│                     │
│   E:210 B:60        │
│   2h 15m left       │
│   test_print.gcode  │
│   Z:5.40            │
└─────────────────────┘
```

**Elements**:
- Circular progress ring (cyan)
  - Background ring (dark gray)
  - Progress arc (cyan)
  - Thickness: 8 pixels
- Progress percentage in center (white, large)
- Temperatures: E=extruder, B=bed (orange)
- Time remaining (gray)
- Filename (cyan, truncated if long)
- Current Z height (gray)

**Updates**:
- Progress ring updates every 1%
- Temperatures update every second
- Time updates every second

---

## 6. Paused Screen

**When shown**: Print is paused

**Duration**: Until resumed or cancelled

```
┌─────────────────────┐
│                     │
│      ╱───╲          │
│     │PAUSED│        │
│     │  45% │        │
│      ╲───╱          │
│                     │
│   E:210 B:60        │
│                     │
│                     │
└─────────────────────┘
```

**Elements**:
- Progress ring (dimmed)
- "PAUSED" text (orange, large)
- Current progress percentage (white)
- Temperatures maintained (orange)

---

## 7. Complete Screen

**When shown**: Print finished successfully

**Duration**: Until next print or reset

```
┌─────────────────────┐
│                     │
│                     │
│       ✓             │
│                     │
│   COMPLETE          │
│                     │
│   2h 34m 12s        │
│                     │
│                     │
└─────────────────────┘
```

**Elements**:
- Large checkmark icon (green)
- "COMPLETE" text (green)
- Total print time (gray)

---

## 8. WiFi Error Screen

**When shown**: Cannot connect to WiFi

**Duration**: Until connection restored

```
┌─────────────────────┐
│                     │
│   WiFi Error        │
│                     │
│   Check config      │
│                     │
│       ✗             │
│                     │
│                     │
│                     │
└─────────────────────┘
```

**Elements**:
- "WiFi Error" text (red)
- "Check config" subtitle (gray)
- Error icon (red X in circle)

**Recovery**:
- Automatically retries connection
- No user intervention needed

---

## 9. Klipper Error Screen

**When shown**: Cannot connect to Klipper/Moonraker

**Duration**: Until connection restored

```
┌─────────────────────┐
│                     │
│    Klipper          │
│    Offline          │
│                     │
│  Reconnecting...    │
│                     │
│       🖨️            │
│                     │
│                     │
└─────────────────────┘
```

**Elements**:
- "Klipper" text (red)
- "Offline" text (red)
- "Reconnecting..." subtitle (gray)
- Printer icon (red)

**Recovery**:
- Automatically retries connection
- Checks every 5 seconds

---

## Screen Transitions

### State Machine

```
Power On
   │
   ▼
Boot Screen (2s)
   │
   ▼
Connecting Screen
   │
   ├─ Success ──► Connected Screen (1s)
   │                    │
   │                    ▼
   │              Test Klipper
   │                    │
   │              ┌─────┴─────┐
   │              │           │
   │           Success     Failure
   │              │           │
   │              ▼           ▼
   │         Idle Screen  Klipper Error
   │              │           │
   │              │      (retry loop)
   │              │           │
   │              └───────────┘
   │                    │
   │                    ▼
   │            ┌───────────────┐
   │            │               │
   │         Idle          Printing
   │            │               │
   │            │          ┌────┴────┐
   │            │          │         │
   │            │       Paused   Complete
   │            │          │         │
   │            └──────────┴─────────┘
   │
   └─ Failure ──► WiFi Error
                     │
                (retry loop)
```

### Transition Rules

1. **Boot → Connecting**: Automatic after 2 seconds
2. **Connecting → Connected**: When WiFi connects
3. **Connecting → WiFi Error**: After 30 seconds timeout
4. **Connected → Idle**: When Klipper responds
5. **Connected → Klipper Error**: When Klipper doesn't respond
6. **Idle → Printing**: When print starts
7. **Printing → Paused**: When print pauses
8. **Printing → Complete**: When print finishes
9. **Paused → Printing**: When print resumes
10. **Any → WiFi Error**: When WiFi disconnects
11. **Any → Klipper Error**: When Klipper disconnects

---

## Color Palette

### Primary Colors
- **Background**: Black (`0x0000`)
- **Text**: White (`0xFFFF`)
- **Accent**: Cyan (`0x07FF`)

### Status Colors
- **Success**: Green (`0x07E0`)
- **Warning**: Orange (`0xFD20`)
- **Error**: Red (`0xF800`)
- **Info**: Cyan (`0x07FF`)

### UI Colors
- **Gray**: Medium Gray (`0x8410`)
- **Dark Gray**: Dark Gray (`0x4208`)
- **Light Gray**: Light Gray (`0xC618`)

### Temperature Colors
- **Hotend**: Orange (`0xFD20`)
- **Bed**: Orange (`0xFD20`)
- **Chamber**: Cyan (`0x07FF`)

---

## Typography

### Font Sizes
- **Size 1**: 8px (small text, details)
- **Size 2**: 16px (normal text, labels)
- **Size 3**: 24px (large text, percentages)

### Text Alignment
- **Centered**: Most text
- **Left**: Long filenames (if needed)
- **Right**: Not used

---

## Layout Guidelines

### Safe Area
- **Outer radius**: 120px (from center)
- **Inner radius**: 100px (text area)
- **Center point**: (120, 120)

### Spacing
- **Line height**: 20px
- **Section gap**: 15px
- **Icon size**: 30x30px

### Positioning
```
Y-coordinates (from top):
  80px  - Title area
 110px  - Main content top
 120px  - Center (for progress ring)
 145px  - Sub-content
 165px  - Details
 190px  - Filename
 210px  - Bottom info
 230px  - Footer
```

---

## Animation Details

### Rolling Eyes (Idle Screen)
- **Frame rate**: 20 FPS (50ms per frame)
- **Rotation speed**: 60° per second
- **Blink duration**: 5 frames (250ms)
- **Blink frequency**: Every 60 frames (3 seconds)

### Progress Ring (Printing Screen)
- **Update frequency**: On progress change (1%)
- **Animation**: Smooth arc drawing
- **Colors**: Dark gray background, cyan progress

### Screen Transitions
- **Fade duration**: Not implemented (instant switch)
- **Future**: Could add fade/slide transitions

---

## Customization

### Changing Colors

Edit `DisplayDriver.h`:
```cpp
#define COLOR_ACCENT      0x07FF  // Change accent color
#define COLOR_WARNING     0xFD20  // Change warning color
#define COLOR_ERROR       0xF800  // Change error color
```

### Changing Layout

Edit `UIManager.cpp`:
```cpp
void UIManager::drawPrintingScreen(PrinterStatus& status) {
  // Modify Y-coordinates for different layout
  display->drawCenteredText(progressStr, 110, 3);  // Change Y position
}
```

### Adding Custom Screens

1. Add screen type to `UIManager.h`:
```cpp
enum ScreenType {
  // ... existing screens
  SCREEN_CUSTOM,
};
```

2. Add drawing function to `UIManager.cpp`:
```cpp
void UIManager::drawCustomScreen() {
  display->clear();
  // Your custom drawing code
}
```

3. Add logic to show screen:
```cpp
if (/* your condition */) {
  currentScreen = SCREEN_CUSTOM;
  drawCustomScreen();
}
```

---

## Testing Screens

### Manual Testing

To test each screen, you can modify the code temporarily:

```cpp
// In KnomiC3.ino setup()
void setup() {
  // ... existing setup code
  
  // Force specific screen for testing
  uiManager.showBootScreen();
  delay(5000);  // Hold for 5 seconds
  
  // Or test error screen
  uiManager.showWiFiError();
  delay(5000);
}
```

### Serial Commands

Future feature: Control screens via serial commands for testing.

---

## Troubleshooting Display Issues

### Screen is blank
- Check backlight connection
- Verify power supply
- Check pin configuration

### Wrong colors
- Check RGB order in `User_Setup.h`
- Try `TFT_RGB` vs `TFT_BGR`

### Text is cut off
- Verify screen dimensions (240x240)
- Check text positioning
- Adjust font size

### Animations are jerky
- Reduce `ANIMATION_SPEED`
- Check CPU usage
- Simplify drawing code

---

## Future Screen Ideas

### Planned Screens
- **Temperature Graph**: Historical temperature data
- **Menu Screen**: Settings and options
- **Statistics Screen**: Print statistics
- **Webcam Screen**: Low-res camera preview

### Community Suggestions
- Multi-printer view
- Filament sensor status
- Power usage graph
- Custom logo screen

---

*For more information, see the main documentation in the `docs/` folder.*
