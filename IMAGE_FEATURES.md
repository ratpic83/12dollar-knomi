# 🖼️ Image Features Guide

## 1. Custom Boot Logos ✅

Convert any image (PNG, JPG, GIF) to a custom boot logo!

### Quick Start:

```bash
cd tools
source venv/bin/activate

# Convert your image (auto-resizes to fit 240x240)
python image_to_header.py your_logo.png ../firmware/src/custom_logo.h

# Or specify custom size
python image_to_header.py your_logo.png ../firmware/src/custom_logo.h 100 100
```

### Use in Boot Screen:

```cpp
// In UIManager.cpp
#include "custom_logo.h"

void UIManager::showBootScreen() {
  display->clear();
  
  // Draw your custom logo
  draw_custom_logo(display->getTFT(), 70, 70);  // Centered at 70,70
  
  // Add text below
  display->setTextColor(display->getThemeColors().text);
  display->drawCenteredText("Loading...", 190, 2);
}
```

### Memory Usage:
- **80x80 image** = ~12.5KB
- **120x120 image** = ~28KB
- **240x240 image** = ~112KB

---

## 2. Live Webcam Thumbnails 🎥

Display live webcam feed from your printer!

### Setup:

1. **Install Image Decoder Libraries:**
```bash
cd firmware
pio lib install "bodmer/TJpg_Decoder"
```

2. **Add to Your Code:**
```cpp
#include "ImageFetcher.h"

ImageFetcher imageFetcher;

void setup() {
  imageFetcher.init(&display);
}

void loop() {
  // Fetch webcam every 5 seconds
  static unsigned long lastFetch = 0;
  if (millis() - lastFetch > 5000) {
    imageFetcher.fetchWebcam("http://192.168.68.91/webcam/?action=snapshot", 20, 20);
    lastFetch = millis();
  }
}
```

### Webcam URLs:

**Mainsail/Fluidd (mjpeg-streamer):**
```
http://YOUR_IP/webcam/?action=snapshot
```

**OctoPrint:**
```
http://YOUR_IP/webcam/?action=snapshot
```

**ESP32-CAM:**
```
http://ESP32_IP/capture
```

---

## 3. Print Preview Thumbnails 📸

Show the model being printed!

### Klipper Setup:

1. **Enable Thumbnails in Slicer:**

**PrusaSlicer/SuperSlicer:**
```gcode
; In Printer Settings > Custom G-code > Start G-code
; Add thumbnail generation
M118 P0 action:notification Thumbnail
```

**Cura:**
Install "Cura Thumbnail" plugin

2. **Fetch and Display:**
```cpp
// When print starts
imageFetcher.fetchPrintPreview("192.168.68.91", "my_model.gcode", 20, 20);
```

### Auto-Display on Print Start:

```cpp
void UIManager::drawPrintingScreen(PrinterStatus& status) {
  // ... existing code ...
  
  // Show print preview in corner
  static bool thumbnailFetched = false;
  if (!thumbnailFetched && status.filename[0] != '\0') {
    imageFetcher.fetchPrintPreview("192.168.68.91", status.filename, 10, 10);
    thumbnailFetched = true;
  }
}
```

---

## 📋 Complete Example: All Features

```cpp
// main.cpp
#include "ImageFetcher.h"

ImageFetcher imageFetcher;
unsigned long lastWebcamUpdate = 0;

void setup() {
  // ... existing setup ...
  imageFetcher.init(&display);
}

void loop() {
  // ... existing loop ...
  
  // Update webcam every 10 seconds when printing
  if (printerStatus.state == STATE_PRINTING) {
    if (millis() - lastWebcamUpdate > 10000) {
      imageFetcher.fetchWebcam("http://192.168.68.91/webcam/?action=snapshot", 140, 140);
      lastWebcamUpdate = millis();
    }
  }
}
```

---

## 🎨 Tips & Tricks:

### Optimize Images:
```bash
# Reduce colors for smaller file size
convert input.png -colors 256 -resize 100x100 output.png

# Convert to indexed color
convert input.png -type Palette -colors 64 output.png
```

### Create Circular Thumbnails:
```bash
# Create circular mask
convert input.png -alpha set -virtual-pixel transparent \
  -channel A -blur 0x1 -level 50%,100% +channel \
  -background none -gravity center -extent 100x100 circular.png
```

### Animated Boot Logo:
Use multiple images and cycle through them:
```cpp
const uint16_t* logos[] = {logo1_data, logo2_data, logo3_data};
int frame = (millis() / 200) % 3;
display->pushImage(70, 70, 100, 100, logos[frame]);
```

---

## ⚠️ Current Limitations:

1. **JPEG/PNG Decoding** - Requires additional libraries (TJpg_Decoder, PNGdec)
2. **Memory** - Large images (>100KB) may cause issues
3. **Speed** - Image fetching takes 1-3 seconds
4. **Format** - Only JPEG and PNG supported

---

## 🚀 Future Enhancements:

- [ ] Cached thumbnails (store in SPIFFS)
- [ ] Progressive JPEG loading
- [ ] Image rotation/scaling
- [ ] Multiple webcam support
- [ ] Timelapse preview
- [ ] QR code generation (share print status)

---

## 📖 Examples:

See `examples/` folder for:
- `custom_boot_logo.cpp` - Custom boot screen
- `webcam_display.cpp` - Live webcam feed
- `print_preview.cpp` - Print thumbnail display
