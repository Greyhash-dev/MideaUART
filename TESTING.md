# MideaUART Library Testing Framework

## 🎯 Overview

This testing framework provides comprehensive validation for the MideaUART library without requiring actual hardware. It includes:

- **Demo Applications** - Full working examples that demonstrate library usage
- **Unit Tests** - Comprehensive test suite using Unity framework  
- **Mock Hardware** - Software simulation of serial communication
- **Multi-Platform** - Primary support for ESP8266, secondary for ESP32

## 🚀 Quick Start

### Option 1: Quick Compilation Test
```powershell
.\quick_test.bat
```
*Tests basic compilation on ESP8266 (primary target) to ensure library builds correctly*

### Option 2: Full Test Suite  
```powershell
.\run_tests.bat
```
*Runs comprehensive tests including unit tests and demos on both ESP8266 and ESP32*

### Option 3: Manual Testing
```powershell
# Install PlatformIO (if not already installed)
python -m venv .venv
.venv\Scripts\pip.exe install platformio

# Run demo application (ESP8266 - primary target)
.venv\Scripts\python.exe -m platformio run -e demo_esp8266

# Run unit tests (ESP8266 - primary target)
.venv\Scripts\python.exe -m platformio test -e test_esp8266

# Optional: Test on ESP32 (secondary target)
.venv\Scripts\python.exe -m platformio run -e demo_esp32
.venv\Scripts\python.exe -m platformio test -e test_esp32
```

## 📁 Project Structure

```
MideaUART/
├── platformio.ini          # Build configurations (ESP8266 primary, ESP32 secondary)
├── src/
│   └── main.cpp            # Demo application with comprehensive tests
├── test/
│   ├── test_main.cpp       # Unity-based unit tests
│   └── test_mocks.h        # Mock hardware implementations
├── include/                # Library headers (unchanged)
├── src/                    # Library source (unchanged)
├── run_tests.bat          # Full test runner
├── quick_test.bat         # Quick compilation test
└── TESTING.md             # This documentation
```

## 🧪 Test Environments

| Environment | Target | Purpose | Priority |
|-------------|--------|---------|----------|
| `test_esp8266` | ESP8266 (NodeMCU v2) | Unity unit tests for ESP8266 | **Primary** |
| `demo_esp8266` | ESP8266 (NodeMCU v2) | Demo application for ESP8266 | **Primary** |
| `test_esp32` | ESP32 | Unity unit tests for ESP32 | Secondary |
| `demo_esp32` | ESP32 | Demo application for ESP32 | Secondary |
| `test_native` | PC/Host | Native tests (optional) | Optional |

**Note**: ESP8266 (NodeMCU v2) is the primary target platform. ESP32 support is maintained for compatibility, and native testing is optional since hardware-based testing is preferred.

## 📋 What Gets Tested

### Demo Application (`src/main.cpp`)
✅ **Library Instantiation** - AirConditioner object creation  
✅ **Initial State Validation** - Default values and modes  
✅ **Configuration Methods** - Period, timeout, attempts settings  
✅ **Control Commands** - Mode changes, temperature settings  
✅ **Power Operations** - On/off, toggle functionality  
✅ **Communication Simulation** - Mock serial data exchange  
✅ **Continuous Operation** - Long-running stability tests  
✅ **Error Handling** - Graceful failure handling  

### Unit Tests (`test/test_main.cpp`)
✅ **Frame Operations** - Frame creation and data handling  
✅ **FrameData Manipulation** - Data structure operations  
✅ **AirConditioner API** - All public methods  
✅ **Configuration Persistence** - Settings retention  
✅ **Autoconf Functionality** - Auto-configuration features  
✅ **Communication Generation** - TX data validation  
✅ **Getter Methods** - State retrieval functions  

## 🔧 Mock Hardware System

The testing framework includes a sophisticated mock system that simulates hardware without requiring actual devices:

### MockStream Class
- **TX Buffer Capture** - Records all outgoing serial data
- **RX Data Injection** - Simulates incoming device responses  
- **Communication Patterns** - Validates protocol compliance
- **Performance Metrics** - Tracks data volumes and timing

### Usage Example
```cpp
MockStream mockSerial;
AirConditioner ac;
ac.setStream(&mockSerial);

// Simulate device operation
ac.setPowerState(true);
ac.loop();

// Verify communication occurred
assert(mockSerial.getTxLength() > 0);
```

## 📊 Test Output Interpretation

### Successful Demo Run
```
=== MideaUART Library Tests ===
Test: AirConditioner instantiation ... PASS
Test: Initial power state is OFF ... PASS
Test: Period configuration ... PASS
Test: Communication generated ... PASS

=== Test Results ===
Tests run: 10
Tests passed: 10
Tests failed: 0
🎉 ALL TESTS PASSED!
```

### Successful Unit Tests
```
Unity Test Summary
------------------
10 Tests 0 Failures 0 Ignored 
OK
```

## 🎛️ Development Workflow

### Before Making Changes
```powershell
# Verify current state
.\quick_test.bat

# Run full test suite
.\run_tests.bat
```

### After Making Changes
```powershell
# Quick validation
.\quick_test.bat

# Full regression testing
.\run_tests.bat
```

### Adding New Features
1. **Add tests first** - Modify `src/main.cpp` or `test/test_main.cpp`
2. **Implement feature** - Update library code
3. **Verify tests pass** - Run test suite
4. **Test on hardware** - Upload to device if available

## 🖥️ Hardware Testing (Optional)

If you have ESP32/ESP8266 hardware available:

```powershell
# Upload demo to ESP32
.venv\Scripts\python.exe -m platformio run -e demo_esp32 --target upload

# Monitor serial output
.venv\Scripts\python.exe -m platformio device monitor

# Upload unit tests to ESP32
.venv\Scripts\python.exe -m platformio test -e test_esp32 --target upload
```

## 🔍 Troubleshooting

### "PlatformIO not found"
```powershell
python -m venv .venv
.venv\Scripts\pip.exe install platformio
```

### "Compilation errors"
- Check that all header files exist in `include/`
- Verify namespace declarations
- Ensure all dependencies are properly included

### "Tests fail"
- Check test output for specific failure reasons
- Verify mock objects are properly initialized
- Ensure library state is reset between tests

## � Adding Your Own Tests

### Demo Application Tests
Add to `src/main.cpp`:
```cpp
TEST_ASSERT("Your test name", your_condition);
```

### Unit Tests
Add to `test/test_main.cpp`:
```cpp
void test_your_feature(void) {
    // Your test code
    TEST_ASSERT_EQUAL(expected, actual);
}

// In setup():
RUN_TEST(test_your_feature);
```

## 🎉 Success Criteria

Your testing framework is working correctly when:

✅ **Compilation succeeds** for all environments  
✅ **Demo application runs** without crashes  
✅ **Unit tests pass** with 100% success rate  
✅ **Communication is generated** during mock operations  
✅ **No memory leaks** during continuous operation  
✅ **All API methods callable** without errors  

## 🚀 Ready for Enhancement!

With this testing framework in place, you can now:

- ✅ **Safely add new features** with confidence
- ✅ **Refactor existing code** knowing tests will catch regressions  
- ✅ **Validate protocol changes** using mock communication
- ✅ **Measure performance** through automated testing
- ✅ **Document behavior** with executable examples

Your MideaUART library now has professional-grade testing capabilities! 🎯
