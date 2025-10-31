#!/usr/bin/env python3
"""
Image to C Header Converter
Converts static images (PNG, JPG, GIF) to C arrays for ESP32 display

Usage: python image_to_header.py input.png output.h [width] [height]
"""

import sys
from PIL import Image
import os

def rgb888_to_rgb565(r, g, b):
    """Convert RGB888 to RGB565 format"""
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

def convert_image_to_header(image_path, output_path, max_width=240, max_height=240):
    """Convert image to C header array"""
    
    # Open image
    img = Image.open(image_path)
    
    # Convert to RGB
    img = img.convert('RGB')
    
    # Resize if needed
    orig_width, orig_height = img.size
    scale = min(max_width / orig_width, max_height / orig_height, 1.0)
    
    if scale < 1.0:
        new_width = int(orig_width * scale)
        new_height = int(orig_height * scale)
        img = img.resize((new_width, new_height), Image.Resampling.LANCZOS)
    
    width, height = img.size
    
    # Get base name for variable
    base_name = os.path.splitext(os.path.basename(image_path))[0]
    base_name = base_name.replace(' ', '_').replace('-', '_').replace('.', '_')
    
    # Start output
    output = []
    output.append(f"// Auto-generated from {os.path.basename(image_path)}")
    output.append(f"// Size: {width}x{height}")
    output.append("")
    output.append(f"#ifndef {base_name.upper()}_H")
    output.append(f"#define {base_name.upper()}_H")
    output.append("")
    output.append("#include <Arduino.h>")
    output.append("")
    
    # Convert pixels to RGB565
    pixels = img.load()
    output.append(f"const uint16_t {base_name}_data[] PROGMEM = {{")
    
    pixel_count = 0
    for y in range(height):
        line = "  "
        for x in range(width):
            r, g, b = pixels[x, y]
            rgb565 = rgb888_to_rgb565(r, g, b)
            line += f"0x{rgb565:04X}, "
            pixel_count += 1
            
            # Break line every 10 pixels for readability
            if pixel_count % 10 == 0:
                output.append(line)
                line = "  "
        
        if line.strip() != "":
            output.append(line)
    
    output.append("};")
    output.append("")
    
    # Add metadata
    output.append(f"const uint16_t {base_name}_width = {width};")
    output.append(f"const uint16_t {base_name}_height = {height};")
    output.append("")
    
    # Add helper function to draw the image
    output.append(f"// Helper function to draw {base_name}")
    output.append(f"// Usage: draw_{base_name}(display, x, y);")
    output.append(f"inline void draw_{base_name}(TFT_eSPI* tft, int16_t x, int16_t y) {{")
    output.append(f"  tft->pushImage(x, y, {base_name}_width, {base_name}_height, {base_name}_data);")
    output.append("}")
    output.append("")
    
    output.append(f"#endif // {base_name.upper()}_H")
    
    # Write output
    with open(output_path, 'w') as f:
        f.write('\n'.join(output))
    
    size_kb = (width * height * 2) / 1024
    print(f"✅ Converted image")
    print(f"   Size: {width}x{height}")
    print(f"   Memory: {size_kb:.1f}KB")
    print(f"   Output: {output_path}")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python image_to_header.py input.png output.h [max_width] [max_height]")
        sys.exit(1)
    
    image_path = sys.argv[1]
    output_path = sys.argv[2]
    max_width = int(sys.argv[3]) if len(sys.argv) > 3 else 240
    max_height = int(sys.argv[4]) if len(sys.argv) > 4 else 240
    
    if not os.path.exists(image_path):
        print(f"Error: {image_path} not found")
        sys.exit(1)
    
    convert_image_to_header(image_path, output_path, max_width, max_height)
