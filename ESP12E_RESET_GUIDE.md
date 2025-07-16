# ESP-12E Manual Reset Procedure

## Hardware Setup
- **Board**: ESP-12E Module (no auto-reset circuit)
- **Connections**: TX, RX, VCC (3.3V), GND, GPIO0, RST accessible

## Upload Mode (for flashing firmware)
1. **Hold GPIO0 LOW** (connect GPIO0 to GND)
2. **Press and release RESET** (briefly connect RST to GND)
3. **Release GPIO0** (disconnect from GND)
4. **ESP-12E is now in boot/flash mode**

## Run Mode (normal operation)
1. **Press RESET** (or power cycle)
2. **Ensure GPIO0 is HIGH** (floating or pulled up)
3. **ESP-12E runs uploaded firmware**

## Testing Workflow

### Compile and Upload Tests:
```bash
# Put ESP-12E in boot mode first (steps above)
pio test -e test_esp8266 --without-testing
```

### Run Tests:
```bash
# Reset ESP-12E to run mode, then monitor
pio device monitor -e test_esp8266 --baud 115200
```

### Upload and Run Demo:
```bash
# Put ESP-12E in boot mode first
pio run -e demo_esp8266 --target upload

# Reset to run mode, then monitor  
pio run -e demo_esp8266 --target monitor
```

## Alternative: Hardware Auto-Reset Circuit

If manual reset becomes tedious, you can add auto-reset hardware:

```
USB-Serial    ESP-12E
DTR ----[100nF]---- RST
RTS ----[100nF]---- GPIO0
```

This allows PlatformIO to automatically control boot/run modes.

## Troubleshooting

**Upload fails**: 
- Verify GPIO0 was LOW during reset
- Check USB-Serial connections
- Ensure 3.3V power supply

**Tests don't run**:
- Press RESET after upload to exit boot mode
- Check Serial Monitor at 115200 baud
- Verify firmware uploaded successfully
