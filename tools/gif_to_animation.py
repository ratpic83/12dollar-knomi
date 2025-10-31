#!/usr/bin/env python3
"""
GIF to Animation Converter
Converts animated GIFs to C arrays for ESP32 display

Usage: python gif_to_animation.py input.gif output.h
"""

import sys
from PIL import Image
import os

def rgb888_to_rgb565(r, g, b):
    """Convert RGB888 to RGB565 format"""
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

def convert_gif_to_animation(gif_path, output_path, max_width=240, max_height=240):
    """Convert GIF to C animation array"""
    
    # Open GIF
    img = Image.open(gif_path)
    
    # Get animation info
    frame_count = 0
    try:
        while True:
            frame_count += 1
            img.seek(frame_count)
    except EOFError:
        pass
    
    img.seek(0)
    
    # Get base name for variable
    base_name = os.path.splitext(os.path.basename(gif_path))[0]
    base_name = base_name.replace(' ', '_').replace('-', '_')
    
    # Start output
    output = []
    output.append(f"// Auto-generated from {os.path.basename(gif_path)}")
    output.append(f"// Frame count: {frame_count}")
    output.append("")
    output.append("#ifndef ANIMATION_DATA_H")
    output.append("#define ANIMATION_DATA_H")
    output.append("")
    output.append('#include "AnimationPlayer.h"')
    output.append("")
    
    # Process each frame
    for frame_idx in range(frame_count):
        img.seek(frame_idx)
        
        # Convert to RGB and resize if needed
        frame = img.convert('RGB')
        orig_width, orig_height = frame.size
        
        # Calculate scaling to fit display
        scale = min(max_width / orig_width, max_height / orig_height, 1.0)
        new_width = int(orig_width * scale)
        new_height = int(orig_height * scale)
        
        if scale < 1.0:
            frame = frame.resize((new_width, new_height), Image.Resampling.LANCZOS)
        
        width, height = frame.size
        
        # Get frame duration (default 100ms if not specified)
        try:
            duration = img.info.get('duration', 100)
        except:
            duration = 100
        
        # Convert pixels to RGB565
        pixels = frame.load()
        output.append(f"// Frame {frame_idx} - {width}x{height}, {duration}ms")
        output.append(f"const uint16_t {base_name}_frame_{frame_idx}_data[] PROGMEM = {{")
        
        for y in range(height):
            line = "  "
            for x in range(width):
                r, g, b = pixels[x, y]
                rgb565 = rgb888_to_rgb565(r, g, b)
                line += f"0x{rgb565:04X}, "
            output.append(line)
        
        output.append("};")
        output.append("")
        
        # Frame structure
        output.append(f"const AnimationFrame {base_name}_frame_{frame_idx} = {{")
        output.append(f"  .data = {base_name}_frame_{frame_idx}_data,")
        output.append(f"  .width = {width},")
        output.append(f"  .height = {height},")
        output.append(f"  .delay_ms = {duration}")
        output.append("};")
        output.append("")
    
    # Create frames array
    output.append(f"const AnimationFrame* {base_name}_frames[] = {{")
    for frame_idx in range(frame_count):
        output.append(f"  &{base_name}_frame_{frame_idx},")
    output.append("};")
    output.append("")
    
    # Create animation structure
    output.append(f"const Animation {base_name}_animation = {{")
    output.append(f"  .frames = {base_name}_frames,")
    output.append(f"  .frameCount = {frame_count},")
    output.append(f"  .loop = true")
    output.append("};")
    output.append("")
    output.append("#endif // ANIMATION_DATA_H")
    
    # Write output
    with open(output_path, 'w') as f:
        f.write('\n'.join(output))
    
    print(f"✅ Converted {frame_count} frames")
    print(f"   Size: {width}x{height}")
    print(f"   Output: {output_path}")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python gif_to_animation.py input.gif output.h")
        sys.exit(1)
    
    gif_path = sys.argv[1]
    output_path = sys.argv[2]
    
    if not os.path.exists(gif_path):
        print(f"Error: {gif_path} not found")
        sys.exit(1)
    
    convert_gif_to_animation(gif_path, output_path)
