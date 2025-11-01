# 🦾 Xenomorph Toolhead Mount Guide

Guide for mounting the PaWe i-print display on a Xenomorph toolhead.

---

## 📐 Display Specifications

**ESP32-2424S012C Dimensions:**
- **Diameter:** 35mm (round PCB)
- **Thickness:** ~6.6mm total
  - PCB: 1.6mm
  - Display: 3mm
  - USB-C connector: 2mm
- **Display active area:** 28mm diameter (1.28" round LCD)
- **Mounting:** No mounting holes on PCB

---

## 🔍 Xenomorph Toolhead Information

The **Xenomorph** is a toolhead conversion for Voron printers that typically includes:
- Extruder mount
- Hotend mount
- Part cooling ducts
- Optional display mount (for BTT KNOMI)

**Original KNOMI mount location:** Front center of toolhead

---

## 🎯 Mounting Options

### Option 1: BTT KNOMI Mount Adapter

The Xenomorph likely has a mount designed for BTT KNOMI. You'll need an adapter.

**BTT KNOMI dimensions:**
- Diameter: ~40mm
- Mounting: 2x M3 screws on sides

**Adapter needed:**
- Convert KNOMI mount to fit 35mm ESP32 display
- Friction fit or snap-in design
- Access to USB-C port on bottom

### Option 2: Custom Mount Design

**Design requirements:**
- **Diameter:** 35mm inner diameter
- **Depth:** 7-8mm to accommodate PCB + USB-C
- **Retention:** Friction fit or clips (no mounting holes on PCB)
- **Cable access:** Bottom cutout for USB-C cable
- **Viewing angle:** Slight tilt (10-15°) for better visibility

---

## 🛠️ DIY Mount Design (FreeCAD/Fusion360)

### Base Measurements

```
Outer ring: 40mm diameter (match KNOMI mount)
Inner ring: 35.5mm diameter (snug fit for display)
Depth: 8mm
Wall thickness: 2.5mm
Bottom cutout: 12mm x 5mm (USB-C access)
Mounting holes: 2x M3 (match Xenomorph KNOMI mount)
```

### Features to Include

1. **Friction fit ring** - Holds display securely
2. **Cable channel** - Routes USB cable to toolhead
3. **Viewing angle** - 10-15° tilt
4. **Snap clips** - Easy removal for maintenance
5. **Ventilation** - Small slots for heat dissipation

---

## 📦 Where to Find Xenomorph Files

### Official Sources

1. **Voron Design GitHub**
   - https://github.com/VoronDesign/VoronUsers
   - Look for Xenomorph in `/printer_mods/`

2. **Printables/Thingiverse**
   - Search: "Xenomorph toolhead"
   - Search: "Xenomorph KNOMI mount"

3. **Voron Discord**
   - #voron_user_mods channel
   - Ask for Xenomorph + KNOMI mount files

---

## 🔧 Modification Steps

### 1. Find Original KNOMI Mount

```bash
# Example file structure
Xenomorph/
├── toolhead_main.stl
├── knomi_mount.stl          ← This is what you need
├── cable_cover.stl
└── assembly_guide.pdf
```

### 2. Measure KNOMI Mount

- Import `knomi_mount.stl` into CAD software
- Measure mounting hole spacing
- Measure inner diameter
- Note attachment method (screws, clips, etc.)

### 3. Design Adapter

**Option A: Modify existing mount**
```
1. Open knomi_mount.stl in FreeCAD/Fusion360
2. Reduce inner diameter from 40mm to 35.5mm
3. Adjust depth to 8mm
4. Add USB-C cutout at bottom
5. Export as STL
```

**Option B: Create insert ring**
```
1. Design ring: 40mm OD, 35.5mm ID, 8mm height
2. Add tabs to clip into KNOMI mount
3. Add USB-C cutout
4. Print in flexible filament (TPU) for friction fit
```

---

## 🖨️ 3D Printing Recommendations

### Material
- **PLA/PETG:** Rigid mount
- **TPU:** Flexible insert (better grip)
- **ABS:** Heat resistant (if near hotend)

### Settings
```
Layer height: 0.2mm
Infill: 30-50%
Walls: 3-4 perimeters
Supports: Minimal (design for no supports)
Orientation: Print upright for strength
```

---

## 🔌 Wiring Considerations

### Cable Routing

1. **USB-C cable** exits bottom of display
2. Route along toolhead to umbilical
3. Use cable clips to secure
4. Leave slack for toolhead movement

### Cable Length

- **Minimum:** 50cm (for cable chain)
- **Recommended:** 1m (with slack)
- **Type:** USB-C to USB-A (for Raspberry Pi)

### Power Options

**Option 1: USB from Pi**
- Simple, no extra wiring
- 5V/500mA from USB port
- Plug into Pi USB port

**Option 2: Dedicated 5V supply**
- More reliable power
- Connect to printer 5V rail
- Use buck converter if needed

---

## 📏 Quick Adapter Design

### Simple Friction-Fit Ring

**FreeCAD/OpenSCAD code:**

```openscad
// ESP32-2424S012C adapter ring
difference() {
    // Outer ring (matches KNOMI mount)
    cylinder(h=8, d=40, $fn=100);
    
    // Inner ring (holds display)
    translate([0, 0, -1])
    cylinder(h=10, d=35.5, $fn=100);
    
    // USB-C cutout
    translate([-6, -20, -1])
    cube([12, 10, 10]);
}

// Mounting tabs (if needed)
for (angle = [0, 180]) {
    rotate([0, 0, angle])
    translate([18, 0, 4])
    cube([4, 2, 8], center=true);
}
```

---

## 🎨 Visual Design Ideas

### LED Ring Integration
- Add WS2812B LED ring around display
- 16-24 LEDs in 40mm ring
- Status indication (heating, printing, etc.)

### Protective Cover
- Clear acrylic or PETG cover
- Protects display from debris
- Snap-on design

---

## 📋 Assembly Checklist

- [ ] Print adapter/mount
- [ ] Test fit display in mount
- [ ] Check USB-C cable access
- [ ] Verify viewing angle
- [ ] Install on Xenomorph toolhead
- [ ] Route USB cable to Pi
- [ ] Secure cable with clips
- [ ] Test display visibility during print
- [ ] Check for interference with parts

---

## 🔍 Finding Xenomorph Files

### Search Terms
- "Xenomorph toolhead STL"
- "Xenomorph KNOMI mount"
- "Voron Xenomorph mod"
- "Xenomorph display mount"

### Communities
- **Voron Discord:** https://discord.gg/voron
- **Reddit:** r/voroncorexy
- **Voron Forum:** https://forum.vorondesign.com/

---

## 💡 Alternative Mounting Locations

If Xenomorph doesn't have a KNOMI mount:

1. **Side mount** - Mount on toolhead side
2. **Top mount** - Above extruder
3. **Gantry mount** - On X-axis gantry (not toolhead)
4. **Frame mount** - Static mount on printer frame

---

## 📸 Documentation

**Please share your mount design!**

If you create a mount, consider:
- Taking photos of the installation
- Uploading STL to Printables/Thingiverse
- Sharing in Voron Discord
- Contributing to this repo

---

## 🔗 Useful Links

- **Voron Design:** https://vorondesign.com/
- **VoronUsers Mods:** https://github.com/VoronDesign/VoronUsers
- **BTT KNOMI:** https://github.com/bigtreetech/KNOMI
- **This Project:** https://github.com/ratpic83/12dollar-knomi

---

## 🆘 Need Help?

**Can't find Xenomorph files?**
1. Ask in Voron Discord #voron_user_mods
2. Check VoronUsers GitHub repo
3. Search Printables.com for "Xenomorph"

**Need custom mount designed?**
1. Measure your toolhead
2. Take photos from multiple angles
3. Post in Voron Discord with measurements
4. Community members can help design

---

**Happy mounting!** 🦾🖥️
