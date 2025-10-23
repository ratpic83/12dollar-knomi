# Architecture Documentation

## System Overview

```
┌─────────────────────────────────────────────────────────────┐
│                     Knomi Clone System                       │
└─────────────────────────────────────────────────────────────┘

┌──────────────┐         WiFi          ┌──────────────┐
│   ESP32-C3   │◄─────────────────────►│   Klipper    │
│   Display    │    HTTP/JSON API      │   Moonraker  │
└──────────────┘                        └──────────────┘
      │                                        │
      │                                        │
   GC9A01                                  Printer
   Display                                 Hardware
```

## Component Architecture

### Hardware Layer
```
┌─────────────────────────────────────────────────┐
│              ESP32-C3 Module                     │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐     │
│  │   CPU    │  │   WiFi   │  │   SPI    │     │
│  │ 160MHz   │  │ 2.4GHz   │  │  40MHz   │     │
│  │ 1 Core   │  │          │  │          │     │
│  └──────────┘  └──────────┘  └──────────┘     │
│                                                  │
│  ┌──────────────────────────────────────┐      │
│  │         Memory                        │      │
│  │  RAM: 400KB  |  Flash: 4MB           │      │
│  └──────────────────────────────────────┘      │
└─────────────────────────────────────────────────┘
                    │
                    │ SPI Bus
                    ▼
┌─────────────────────────────────────────────────┐
│            GC9A01 Display Driver                 │
│                                                  │
│  ┌────────────────────────────────────────┐    │
│  │      1.28" Round LCD                    │    │
│  │      240x240 pixels                     │    │
│  │      RGB565 color                       │    │
│  └────────────────────────────────────────┘    │
└─────────────────────────────────────────────────┘
```

### Software Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    Application Layer                         │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐     │
│  │  KnomiC3.ino │  │  UIManager   │  │ KlipperAPI   │     │
│  │              │  │              │  │              │     │
│  │ - Main loop  │  │ - Screens    │  │ - HTTP       │     │
│  │ - Setup      │  │ - Animations │  │ - JSON parse │     │
│  │ - State mgmt │  │ - State mgmt │  │ - Data fetch │     │
│  └──────────────┘  └──────────────┘  └──────────────┘     │
│         │                  │                  │             │
│         └──────────────────┴──────────────────┘             │
│                            │                                │
├────────────────────────────┼────────────────────────────────┤
│                    Driver Layer                              │
├────────────────────────────┼────────────────────────────────┤
│                            │                                │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐     │
│  │DisplayDriver │  │   WiFi       │  │ HTTPClient   │     │
│  │              │  │              │  │              │     │
│  │ - Graphics   │  │ - Connect    │  │ - GET/POST   │     │
│  │ - Text       │  │ - Reconnect  │  │ - Timeout    │     │
│  │ - Shapes     │  │ - Status     │  │ - Error hdl  │     │
│  └──────────────┘  └──────────────┘  └──────────────┘     │
│         │                  │                  │             │
├─────────┼──────────────────┼──────────────────┼─────────────┤
│         │      Hardware Abstraction Layer     │             │
├─────────┼──────────────────┼──────────────────┼─────────────┤
│         │                  │                  │             │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐     │
│  │  TFT_eSPI    │  │ ESP32 WiFi   │  │  ArduinoJson │     │
│  │   Library    │  │   Library    │  │   Library    │     │
│  └──────────────┘  └──────────────┘  └──────────────┘     │
│         │                  │                  │             │
├─────────┼──────────────────┼──────────────────┼─────────────┤
│                    Arduino Framework                         │
├─────────────────────────────────────────────────────────────┤
│                    ESP32-C3 SDK                              │
└─────────────────────────────────────────────────────────────┘
```

## Data Flow

### Startup Sequence

```
1. Power On
   │
   ▼
2. Initialize Hardware
   ├─ Display init
   ├─ WiFi init
   └─ Serial init
   │
   ▼
