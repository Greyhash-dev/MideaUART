# MideaUART Library Examples

This folder contains example projects demonstrating how to use the MideaUART library.

## Examples Overview

### 1. `basic_usage/`
**File:** `basic_usage.cpp`  
**Purpose:** Simple example showing how to integrate MideaUART library into your project  
**Features:**
- Basic AC control (power on/off, temperature setting)
- State change callbacks
- Periodic command sending
- Real hardware communication (9600 baud UART)

### 2. `demo/`
**File:** `demo_main.cpp`  
**Purpose:** Advanced demonstration with mock AC simulation  
**Features:**
- Mock AC emulator for safe testing
- Protocol analysis and debugging
- No real hardware required
- Educational frame analysis

## Usage in Your Project

### Method 1: PlatformIO Library Manager
```ini
; In your platformio.ini
lib_deps = 
    https://github.com/rrelande/MideaUART.git
```

### Method 2: Local Library
1. Copy the MideaUART folder to your project's `lib/` directory
2. Include in your code:
```cpp
#include "Appliance/AirConditioner/AirConditioner.h"
using namespace dudanov::midea::ac;
```

### Method 3: Git Submodule
```bash
git submodule add https://github.com/rrelande/MideaUART.git lib/MideaUART
```

## Running Examples

### For basic_usage:
```bash
# Create new PlatformIO project
pio project init --board nodemcuv2

# Copy basic_usage.cpp to src/main.cpp
cp examples/basic_usage/basic_usage.cpp src/main.cpp

# Add library dependency in platformio.ini
# Build and upload
pio run -t upload
```

### For demo (mock AC):
```bash
# Use the demo environment from this project
pio run -e demo_esp8266 -t upload
```

## Library Structure

```
MideaUART/
├── include/           # Library headers
├── src/              # Library implementation  
├── examples/         # Usage examples
├── test/             # Test files and mocks
├── library.json      # Library metadata
└── platformio.ini    # Build configuration
```

## Hardware Connection

For real AC hardware:
- **TX**: ESP8266 GPIO1 (Serial TX) → AC RX
- **RX**: ESP8266 GPIO3 (Serial RX) → AC TX  
- **Baud**: 9600, 8N1
- **Power**: Isolate and power both devices safely

## Important Notes

1. **Library Mode**: When using as a library, the `src/main.cpp` is excluded automatically
2. **Examples**: Demonstration code is in `examples/` folder  
3. **Testing**: Use `demo/` example for safe testing without real hardware
4. **Serial**: Real hardware uses 9600 baud, demo uses 115200 for debugging
