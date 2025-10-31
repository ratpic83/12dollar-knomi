# 📁 Examples

Example files and resources for PaWe i-print.

## 🎬 Animations

### Spaceman_optimized.gif
**Size:** 76KB (15 frames, 100x100)  
**Use:** Example for GIF conversion  
**Convert:**
```bash
cd ../tools
source venv/bin/activate
python gif_to_animation.py ../examples/animations/Spaceman_optimized.gif ../firmware/src/example_animation.h
```

### Spaceman Finch Sticker by Electric Callboy.gif
**Size:** 2.3MB (60 frames, 480x480)  
**Note:** Too large for ESP32, use optimized version  
**Download:** [Original source](https://giphy.com/electriccallboy)

---

## 🖼️ Custom Boot Logos

Create your own boot logo:

```bash
cd ../tools
source venv/bin/activate

# Convert any image
python image_to_header.py your_logo.png ../firmware/src/custom_logo.h 120 120
```

**Tips:**
- Keep images under 120x120 for reasonable memory usage
- Use simple graphics for best results
- RGB565 format (65K colors)

---

## 📸 Webcam Examples

See [IMAGE_FEATURES.md](../IMAGE_FEATURES.md) for webcam integration examples.

---

## 🎨 Theme Examples

Custom theme in `ThemeManager.cpp`:

```cpp
themes[4] = {
  .background = 0x0000,  // Black
  .text = 0xFFFF,        // White
  .primary = 0x07FF,     // Cyan
  .secondary = 0x8410,   // Gray
  .accent = 0xF81F,      // Magenta
  .success = 0x07E0,     // Green
  .error = 0xF800,       // Red
  .warning = 0xFFE0      // Yellow
};
```

---

## 🔧 Macro Examples

See [klipper_config/pawe_status_addon.cfg](../klipper_config/pawe_status_addon.cfg) for status message examples.

---

**Need more examples?** Check the [documentation](../README.md)!
