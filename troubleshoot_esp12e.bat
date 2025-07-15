@echo off
echo.
echo ================================================================
echo ESP-12E Monitoring Troubleshooting Guide
echo ================================================================
echo.
echo If you're not seeing any output in the serial monitor, try these steps:
echo.

echo [STEP 1] Verify ESP-12E is in RUN mode (not BOOT mode)
echo.
echo   Check GPIO0 status:
echo   - GPIO0 should be HIGH (floating or pulled up to 3.3V)
echo   - If GPIO0 is LOW (connected to GND), ESP-12E stays in boot mode
echo.
echo   Action: Ensure GPIO0 is disconnected from GND
echo.
pause

echo [STEP 2] Hard Reset ESP-12E
echo.
echo   1. Disconnect power (or press and hold RESET)
echo   2. Ensure GPIO0 is HIGH (not connected to GND)
echo   3. Reconnect power (or release RESET)
echo   4. ESP-12E should boot into RUN mode
echo.
pause

echo [STEP 3] Test Serial Communication
echo.
echo Testing basic serial communication...
echo.

echo Trying different baud rates...

echo [Testing 115200 baud]
timeout /t 2 >nul
pio device monitor -p COM* --baud 115200 --filter time --exit-char 3

echo.
echo [Testing 74880 baud - ESP8266 boot messages]
timeout /t 2 >nul  
pio device monitor -p COM* --baud 74880 --filter time --exit-char 3

echo.
echo [Testing 9600 baud]
timeout /t 2 >nul
pio device monitor -p COM* --baud 9600 --filter time --exit-char 3

echo.
echo [STEP 4] Manual Reset Test
echo.
echo Now testing with manual reset...
echo 1. Keep this terminal open
echo 2. Press RESET button on ESP-12E NOW
echo 3. You should see boot messages or test output
echo.

echo Starting monitor (press Ctrl+C to exit)...
pio device monitor -e test_esp8266 --baud 115200

echo.
echo ================================================================
echo TROUBLESHOOTING CHECKLIST:
echo ================================================================
echo.
echo ✓ Did you see any output? (boot messages, test results, etc.)
echo.
echo If NO OUTPUT:
echo   □ Check GPIO0 is HIGH (not connected to GND) 
echo   □ Verify 3.3V power supply is stable
echo   □ Check TX/RX connections to USB-Serial adapter
echo   □ Try different USB-Serial adapter or USB port
echo   □ Verify ESP-12E is not damaged
echo.
echo If BOOT MESSAGES ONLY:
echo   □ Firmware may not have uploaded correctly
echo   □ Re-upload with: pio test -e test_esp8266 --without-testing
echo   □ Ensure GPIO0 LOW during upload, HIGH during run
echo.
echo If GARBLED OUTPUT:
echo   □ Try different baud rates (74880, 115200, 9600)
echo   □ Check for loose connections
echo   □ Verify ground connection between ESP-12E and USB-Serial
echo.
pause
