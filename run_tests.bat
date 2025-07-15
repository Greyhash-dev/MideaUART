@echo off
REM MideaUART Library Test Runner
REM Runs comprehensive tests for the library

echo === MideaUART Library Test Suite ===
echo.

REM Check if virtual environment exists
if not exist ".venv\Scripts\python.exe" (
    echo Installing PlatformIO in virtual environment...
    python -m venv .venv
    .venv\Scripts\pip.exe install platformio
)

set "PIO_CMD=.venv\Scripts\python.exe -m platformio"

echo 🔧 Testing library compilation and functionality...
echo.

echo 📋 Running demo application (ESP8266 - Primary Target)...
%PIO_CMD% run -e demo_esp8266
if %ERRORLEVEL% EQU 0 (
    echo ✅ ESP8266 demo compilation PASSED
) else (
    echo ❌ ESP8266 demo compilation FAILED
    set FAILED=1
)
echo.

echo 📋 Running demo application (ESP32 - Secondary Target)...
%PIO_CMD% run -e demo_esp32
if %ERRORLEVEL% EQU 0 (
    echo ✅ ESP32 demo compilation PASSED
) else (
    echo ❌ ESP32 demo compilation FAILED
    set FAILED=1
)
echo.

echo 📋 Running unit tests (ESP8266 - Primary Target)...
%PIO_CMD% test -e test_esp8266
if %ERRORLEVEL% EQU 0 (
    echo ✅ ESP8266 unit tests PASSED
) else (
    echo ❌ ESP8266 unit tests FAILED
    set FAILED=1
)
echo.

echo 📋 Running unit tests (ESP32 - Secondary Target)...
%PIO_CMD% test -e test_esp32
if %ERRORLEVEL% EQU 0 (
    echo ✅ ESP32 unit tests PASSED
) else (
    echo ❌ ESP32 unit tests FAILED
    set FAILED=1
)
echo.

echo === Test Results ===
if "%FAILED%"=="1" (
    echo ❌ Some tests FAILED
    echo Check the output above for details.
) else (
    echo 🎉 ALL TESTS PASSED!
    echo Your MideaUART library is ready for enhancements!
)

echo.
echo === Next Steps ===
echo To upload tests to hardware (ESP8266 - Primary Target):
echo   %PIO_CMD% run -e demo_esp8266 --target upload
echo   %PIO_CMD% test -e test_esp8266 --target upload
echo.
echo To upload tests to hardware (ESP32 - Secondary Target):
echo   %PIO_CMD% run -e demo_esp32 --target upload
echo   %PIO_CMD% test -e test_esp32 --target upload
echo.
echo To monitor serial output:
echo   %PIO_CMD% device monitor
echo.
pause
