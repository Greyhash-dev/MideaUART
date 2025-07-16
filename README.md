# MideaUART
Arduino framework library for controlling Midea home appliances using the UART protocol with communication tracking.

Control is possible with a custom dongle. You can make it yourself according to numerous instructions on the Internet, or you can buy a ready-made one in [Tindie Shop](https://www.tindie.com/products/24607/), thereby supporting me and my work.

## Supported Platforms

**Primary Target**: ESP8266 (NodeMCU v2)  
**Secondary Target**: ESP32  
**Framework**: Arduino

This library is optimized for ESP8266 but also supports ESP32. The comprehensive test framework ensures compatibility across both platforms.

## Supported Brands

A far from complete list of supported brands:
1. [Midea](https://www.midea.com/)
2. [Electrolux](https://www.electrolux.ru/)
3. [Qlima](https://www.qlima.com/)
4. [Artel](https://www.artelgroup.com/)
5. [Carrier](https://www.carrier.com/)
6. [Comfee](http://www.comfee-russia.ru/)
7. [Inventor](https://www.inventorairconditioner.com/)
8. [Dimstal/Simando](https://www.simando24.de/)

## Using
It's simple.
1. Create appliance instance of `dudanov::midea::ac::AirConditioner`.
2. Set serial stream interface and communication mode to `9600 8N1`.
3. Add `setup()` and `loop()` methods to the same-named global functions of the project.
4. Control device via `void control(const Control &control)` with optional parameters.
5. You may optionally add your callback function for receive state changes notifications.

```cpp
#include <Arduino.h>
#include <Appliance/AirConditioner/AirConditioner.h>

using namespace dudanov::midea::ac;

AirConditioner ac;

// Example how can change work mode easily
static inline void switchMode(Mode mode) {
  Control control;
  control.mode = mode;
  ac.control(control);
}

// Example how can change mode and temp in same time
static inline void changeState(Mode mode, float targetTemp) {
  Control control;
  control.mode = mode;
  control.targetTemp = targetTemp;
  ac.control(control);
}

// Example how can change power state
static inline void setPowerState(bool state) {
  ac.setPowerState(state);
}

// Example how can change mode to AUTO and set target temp to 25C
static inline void changeAuto25() {
  Control control;
  control.mode = Mode::MODE_AUTO;
  control.targetTemp = 25.0f;
  ac.control(control);
}

// Here you may get new properties states
void onStateChange() {
  ac.getTargetTemp();
  ac.getIndoorTemp();
  ac.getMode();
  ac.getPreset();
  ac.getSwingMode();
  ac.getFanMode();
  
  // New v1.2.0: Monitor communication health
  Serial.print("Successful frames: ");
  Serial.println(ac.getFrameSuccess());
  Serial.print("Failed frames: ");
  Serial.println(ac.getFrameFailure());
  Serial.print("Last success time: ");
  Serial.println(ac.getLastSuccessTime());
}

void setup() {
  Serial.begin(9600);     // set serial baudrate to 9600 8N1
  ac.setStream(&Serial);  // set stream serial interface
  ac.addOnStateCallback(onStateChange); // add callback
  ac.setup();
}

void loop() {
  ac.loop();
}
```

## New Features (v1.2.0)

### Communication Tracking
The library now includes robust communication monitoring to help track the health of your connection:

```cpp
// Get communication statistics
uint32_t successes = ac.getFrameSuccess();   // Number of successful frame exchanges
uint32_t failures = ac.getFrameFailure();    // Number of failed frame exchanges
uint32_t lastSuccess = ac.getLastSuccessTime(); // Timestamp of last successful communication

// Calculate success rate
float successRate = (successes + failures > 0) ? 
    (float)successes / (successes + failures) * 100.0f : 0.0f;

Serial.printf("Success rate: %.1f%% (%u/%u)\n", 
    successRate, successes, successes + failures);
```

These metrics are automatically tracked in the background and help identify:
- Connection reliability issues
- When the last successful communication occurred
- Overall communication health over time

## Testing & Development

This library includes a comprehensive test framework for safe development and enhancement:

### Quick Demo/Test (ESP8266 - Primary Target)
```powershell
# Complete demo with automatic upload and monitoring
.\demo_complete.bat

# Quick test compilation and upload
.\quick_test.bat
```

### Enhanced Serial Monitoring
If you experience issues with PlatformIO's device monitor (common on Windows):

```powershell
# Enhanced monitoring script (multiple fallback options)
.\monitor_device.bat

# PowerShell alternative
powershell -ExecutionPolicy Bypass -File monitor_device.ps1
```

### Full Test Suite (ESP8266 + ESP32)
```powershell
.\run_tests.bat
```

### Manual Commands
```bash
# Build and upload demo to ESP8266
pio run -e demo_esp8266 -t upload

# Monitor serial output (if PlatformIO monitor works)
pio device monitor -e demo_esp8266

# Alternative monitoring with specific COM port
pio device monitor --port COM3 --baud 115200 --eol CRLF
```

### Troubleshooting Serial Monitor
See [`SERIAL_MONITOR_GUIDE.md`](SERIAL_MONITOR_GUIDE.md) for comprehensive troubleshooting if monitoring doesn't work.

For detailed testing information, see [`TESTING.md`](TESTING.md).

## My thanks

to the following people for their contributions to reverse engineering the UART protocol and source code in the following repositories:

* [Mac Zhou](https://github.com/mac-zhou/midea-msmart)
* [Rene Klootwijk](https://github.com/reneklootwijk/midea-uart)
* [NeoAcheron](https://github.com/NeoAcheron/midea-ac-py)

### Your thanks

If this project was useful to you, you can [buy me](https://paypal.me/dudan0v) a Cup of coffee :)