3. Show Boot Screen
   │
   ▼
4. Connect to WiFi
   │
   ├─ Success ──► Show Connected Screen
   │
   └─ Failure ──► Show WiFi Error
   │
   ▼
5. Test Klipper Connection
   │
   ├─ Success ──► Enter Main Loop
   │
   └─ Failure ──► Show Klipper Error
   │
   ▼
6. Main Loop (Running)
```

### Main Loop Flow

```
┌─────────────────────────────────────────┐
│          Main Loop (1Hz)                 │
└─────────────────────────────────────────┘
              │
              ▼
┌─────────────────────────────────────────┐
│      Check WiFi Connection               │
│  Connected? ──No──► Reconnect            │
│      │                                    │
│     Yes                                   │
└──────┼──────────────────────────────────┘
       │
       ▼
┌─────────────────────────────────────────┐
│    Fetch Printer Status from API         │
│  ┌────────────────────────────────┐     │
│  │ GET /printer/objects/query     │     │
│  │ - heater_bed                   │     │
│  │ - extruder                     │     │
│  │ - print_stats                  │     │
│  │ - gcode_move                   │     │
│  │ - fan                          │     │
│  │ - display_status               │     │
│  └────────────────────────────────┘     │
└──────┼──────────────────────────────────┘
       │
       ▼
┌─────────────────────────────────────────┐
│      Parse JSON Response                 │
│  ┌────────────────────────────────┐     │
│  │ Extract temperatures           │     │
│  │ Extract print progress         │     │
│  │ Extract position               │     │
│  │ Extract state                  │     │
│  └────────────────────────────────┘     │
└──────┼──────────────────────────────────┘
       │
       ▼
┌─────────────────────────────────────────┐
│      Update UI Manager                   │
│  ┌────────────────────────────────┐     │
│  │ Determine screen to show       │     │
│  │ Update display data            │     │
│  │ Trigger redraw if needed       │     │
│  └────────────────────────────────┘     │
└──────┼──────────────────────────────────┘
       │
       ▼
┌─────────────────────────────────────────┐
│      Render Display                      │
│  ┌────────────────────────────────┐     │
│  │ Clear screen (if needed)       │     │
│  │ Draw background                │     │
│  │ Draw UI elements               │     │
│  │ Draw text                      │     │
│  │ Update animations              │     │
│  └────────────────────────────────┘     │
└──────┼──────────────────────────────────┘
       │
       ▼
┌─────────────────────────────────────────┐
│      Wait for Next Cycle                 │
│  (1000ms - elapsed time)                 │
└─────────────────────────────────────────┘
       │
       └──────► Back to Check WiFi
