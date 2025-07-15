@echo off
echo === MideaUART Demo Test Script ===
echo.
echo 1. Testing demo compilation...
pio run -e demo_esp8266
if %ERRORLEVEL% == 0 (
    echo ✓ Demo compilation successful!
) else (
    echo ✗ Demo compilation failed!
    goto :end
)
echo.
echo 2. Testing simple demo compilation...
pio run -e simple_demo_esp8266
if %ERRORLEVEL% == 0 (
    echo ✓ Simple demo compilation successful!
) else (
    echo ✗ Simple demo compilation failed!
)
echo.
echo 3. Testing ESP8266 test compilation...
pio test -e test_esp8266 --without-uploading
if %ERRORLEVEL% == 0 (
    echo ✓ Test compilation successful!
) else (
    echo ✗ Test compilation failed!
)
echo.
echo === All Environments Tested ===
echo Demo ready for upload to ESP8266!
echo.
echo To upload and run:
echo   pio run -e demo_esp8266 --target upload
echo   pio device monitor
echo.
:end
pause
