#!/usr/bin/env python3
"""
Convert GIF animation to C header file with RGB565 format for ESP32 display
"""

from PIL import Image
import sys
import os

def rgb888_to_rgb565(r, g, b):
    """Convert RGB888 to RGB565 format"""
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

def convert_gif_to_header(gif_path, output_path, var_name="spaceman"):
    """Convert GIF to C header file with RGB565 pixel data"""
    
    print(f"Loading GIF: {gif_path}")
    img = Image.open(gif_path)
    
    # Get number of frames
    frame_count = 0
    try:
        while True:
            img.seek(frame_count)
            frame_count += 1
    except EOFError:
        pass
    
    print(f"Found {frame_count} frames")
    
    # Target size
    target_width = 240
    target_height = 240
    
    # Open output file
    with open(output_path, 'w') as f:
        # Write header
        f.write(f"#ifndef {var_name.upper()}_GIF_H\n")
        f.write(f"#define {var_name.upper()}_GIF_H\n\n")
        f.write("#include <Arduino.h>\n\n")
        f.write(f"#define {var_name.upper()}_FRAME_COUNT {frame_count}\n\n")
        
        # Process each frame
        for frame_idx in range(frame_count):
            print(f"Processing frame {frame_idx + 1}/{frame_count}...")
            
            img.seek(frame_idx)
            
            # Convert to RGB and resize to 240x240
            frame = img.convert('RGB')
            frame = frame.resize((target_width, target_height), Image.Resampling.LANCZOS)
            
            # Write frame data
            f.write(f"const uint16_t {var_name}_frame_{frame_idx}[{target_width * target_height}] PROGMEM = {{\n")
            
            pixels = frame.load()
            pixel_count = 0
            
            for y in range(target_height):
                for x in range(target_width):
                    r, g, b = pixels[x, y]
                    rgb565 = rgb888_to_rgb565(r, g, b)
                    
                    if pixel_count % 12 == 0:
                        f.write("  ")
                    
                    f.write(f"0x{rgb565:04X}")
                    
                    if pixel_count < (target_width * target_height - 1):
                        f.write(",")
                    
                    if pixel_count % 12 == 11:
                        f.write("\n")
                    
                    pixel_count += 1
            
            if pixel_count % 12 != 0:
                f.write("\n")
            
            f.write("};\n\n")
        
        # Write frame array pointer
        f.write(f"const uint16_t* {var_name}_frames[{frame_count}] = {{\n")
        for frame_idx in range(frame_count):
            f.write(f"  {var_name}_frame_{frame_idx}")
            if frame_idx < frame_count - 1:
                f.write(",")
            f.write("\n")
        f.write("};\n\n")
        
        f.write(f"#endif // {var_name.upper()}_GIF_H\n")
    
    print(f"✅ Conversion complete! Output: {output_path}")
    print(f"   Frames: {frame_count}")
    print(f"   Size: {target_width}x{target_height}")
    
    # Print file size
    file_size = os.path.getsize(output_path)
    print(f"   File size: {file_size / 1024:.1f} KB")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python gif_to_header.py <input.gif> [output.h] [var_name]")
        sys.exit(1)
    
    input_gif = sys.argv[1]
    output_h = sys.argv[2] if len(sys.argv) > 2 else "animation.h"
    var_name = sys.argv[3] if len(sys.argv) > 3 else "spaceman"
    
    if not os.path.exists(input_gif):
        print(f"❌ Error: File not found: {input_gif}")
        sys.exit(1)
    
    convert_gif_to_header(input_gif, output_h, var_name)
