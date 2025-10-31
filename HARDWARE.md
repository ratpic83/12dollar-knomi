# 🔌 Hardware & Pinout Guide

## ESP32-2424S012C Board Specifications

### Board Overview
- **MCU:** ESP32-C3-MINI-1 (RISC-V, 160MHz)
- **RAM:** 400KB SRAM
- **Flash:** 4MB
- **Display:** 1.28" Round IPS (240x240)
- **Touch:** CST816D Capacitive
- **Power:** USB-C 5V/1A
- **Size:** 35mm diameter

---

## 📍 Complete Pinout

### Display (GC9A01)
| Function | GPIO | Notes |
|----------|------|-------|
| **MOSI** | GPIO7 | SPI Data Out |
| **SCK** | GPIO6 | SPI Clock |
| **CS** | GPIO10 | Chip Select |
| **DC** | GPIO2 | Data/Command |
| **RST** | GPIO-1 | Reset (not used) |
| **BL** | GPIO3 | Backlight (PWM) |

**SPI Configuration:**
```cpp
#define TFT_MOSI 7
#define TFT_SCLK 6
#define TFT_CS   10
#define TFT_DC   2
#define TFT_RST  -1
#define TFT_BL   3
```

---

### Touch Controller (CST816D)
| Function | GPIO | Notes |
|----------|------|-------|
| **SDA** | GPIO4 | I2C Data |
| **SCL** | GPIO5 | I2C Clock |
| **RST** | GPIO1 | Reset |
| **INT** | GPIO0 | Interrupt (optional) |

**I2C Configuration:**
```cpp
#define TOUCH_SDA 4
#define TOUCH_SCL 5
#define TOUCH_RST 1
#define TOUCH_INT 0
#define TOUCH_I2C_ADDR 0x15  // CST816D address
```

**Alternative Touch IC (FT6236):**
- Some boards use FT6236 at address `0x38`
- Firmware auto-detects both controllers

---

### Power & USB
| Function | Pin | Notes |
|----------|-----|-------|
| **USB-C** | - | 5V input, data lines for programming |
| **5V** | VIN | Board power input |
| **3.3V** | 3V3 | Regulated output (max 500mA) |
| **GND** | GND | Ground |

---

### Available GPIOs (Expansion)
| GPIO | Function | Notes |
|------|----------|-------|
| **GPIO8** | Free | Available for sensors/buttons |
| **GPIO9** | Free | Available for sensors/buttons |

**Example Uses:**
- GPIO9: Theme button (currently in code)
- GPIO8: External LED strip
- I2C bus: BME280/DHT sensors (share with touch)

---

## 🔧 Hardware Modifications

### Adding a Physical Button
Connect a button between GPIO9 and GND for theme switching:

```
GPIO9 ----[Button]---- GND
```

**Code:**
```cpp
pinMode(9, INPUT_PULLUP);
if (digitalRead(9) == LOW) {
  display.nextTheme();
}
```

### Adding Environmental Sensor
Connect BME280/SHT30 to I2C bus (shares with touch):

```
SDA (GPIO4) ----[Sensor SDA]
SCL (GPIO5) ----[Sensor SCL]
3.3V ----[Sensor VCC]
GND ----[Sensor GND]
```

**Supported Sensors:**
- BME280 (temp, humidity, pressure) - 0x76 or 0x77
- SHT30 (temp, humidity) - 0x44 or 0x45
- DHT11/DHT22 (digital pin, not I2C)

---

## ⚡ Power Specifications

### Power Consumption
| Mode | Current | Power |
|------|---------|-------|
| **Idle** | ~80mA | ~0.4W |
| **Active** | ~120mA | ~0.6W |
| **Max Brightness** | ~150mA | ~0.75W |

### Power Supply Requirements
- **Voltage:** 5V ±5%
- **Current:** Minimum 500mA recommended
- **USB-C:** Standard USB 2.0 power delivery

### Backlight Control
- **PWM Channel:** 0
- **Frequency:** 5kHz
- **Resolution:** 8-bit (0-255)
- **GPIO:** 3
- **Default:** 200/255 (~78%)

```cpp
ledcSetup(0, 5000, 8);      // Channel 0, 5kHz, 8-bit
ledcAttachPin(3, 0);         // GPIO3 to channel 0
ledcWrite(0, brightness);    // Set brightness 0-255
```

---

## 🖥️ Display Specifications

