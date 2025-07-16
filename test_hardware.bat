@echo off
echo.
echo ================================================================
echo MideaUART Library - Hardware Test Verification
echo ================================================================
echo.

echo [1/3] Cleaning build cache...
pio run -t clean -e test_esp8266 >nul 2>&1

echo [2/3] Building test environment (ESP8266)...
pio test -e test_esp8266 --without-testing > temp_output.txt 2>&1

if %ERRORLEVEL% equ 0 (
    echo [SUCCESS] ESP8266 test environment compiles successfully!
    echo.
    echo Test environment details:
    echo - Framework: Arduino (ESP8266)
    echo - Test framework: Unity  
    echo - Test file: test_main.cpp
    echo - Build flags: -D UNIT_TEST
    echo.
) else (
    echo [ERROR] ESP8266 test compilation failed.
    echo.
    echo Build output:
    type temp_output.txt
    echo.
)

echo [3/3] Checking ESP32 test environment...
pio test -e test_esp32 --without-testing > temp_output2.txt 2>&1

if %ERRORLEVEL% equ 0 (
    echo [SUCCESS] ESP32 test environment also compiles successfully!
) else (
    echo [ERROR] ESP32 test compilation failed.
)

del temp_output.txt >nul 2>&1
del temp_output2.txt >nul 2>&1

echo.
echo ================================================================
echo Test Environment Verification Complete
echo ================================================================
echo.
echo To run actual hardware tests:
echo   pio test -e test_esp8266
echo   pio test -e test_esp32  
echo.
echo Note: Hardware tests require ESP8266/ESP32 connected via USB
echo ================================================================