```

## Module Descriptions

### main.cpp (Main Application)
**Purpose**: Application entry point and main loop

**Responsibilities**:
- Initialize all subsystems
- Manage WiFi connection
- Coordinate data fetching
- Handle timing and updates
- Error recovery

**Key Functions**:
- `setup()` - Initialize hardware and software
- `loop()` - Main execution loop
- `connectWiFi()` - Establish WiFi connection
- `updateKlipperData()` - Fetch and process printer data

### UIManager (User Interface)
**Purpose**: Manage display screens and animations

**Responsibilities**:
- Screen state management
- UI rendering
- Animation updates
- Screen transitions
- Data presentation

**Key Functions**:
- `updateStatus()` - Update display with new data
- `showBootScreen()` - Display boot animation
- `drawPrintingScreen()` - Render printing UI
- `drawIdleScreen()` - Render idle UI with eyes
- `update()` - Update animations

**Screens**:
- Boot screen
- Connecting screen
- Idle screen (rolling eyes)
- Printing screen (progress ring)
- Paused screen
- Complete screen
- Error screens

### DisplayDriver (Graphics Engine)
**Purpose**: Low-level display control

**Responsibilities**:
- Hardware initialization
- Graphics primitives
- Text rendering
- Shape drawing
- Color management

**Key Functions**:
- `init()` - Initialize display hardware
- `drawCircle()` - Draw circular shapes
- `drawProgressRing()` - Draw progress indicator
- `drawCenteredText()` - Text alignment
- `drawEye()` - Draw animated eyes
- `clear()` - Clear screen

### KlipperAPI (Communication)
**Purpose**: Interface with Klipper/Moonraker

**Responsibilities**:
- HTTP communication
- JSON parsing
- Data extraction
- Error handling
- Connection management

**Key Functions**:
- `getPrinterStatus()` - Fetch all printer data
- `makeRequest()` - HTTP GET request
- `parseState()` - Convert state strings
- `testConnection()` - Verify API access

**Data Structures**:
```cpp
struct PrinterStatus {
    bool connected;
    PrinterState state;
    float hotendTemp, hotendTarget;
    float bedTemp, bedTarget;
    uint8_t printProgress;
    String fileName;
    // ... more fields
};
```

## Memory Management

### Memory Layout (ESP32-C3)
```
┌─────────────────────────────────────┐
│         Flash (4MB)                  │
├─────────────────────────────────────┤
│  Bootloader        │ 0x0000 - 0x8FFF │
│  Partition Table   │ 0x9000 - 0xDFFF │
│  NVS               │ 0xE000 - 0x12FFF│
│  OTA Data          │ 0x13000- 0x14FFF│
│  App (Firmware)    │ 0x15000- 0x34FFFF│ ◄─ 3.3MB
│  SPIFFS            │ 0x350000-0x44FFFF│ ◄─ 1MB
└─────────────────────────────────────┘

┌─────────────────────────────────────┐
│         RAM (400KB)                  │
├─────────────────────────────────────┤
│  System Reserved   │ ~100KB          │
│  Heap              │ ~200KB          │
│  Stack             │ ~50KB           │
│  Static/Global     │ ~50KB           │
└─────────────────────────────────────┘
```

### Memory Optimization Strategies

1. **Stack Usage**
   - Use static buffers where possible
   - Avoid deep recursion
   - Limit local variable size

2. **Heap Usage**
   - Reuse JSON buffers
   - Free unused objects
   - Avoid memory fragmentation

3. **Flash Usage**
   - Custom partition scheme
   - Remove unused fonts
   - Optimize assets

## Communication Protocol

### Moonraker API Endpoints

```
Base URL: http://[KLIPPER_IP]:7125

GET /server/info
├─ Response: Server information
└─ Used for: Connection testing

GET /printer/objects/query?[objects]
├─ Parameters: List of objects to query
├─ Response: Current state of requested objects
└─ Used for: Main data fetching

Objects queried:
- heater_bed
- extruder
- print_stats
- gcode_move
- fan
- display_status
- HomeSetVar (custom)
- BedLevelVar (custom)
```

### JSON Response Format

```json
{
  "result": {
    "status": {
      "extruder": {
        "temperature": 210.5,
        "target": 210.0
      },
      "heater_bed": {
        "temperature": 60.2,
        "target": 60.0
      },
      "print_stats": {
        "state": "printing",
        "filename": "test.gcode",
        "print_duration": 1234.5
      },
      "display_status": {
        "progress": 0.45
      }
    }
  }
}
```

## Performance Characteristics

### Timing
- **Boot time**: 3-5 seconds
- **WiFi connect**: 5-10 seconds
- **Update cycle**: 1 second (configurable)
- **API response**: 50-200ms
- **Screen refresh**: 20-50ms

### Resource Usage
- **CPU**: ~30% average, 60% peak
- **RAM**: ~240KB used, 160KB free
- **Flash**: ~3MB used, 1MB free
- **Network**: ~2KB/s average

### Bottlenecks
1. **Network latency** - API requests
2. **JSON parsing** - CPU intensive
3. **Display refresh** - SPI bandwidth
4. **Memory** - Limited by ESP32-C3

## Error Handling

### Error Types and Recovery

```
┌─────────────────────────────────────┐
│         Error Detection              │
└─────────────────────────────────────┘
              │
    ┌─────────┴─────────┐
    │                   │
    ▼                   ▼
