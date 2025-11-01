# 🎬 BTT KNOMI Animations Integration

This guide shows how to use the original BTT KNOMI animations with your PaWe i-print display.

---

## 📦 Downloaded Animations

The following animations from BTT KNOMI are now in `assets/btt_animations/`:

| Animation | Size | Use Case |
|-----------|------|----------|
| **gif_standby** | 29KB | Idle/standby screen |
| **gif_heated** | 8.6KB | Heating bed/nozzle |
| **gif_homing** | 130KB | Homing axes |
| **gif_probing** | 30KB | Bed leveling/probing |
| **gif_print** | 47KB | Active printing |
| **gif_printed** | 84KB | Print complete |
| **gif_qgling** | 87KB | QGL (Quad Gantry Level) |

---

## 🔧 Converting Animations

### 1. Setup Python Environment

```bash
cd tools
python3 -m venv venv
source venv/bin/activate
pip install Pillow
```

### 2. Convert GIFs to C Arrays

```bash
# Standby animation (idle screen)
python gif_to_animation.py ../assets/btt_animations/gif_standby/gif_standby.gif ../firmware/src/anim_standby.h

# Heating animation
python gif_to_animation.py ../assets/btt_animations/gif_heated/gif_heated.gif ../firmware/src/anim_heated.h

# Homing animation (WARNING: 130KB - too large!)
# python gif_to_animation.py ../assets/btt_animations/gif_homing/gif_homing.gif ../firmware/src/anim_homing.h

# Probing animation
python gif_to_animation.py ../assets/btt_animations/gif_probing/gif_probing.gif ../firmware/src/anim_probing.h

# Printing animation
python gif_to_animation.py ../assets/btt_animations/gif_print/gif_print.gif ../firmware/src/anim_print.h

# Print complete animation
python gif_to_animation.py ../assets/btt_animations/gif_printed/gif_printed.gif ../firmware/src/anim_complete.h

# QGL animation
python gif_to_animation.py ../assets/btt_animations/gif_qgling/gif_qgling.gif ../firmware/src/anim_qgl.h
```

---

## ⚠️ Memory Considerations

**ESP32-C3 has limited memory!**

- **Total Flash:** 1.3MB (57% used = ~500KB free)
- **Each animation frame:** ~115KB for 240x240 pixels
- **Recommendation:** Use smaller animations or reduce frame count

### Optimize Large Animations

```bash
# Reduce size and frame count
ffmpeg -i gif_homing.gif -vf "scale=120:120,fps=10" -frames:v 10 gif_homing_small.gif

# Then convert
python gif_to_animation.py gif_homing_small.gif ../firmware/src/anim_homing.h
```

---

## 🎨 Integration into Firmware

### 1. Include Animation Headers

Edit `firmware/src/UIManager.cpp`:

```cpp
#include "anim_standby.h"
#include "anim_heated.h"
#include "anim_probing.h"
#include "anim_print.h"
#include "anim_complete.h"
```

### 2. Play Animations

```cpp
// In drawIdleScreen()
animPlayer.play(&anim_standby, 60, 60);  // Center position

// In drawPrintingScreen()
animPlayer.play(&anim_print, 60, 60);

// In drawCompleteScreen()
animPlayer.play(&anim_complete, 60, 60);

// When homing detected
if (status.homing) {
  animPlayer.play(&anim_probing, 60, 60);
}

// When heating
if (status.hotendTemp < status.hotendTarget - 5) {
  animPlayer.play(&anim_heated, 60, 60);
}
```

### 3. Update Animation Player

In `loop()`:
```cpp
animPlayer.update();
```

---

## 📋 Recommended Animations to Use

### ✅ Small & Efficient (Use These)
- **gif_heated** (8.6KB) - Perfect for heating status
- **gif_standby** (29KB) - Good for idle screen
- **gif_probing** (30KB) - Bed leveling indicator

### ⚠️ Medium Size (Use Selectively)
- **gif_print** (47KB) - Printing animation
- **gif_printed** (84KB) - Completion celebration
- **gif_qgling** (87KB) - QGL process

### ❌ Too Large (Optimize First)
- **gif_homing** (130KB) - Reduce frames/size before using

---

## 🎯 Example: Replace Eyes with Standby Animation

Edit `firmware/src/UIManager.cpp`:

```cpp
// Remove old eye animation code
// Replace with:

#include "anim_standby.h"

void UIManager::drawIdleScreen(PrinterStatus& status) {
  if (shouldRedraw(status)) {
    display->clear();
  }
  
  // Play standby animation
  if (!animPlayer.isPlaying()) {
    animPlayer.play(&anim_standby, 60, 60);
  }
  animPlayer.update();
  
  // Draw temps
  char tempStr[32];
  sprintf(tempStr, "%.1f°C", status.hotendTemp);
  display->drawCenteredText(tempStr, 200, 2);
  
  lastStatus = status;
}
```

---

## 🔄 Animation States Map

| Printer State | Animation | Trigger |
|---------------|-----------|---------|
| **Idle** | gif_standby | `STATE_IDLE` |
| **Heating** | gif_heated | Temp < Target - 5°C |
| **Homing** | gif_probing | `status.homing == true` |
| **Leveling** | gif_probing | `status.leveling == true` |
| **QGL** | gif_qgling | Custom macro detection |
| **Printing** | gif_print | `STATE_PRINTING` |
| **Complete** | gif_printed | `STATE_COMPLETE` |

---

## 🚀 Quick Start

**1. Convert standby animation:**
```bash
cd tools
source venv/bin/activate
python gif_to_animation.py ../assets/btt_animations/gif_standby/gif_standby.gif ../firmware/src/anim_standby.h
```

**2. Add to UIManager.cpp:**
```cpp
#include "anim_standby.h"

// In drawIdleScreen():
animPlayer.play(&anim_standby, 60, 60);
```

**3. Build and upload:**
```bash
cd ../firmware
pio run -t upload
```

---

## 📊 Memory Budget

Current usage: **749KB / 1310KB (57%)**  
Available: **~500KB**

**Animation sizes (after conversion):**
- 10 frames @ 120x120 = ~30KB
- 20 frames @ 120x120 = ~60KB
- 30 frames @ 240x240 = ~350KB ⚠️

**Recommendation:** Use 120x120 animations with 10-20 frames each.

---

## 🎨 Custom Animation Tips

1. **Reduce resolution:** 120x120 instead of 240x240
2. **Limit frames:** 10-15 frames is enough for smooth animation
3. **Optimize colors:** Use fewer colors to reduce size
4. **Test memory:** Check flash usage after each animation added

---

## 🔗 Resources

- **Original BTT KNOMI:** https://github.com/bigtreetech/KNOMI
- **Animation Player:** `firmware/src/AnimationPlayer.cpp`
- **Conversion Tool:** `tools/gif_to_animation.py`
- **Examples:** `examples/animations/`

---

**Ready to animate your display!** 🎬✨
