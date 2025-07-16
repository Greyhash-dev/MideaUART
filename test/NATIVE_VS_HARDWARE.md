# Native Testing (PC) vs Hardware Testing

## Current Setup: Hardware-Based Testing

Your current `test_main.cpp` is designed for **microcontroller testing**:

```cpp
#include <unity.h>
#include <Arduino.h>  // ← Arduino framework (ESP8266/ESP32 only)
```

**Execution Flow:**
```
PC → Compile → Upload to ESP8266 → Run Tests → Serial Monitor Results
```

## Option 1: Native PC Testing (No Hardware Required)

To run tests on your PC without hardware, you'd need:

### Native Test Environment
```ini
[env:native]
platform = native
test_framework = unity
lib_deps = 
    throwtheswitch/Unity@^2.5.2
build_flags = 
    -D NATIVE_TEST
    -std=c++17
    -I include
    -I test/native
```

### Native Test Code
```cpp
#include <unity.h>
#include <iostream>
#include <string>
// No Arduino.h - use standard C++ libraries

// Mock Arduino functions for PC
unsigned long millis() { return 0; }
void delay(int ms) { /* no-op */ }
```

## Option 2: Hardware Testing (Current Setup)

Your current setup requires ESP8266/ESP32:

```powershell
# Compile and upload to ESP8266
.venv\Scripts\python.exe -m platformio test -e test_esp8266 --target upload

# Monitor results via serial
.venv\Scripts\python.exe -m platformio device monitor
```

## Recommendation

For **library development**, I recommend both approaches:

1. **Native tests** (PC) - Fast feedback during development
2. **Hardware tests** (ESP8266) - Final validation with real hardware

Would you like me to create a native test version that runs on your PC?
