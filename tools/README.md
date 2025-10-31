# Image & Animation Tools

## 1. Static Image Converter

Convert any image (PNG, JPG, GIF) to C header for custom boot logos!

### Usage:

```bash
python image_to_header.py input.png output.h [max_width] [max_height]
```

### Example:

```bash
# Convert logo (auto-resize to 240x240)
python image_to_header.py my_logo.png ../firmware/src/my_logo.h

# Convert with custom size
python image_to_header.py icon.png ../firmware/src/icon.h 80 80
```

### Use in Code:

```cpp
#include "my_logo.h"

// Draw the image
draw_my_logo(display->getTFT(), 70, 70);  // x, y position
```

---

## 2. GIF to Animation Converter

Convert animated GIFs to C arrays for your Knomi Clone display.

### Requirements

```bash
pip install Pillow
```

### Usage

1. **Convert your GIF:**
```bash
python gif_to_animation.py "Spaceman Finch Sticker by Electric Callboy.gif" ../firmware/src/spaceman_animation.h
```

2. **Include in your firmware:**
```cpp
#include "spaceman_animation.h"
#include "AnimationPlayer.h"

AnimationPlayer animPlayer;

void setup() {
  // ... your setup code ...
  animPlayer.init(&display);
}

void loop() {
  // Play animation at center of screen
  if (!animPlayer.isPlaying()) {
    animPlayer.play(&spaceman_animation, 60, 60);
  }
  
  animPlayer.update();
}
```

### Tips

- **Keep GIFs small:** The ESP32 has limited memory. Aim for:
  - Max 100x100 pixels
  - Max 10-20 frames
  - Simple colors (reduces file size)

- **Optimize your GIF:**
  - Reduce colors to 16-256
  - Lower frame rate (10-15 FPS is fine)
  - Crop unnecessary parts

- **Memory usage:**
  - Each pixel = 2 bytes (RGB565)
  - 100x100 frame = 20KB
  - 10 frames = 200KB total

### Example: Convert Electric Callboy GIF

```bash
cd /Users/patricweidermann/Documents/dev/knomi_clone/tools

# Convert (will auto-resize to fit 240x240 display)
python gif_to_animation.py "../Spaceman Finch Sticker by Electric Callboy.gif" ../firmware/src/spaceman_animation.h
```

Then use it as an easter egg or boot animation!
