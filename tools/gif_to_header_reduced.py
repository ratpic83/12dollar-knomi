#!/usr/bin/env python3
"""
Convert GIF animation to C header file with RGB565 format for ESP32 display
This version reduces frames to fit in ESP32 flash memory
"""

from PIL import Image
import sys
import os

def rgb888_to_rgb565(r, g, b):
    """Convert RGB888 to RGB565 format"""
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

def convert_gif_to_header(gif_path, output_path, var_name="spaceman", max_frames=5):
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
    
    # Target size
    target_width = 240
    target_height = 240
    
    # Open output file
    with open(output_path, 'w') as f:
        # Write header
        f.write(f"#ifndef {var_name.upper()}_GIF_H\n")
        f.write(f"#define {var_name.upper()}_GIF_H\n\n")
        f.write("#include <Arduino.h>\n\n")
        f.write(f"#define {var_name.upper()}_FRAME_COUNT {len(selected_frames)}\n\n")
        
        # Process selected frames
        for idx, frame_num in enumerate(selected_frames):
            print(f"Processing frame {idx + 1}/{len(selected_frames)} (original frame {frame_num})...")
            
            img.seek(frame_num)
            
            # Convert to RGB and resize to 240x240
            frame = img.convert('RGB')
            frame = frame.resize((target_width, target_height), Image.Resampling.LANCZOS)
            
            # Write frame data
            f.write(f"const uint16_t {var_name}_frame_{idx}[{target_width * target_height}] PROGMEM = {{\n")
            
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
    print(f"   Size: {target_width}x{target_height}")
    
    # Print file size
    file_size = os.path.getsize(output_path)
    print(f"   File size: {file_size / 1024:.1f} KB")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python gif_to_header_reduced.py <input.gif> [output.h] [var_name] [max_frames]")
        sys.exit(1)
    
    input_gif = sys.argv[1]
    output_h = sys.argv[2] if len(sys.argv) > 2 else "animation.h"
    var_name = sys.argv[3] if len(sys.argv) > 3 else "spaceman"
    max_frames = int(sys.argv[4]) if len(sys.argv) > 4 else 5
    
    if not os.path.exists(input_gif):
        print(f"❌ Error: File not found: {input_gif}")
        sys.exit(1)
    
    convert_gif_to_header(input_gif, output_h, var_name, max_frames)
