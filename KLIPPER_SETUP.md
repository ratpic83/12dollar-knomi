# 🖨️ Klipper Setup for PaWe i-print

## ⚠️ Problem: Printer Won't Boot

The original `knomi.cfg` tries to override macros that may not exist in your printer, causing Klipper to fail.

## ✅ Solution: Use Minimal Config

### Step 1: Remove Old Config

**SSH into your printer:**
```bash
ssh pi@192.168.68.91
```

**Edit your printer.cfg:**
```bash
nano ~/printer_data/config/printer.cfg
```

**Remove or comment out this line:**
```ini
# [include knomi.cfg]  ← Comment this out or delete it
```

**Save and exit:** `Ctrl+X`, then `Y`, then `Enter`

### Step 2: Add Minimal Config (Optional)

The PaWe i-print works **WITHOUT any Klipper config** via the Moonraker API!

But if you want status messages, copy the minimal config:

```bash
# Copy minimal config to your printer
scp klipper_config/knomi_minimal.cfg pi@192.168.68.91:~/printer_data/config/
```

Then in `printer.cfg`:
```ini
[include knomi_minimal.cfg]
```

### Step 3: Restart Klipper

```bash
sudo systemctl restart klipper
```

Or via Mainsail/Fluidd: **Firmware Restart**

---

## 🎯 What Your Display Already Gets (No Config Needed!)

Your PaWe i-print fetches everything via Moonraker API:

✅ **Printer State** - Idle, Printing, Paused, Complete  
✅ **Temperatures** - Hotend, Bed (current & target)  
✅ **Print Progress** - Percentage, time remaining  
✅ **File Name** - Current print job  
✅ **Fan Speed** - Part cooling fan  
✅ **Print Stats** - Total time, filament used  

**No Klipper config required!**

---

## 🔧 Advanced: Custom Status Messages (Optional)

If you want to send custom messages to your display:

### Add to printer.cfg:
```ini
[gcode_macro PAWE_STATUS]
gcode:
  {% set msg = params.MSG|default("Status") %}
  M117 {msg}
  RESPOND MSG="{msg}"
```

### Use in your macros:
```gcode
PAWE_STATUS MSG="Heating bed..."
PAWE_STATUS MSG="Purging nozzle..."
PAWE_STATUS MSG="Starting print!"
```

---

## 🐛 Troubleshooting

### Klipper Won't Start

**Check for errors:**
```bash
tail -f ~/printer_data/logs/klippy.log
```

**Common issues:**
1. **Macro already exists** - Remove duplicate macro definitions
2. **Syntax error** - Check for typos in config
3. **Missing section** - Some macros reference sections that don't exist

**Quick fix:**
```bash
# Remove ALL knomi-related includes
nano ~/printer_data/config/printer.cfg
# Comment out: [include knomi.cfg]
# Save and restart Klipper
```

### Display Not Updating

**Check Moonraker API:**
```bash
curl http://192.168.68.91:7125/printer/objects/query?heater_bed&extruder
```

Should return JSON with temperatures.

**Check display serial output:**
- Look for "Klipper API: Connected"
- Check for HTTP errors

---

## 📋 Recommended: Clean Printer.cfg

Your `printer.cfg` should look like:

```ini
# printer.cfg

# Basic printer configuration
[mcu]
serial: /dev/serial/by-id/your-printer-id

[printer]
kinematics: cartesian
max_velocity: 300
max_accel: 3000

# ... your existing config ...

# Optional: PaWe i-print status messages (ONLY if you want them)
[include knomi_minimal.cfg]
```

---

## 🎉 Summary

**Your PaWe i-print works WITHOUT Klipper config!**

1. ❌ Remove `[include knomi.cfg]` from printer.cfg
2. ✅ Restart Klipper
3. ✅ Your display will work via Moonraker API
4. 🎨 (Optional) Add `knomi_minimal.cfg` for status messages

**That's it!** Your printer will boot normally and the display will show all data.
