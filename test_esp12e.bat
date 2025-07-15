@echo off
echo.
echo ================================================================
echo MideaUART Library - ESP-12E Hardware Testing Guide
echo ================================================================
echo.
echo Your ESP-12E module requires MANUAL RESET for upload and testing.
echo.
echo HARDWARE CONNECTIONS REQUIRED:
echo - GPIO0 pin accessible (for boot mode)
echo - RESET pin accessible (for reset)
echo - TX/RX connected to USB-Serial adapter
echo - VCC: 3.3V, GND: Ground
echo.
echo ================================================================
echo STEP-BY-STEP TESTING PROCEDURE:
echo ================================================================
echo.

echo [STEP 1] Prepare for Upload Mode
echo.
echo   1. Hold GPIO0 LOW (connect GPIO0 to GND with jumper wire)
echo   2. Press and release RESET button (or briefly connect RST to GND)
echo   3. ESP-12E is now in BOOT/FLASH mode
echo   4. Release GPIO0 (remove jumper wire)
echo.
pause

echo [STEP 2] Upload Test Firmware
echo.
echo Running: pio test -e test_esp8266 --without-testing
echo This will compile and upload the test firmware...
echo.

pio test -e test_esp8266 --without-testing

if %ERRORLEVEL% equ 0 (
    echo.
    echo [SUCCESS] Test firmware uploaded successfully!
    echo.
) else (
    echo.
    echo [ERROR] Upload failed. Please check:
    echo - GPIO0 was held LOW during reset
    echo - USB-Serial connection is correct
    echo - ESP-12E has power (3.3V)
    echo.
    pause
    exit /b 1
)

echo [STEP 3] Start Test Execution
echo.
echo   1. Press RESET button to exit boot mode and start running tests
echo   2. Open Serial Monitor to see test results
echo.
echo Press any key when ready to open Serial Monitor...
pause

echo.
echo Opening Serial Monitor (115200 baud)...
echo Press Ctrl+C to exit monitor
echo.

pio device monitor -e test_esp8266 --baud 115200

echo.
echo ================================================================
echo Testing Complete!
echo ================================================================
echo.
echo If you saw Unity test results, your ESP-12E testing setup works!
echo.
echo For future tests, repeat the upload procedure:
echo 1. GPIO0 LOW + RESET + GPIO0 HIGH
echo 2. pio test -e test_esp8266 --without-testing  
echo 3. RESET to run
echo.
pause
