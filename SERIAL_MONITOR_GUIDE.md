# Serial Monitor Troubleshooting Guide

This guide addresses common issues with PlatformIO's device monitor on Windows, especially with ESP8266 devices.

## Quick Solutions

### Option 1: Use the Enhanced Monitor Scripts
```batch
# Batch script (Windows Command Prompt)
monitor_device.bat

# PowerShell script (Windows PowerShell)
powershell -ExecutionPolicy Bypass -File monitor_device.ps1
```

### Option 2: Manual COM Port Monitoring
1. Check available devices:
   ```bash
   pio device list
   ```

2. Connect to specific COM port:
   ```bash
   pio device monitor --port COM3 --baud 115200 --eol CRLF
   ```

### Option 3: Alternative Serial Monitors
- **Arduino IDE**: Tools → Serial Monitor (set to 115200 baud)
- **PuTTY**: Serial connection, COM port, 115200 baud
- **Tera Term**: Serial port, 115200-8-N-1

## Common Issues and Solutions

### Issue 1: "Could not open port"
**Symptoms**: Monitor fails to connect, port access denied
**Solutions**:
1. Close any other applications using the COM port
2. Disconnect and reconnect the ESP8266
3. Check Windows Device Manager for driver issues
4. Try a different USB cable or port

### Issue 2: No output or garbled text
**Symptoms**: Monitor connects but shows no data or strange characters
**Solutions**:
1. Verify baud rate (should be 115200)
2. Check that the ESP8266 program is actually running
3. Press the RESET button on the ESP8266
4. Try different monitor settings:
   ```bash
   pio device monitor --port COMx --baud 115200 --eol CRLF --echo
   ```

### Issue 3: Monitor starts then immediately exits
**Symptoms**: Monitor connection is brief, then terminates
**Solutions**:
1. Update PlatformIO: `pio update`
2. Update platform: `pio platform update espressif8266`
3. Try native Windows serial tools
4. Check for Windows COM port driver issues

### Issue 4: ESP8266 not detected
**Symptoms**: `pio device list` shows no devices
**Solutions**:
1. Install ESP8266 USB drivers (CH340, CP2102, etc.)
2. Check Windows Device Manager for unknown devices
3. Try different USB ports (avoid USB hubs)
4. Test with Arduino IDE to verify hardware

## Recommended Workflow

### For Demo/Testing:
1. **Upload**: `pio run -e demo_esp8266 -t upload`
2. **Monitor**: Use one of these methods:
   - `./monitor_device.bat` (enhanced script)
   - `pio device monitor -e demo_esp8266`
   - Manual COM port connection
   - Arduino IDE Serial Monitor

### For Development:
1. Use the complete demo script: `./demo_complete.bat`
2. This handles upload + monitoring automatically
3. Falls back to alternative monitoring if needed

## Environment-Specific Notes

### demo_esp8266
- Shows AC simulation with realistic protocol
- Outputs every 2 seconds
- Includes frame parsing demonstrations

### simple_demo_esp8266
- Minimal AC emulator
- Standalone, no library dependencies
- Good for basic connectivity testing

### test_esp8266
- Hardware unit tests
- Uses Unity framework
- Shows test results over serial

## Windows-Specific Configuration

The `platformio.ini` now includes Windows-friendly monitor settings:
```ini
monitor_filters = 
    default
    time
monitor_eol = CRLF
monitor_echo = yes
```

## Manual Driver Installation

If ESP8266 is not detected:

1. **CH340 Driver** (NodeMCU v2/v3):
   - Download from manufacturer or use Windows Update
   - Device should appear as "USB-SERIAL CH340"

2. **CP2102 Driver** (Some ESP8266 boards):
   - Download Silicon Labs CP210x driver
   - Device should appear as "Silicon Labs CP210x"

3. **Check Device Manager**:
   - Look for "Ports (COM & LPT)"
   - ESP8266 should show as "USB Serial Port (COMx)"

## Alternative Tools

If PlatformIO monitor continues to fail:

### PuTTY Setup:
1. Download PuTTY
2. Select "Serial" connection type
3. Set Serial line to your COM port (e.g., COM3)
4. Set Speed to 115200
5. Click "Open"

### Arduino IDE Setup:
1. Install ESP8266 board package
2. Select Tools → Board → NodeMCU 1.0
3. Select Tools → Port → your COM port
4. Open Tools → Serial Monitor
5. Set baud rate to 115200

## Getting Help

If issues persist:
1. Run `monitor_device.bat` and choose option 5 (troubleshooting)
2. Check the output for specific error messages
3. Verify hardware with a known-working serial terminal
4. Consider testing with a different ESP8266 board

## Success Indicators

When monitoring works correctly, you should see:
- Regular output every 1-2 seconds
- Structured data showing AC status
- Frame parsing information
- No garbled characters or connection drops

Example expected output:
```
=== MideaUART Demo Starting ===
MockAC: Simulating realistic AC responses
AC Status: Power=ON, Mode=COOL, Temp=22°C, Fan=AUTO
Parsed Frame: Type=STATUS, Temp=22, Power=ON
[Timestamp] Frame sent: AA 20 AC 00 00 00 00 00 ...
```
