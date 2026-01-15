#!/usr/bin/env python3
"""
Convert GIF animation to C header file with RGB565 format for ESP32 display
This version uses smaller resolution (120x120) to fit in flash
"""

from PIL import Image
import sys
import os

def rgb888_to_rgb565(r, g, b):
    """Convert RGB888 to RGB565 format with byte swap for ESP32"""
    rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)
    # Swap bytes for correct display on ESP32/LovyanGFX
    return ((rgb565 & 0xFF) << 8) | ((rgb565 >> 8) & 0xFF)

def convert_gif_to_header(gif_path, output_path, var_name="spaceman", max_frames=5, size=120):
    """Convert GIF to C header file with RGB565 pixel data"""
    
    print(f"Loading GIF: {gif_path}")
    img = Image.open(gif_path)
    
    # Get number of frames
    total_frames = 0
    try:
        while True:
            img.seek(total_frames)
            total_frames += 1
    except EOFError:
        pass
    
    print(f"Found {total_frames} frames in GIF")
    
    # Calculate frame skip to get max_frames
    frame_skip = max(1, total_frames // max_frames)
    selected_frames = list(range(0, total_frames, frame_skip))[:max_frames]
    
    print(f"Selecting {len(selected_frames)} frames: {selected_frames}")
    print(f"Target size: {size}x{size}")
    
    # Open output file
    with open(output_path, 'w') as f:
        # Write header
        f.write(f"#ifndef {var_name.upper()}_GIF_H\n")
        f.write(f"#define {var_name.upper()}_GIF_H\n\n")
        f.write("#include <Arduino.h>\n\n")
        f.write(f"#define {var_name.upper()}_FRAME_COUNT {len(selected_frames)}\n")
        f.write(f"#define {var_name.upper()}_WIDTH {size}\n")
        f.write(f"#define {var_name.upper()}_HEIGHT {size}\n\n")
        
        # Process selected frames
        for idx, frame_num in enumerate(selected_frames):
            print(f"Processing frame {idx + 1}/{len(selected_frames)} (original frame {frame_num})...")
            
            img.seek(frame_num)
            
            # Convert to RGB and resize
            frame = img.convert('RGB')
            frame = frame.resize((size, size), Image.Resampling.LANCZOS)
            
            # Write frame data
            f.write(f"const uint16_t {var_name}_frame_{idx}[{size * size}] PROGMEM = {{\n")
            
            pixels = frame.load()
            pixel_count = 0
            
            for y in range(size):
                for x in range(size):
                    r, g, b = pixels[x, y]
                    rgb565 = rgb888_to_rgb565(r, g, b)
                    
                    if pixel_count % 12 == 0:
                        f.write("  ")
                    
                    f.write(f"0x{rgb565:04X}")
                    
                    if pixel_count < (size * size - 1):
                        f.write(",")
                    
                    if pixel_count % 12 == 11:
                        f.write("\n")
                    
                    pixel_count += 1
            
            if pixel_count % 12 != 0:
                f.write("\n")
            
            f.write("};\n\n")
        
        # Write frame array pointer
        f.write(f"const uint16_t* {var_name}_frames[{len(selected_frames)}] = {{\n")
        for idx in range(len(selected_frames)):
            f.write(f"  {var_name}_frame_{idx}")
            if idx < len(selected_frames) - 1:
                f.write(",")
            f.write("\n")
        f.write("};\n\n")
        
        f.write(f"#endif // {var_name.upper()}_GIF_H\n")
    
    print(f"✅ Conversion complete! Output: {output_path}")
    print(f"   Frames: {len(selected_frames)} (reduced from {total_frames})")
    print(f"   Size: {size}x{size}")
    
    # Print file size
    file_size = os.path.getsize(output_path)
    print(f"   File size: {file_size / 1024:.1f} KB")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python gif_to_header_small.py <input.gif> [output.h] [var_name] [max_frames] [size]")
        sys.exit(1)
    
    input_gif = sys.argv[1]
    output_h = sys.argv[2] if len(sys.argv) > 2 else "animation.h"
    var_name = sys.argv[3] if len(sys.argv) > 3 else "spaceman"
    max_frames = int(sys.argv[4]) if len(sys.argv) > 4 else 5
    size = int(sys.argv[5]) if len(sys.argv) > 5 else 120
    
    if not os.path.exists(input_gif):
        print(f"❌ Error: File not found: {input_gif}")
        sys.exit(1)
    
    convert_gif_to_header(input_gif, output_h, var_name, max_frames, size)
