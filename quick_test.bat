@echo off
REM Quick compilation test for MideaUART library

echo === MideaUART Quick Test ===
echo.

set "PIO_CMD=.venv\Scripts\python.exe -m platformio"

echo 🔧 Quick compilation test...
echo.

echo 📋 Testing ESP8266 compilation (primary target)...
%PIO_CMD% run -e demo_esp8266 --target clean
%PIO_CMD% run -e demo_esp8266
if %ERRORLEVEL% EQU 0 (
    echo ✅ ESP8266 compilation SUCCESS
) else (
    echo ❌ ESP8266 compilation FAILED
    exit /b 1
)

echo.
echo 🎉 Library compiles successfully!
echo Your MideaUART library is ready for testing.
echo.
echo Run 'run_tests.bat' for comprehensive testing.
pause