┌─────────┐      ┌─────────┐
│  WiFi   │      │ Klipper │
│  Error  │      │  Error  │
└─────────┘      └─────────┘
    │                   │
    ▼                   ▼
┌─────────┐      ┌─────────┐
│ Retry   │      │ Retry   │
│ Connect │      │ Request │
└─────────┘      └─────────┘
    │                   │
    ├─ Success ─────────┤
    │                   │
    ▼                   ▼
┌─────────────────────────┐
│   Resume Normal Ops     │
└─────────────────────────┘
```

### Error Recovery Strategies

1. **WiFi Disconnection**
   - Display error screen
   - Attempt reconnection every 5 seconds
   - No limit on retries

2. **API Timeout**
   - Retry request once
   - Show Klipper error if persistent
   - Continue trying in background

3. **JSON Parse Error**
   - Log error to serial
   - Skip this update cycle
   - Continue with next cycle

4. **Display Error**
   - Reinitialize display
   - Reset graphics state
   - Continue operation

## Future Architecture Improvements

### Planned Enhancements

1. **WebSocket Support**
   - Replace HTTP polling
   - Real-time updates
   - Lower latency
   - Reduced network traffic

2. **Multi-threading**
   - Separate network thread
   - Dedicated UI thread
   - Background data processing

3. **Caching Layer**
   - Cache static data
   - Delta updates only
   - Reduce API calls

4. **Plugin System**
   - Loadable modules
   - Custom screens
   - Extended functionality

## Build System

### PlatformIO Configuration

The project uses PlatformIO for building and dependency management:

```ini
[env:esp32-c3-devkitm-1]
platform = espressif32
board = esp32-c3-devkitm-1
framework = arduino

lib_deps = 
    bodmer/TFT_eSPI@^2.5.43
    bblanchon/ArduinoJson@^6.21.5

build_flags =
    -DUSER_SETUP_LOADED=1
    -DGC9A01_DRIVER=1
    -DTFT_WIDTH=240
    -DTFT_HEIGHT=240
    # ... pin configurations
```

**Advantages**:
- Automatic dependency management
- Consistent build environment
- Better IDE integration
- Easier CI/CD setup
- No manual library installation

## Development Guidelines

### Code Organization
```
firmware/
├── src/
│   ├── main.cpp             # Main application
│   ├── WifiConfig.h         # Configuration
│   ├── DisplayDriver.h/cpp  # Display control
│   ├── KlipperAPI.h/cpp     # API client
│   ├── UIManager.h/cpp      # UI management
│   ├── SimpleUI.h/cpp       # Simple UI implementation
│   ├── LVGLDriver.h/cpp     # LVGL driver (optional)
│   └── User_Setup.h         # TFT_eSPI config
├── include/                 # Header files
├── lib/                     # Custom libraries
├── partitions/              # Partition schemes
└── platformio.ini           # PlatformIO configuration
```

### Coding Standards
- Use descriptive variable names
- Comment complex logic
- Keep functions focused
- Limit file size to ~500 lines
- Use const where possible
- Avoid global variables

### Testing Strategy
- Serial debug output
- Unit tests (future)
- Integration tests (future)
- Hardware-in-loop testing

## References

- ESP32-C3 Technical Reference: https://www.espressif.com/sites/default/files/documentation/esp32-c3_technical_reference_manual_en.pdf
- Moonraker API Documentation: https://moonraker.readthedocs.io/
- TFT_eSPI Library: https://github.com/Bodmer/TFT_eSPI
- ArduinoJson: https://arduinojson.org/