### GC9A01 Driver
- **Resolution:** 240x240 pixels
- **Color Depth:** 16-bit RGB565 (65K colors)
- **Interface:** SPI (4-wire)
- **Max SPI Speed:** 40MHz (configured at 40MHz)
- **Viewing Angle:** 360° (IPS)
- **Brightness:** 300 cd/m²

### Display Memory
- **Frame Buffer:** Not used (direct drawing)
- **Memory per frame:** 240×240×2 = 115,200 bytes
- **Sprites:** Drawn on-demand to save RAM

---

## 🤚 Touch Specifications

### CST816D Controller
- **Type:** Capacitive touch
- **Interface:** I2C
- **Address:** 0x15
- **Max Touch Points:** 1
- **Scan Rate:** ~60Hz
- **Gestures:** Swipe, tap detection in firmware

### Touch Calibration
No calibration needed! Touch coordinates are:
- **X Range:** 0-240
- **Y Range:** 0-240
- **Origin:** Top-left corner

---

## 🔌 Wiring Diagram

```
ESP32-2424S012C Board Layout:
                    
         ┌─────────────┐
         │   USB-C     │
         └──────┬──────┘
                │
    ┌───────────┴───────────┐
    │                       │
    │    ESP32-C3-MINI-1    │
    │                       │
    │  ┌─────────────────┐  │
    │  │                 │  │
    │  │   GC9A01 LCD    │  │
    │  │   240x240 IPS   │  │
    │  │                 │  │
    │  │   CST816D Touch │  │
    │  └─────────────────┘  │
    │                       │
    │  GPIO Pinout:         │
    │  ┌─┐ ┌─┐             │
    │  │4│ │5│ I2C (Touch) │
    │  └─┘ └─┘             │
    │  ┌─┐ ┌─┐             │
    │  │7│ │6│ SPI (LCD)   │
    │  └─┘ └─┘             │
    │  ┌──┐ ┌──┐           │
    │  │10│ │2 │ CS/DC     │
    │  └──┘ └──┘           │
    │  ┌─┐                 │
    │  │3│ Backlight PWM   │
    │  └─┘                 │
    └───────────────────────┘
```

---

## 🛠️ Troubleshooting

### Display Not Working
1. **Check SPI pins** - Verify TFT_eSPI User_Setup.h
2. **Check power** - Ensure 5V/500mA supply
3. **Check brightness** - GPIO3 PWM must be active

### Touch Not Working
1. **Check I2C address** - Try 0x15 (CST816D) or 0x38 (FT6236)
2. **Check wiring** - SDA=4, SCL=5
3. **Check reset** - GPIO1 must pulse on init

### No WiFi Connection
1. **Check antenna** - Built-in PCB antenna
2. **Check 2.4GHz** - ESP32-C3 doesn't support 5GHz
3. **Check credentials** - Verify SSID/password in WifiConfig.h

---

## 📏 Mechanical Dimensions

```
Top View (mm):
     ┌─────────────┐
     │   USB-C     │
     └─────┬───────┘
           │
    ╔══════╧══════╗
    ║             ║
    ║   ┌─────┐   ║  35mm
    ║   │ LCD │   ║  diameter
    ║   └─────┘   ║
    ║             ║
    ╚═════════════╝

Side View (mm):
    ┌─────────────┐
    │   USB-C     │ 2mm
    ├─────────────┤
    │   PCB       │ 1.6mm
    ├─────────────┤
    │   Display   │ 3mm
    └─────────────┘
    Total: ~6.6mm
```

---

## 🔗 Resources

- **Datasheet:** [ESP32-C3 Technical Reference](https://www.espressif.com/sites/default/files/documentation/esp32-c3_technical_reference_manual_en.pdf)
- **Display Driver:** [GC9A01 Datasheet](https://www.buydisplay.com/download/ic/GC9A01A.pdf)
- **Touch IC:** [CST816D Datasheet](https://github.com/fbiego/CST816S)
- **Purchase:** [AliExpress Link](https://de.aliexpress.com/item/1005005453515690.html)

---

## ⚠️ Important Notes

1. **3.3V Logic** - All GPIOs are 3.3V, NOT 5V tolerant
2. **USB-C** - Data lines used for programming only
3. **I2C Shared** - Touch and sensors share same I2C bus
4. **SPI Speed** - 40MHz works reliably, higher may cause glitches
5. **PWM Frequency** - 5kHz for backlight prevents visible flicker
