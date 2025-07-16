@echo off
echo MideaUART ESP8266 Device Monitor
echo ================================
echo.

REM Check if PlatformIO is available
where pio >nul 2>&1
if %errorlevel% neq 0 (
    echo Error: PlatformIO CLI not found. Please install PlatformIO.
    pause
    exit /b 1
)

REM Try to detect the ESP8266 device
echo Detecting ESP8266 device...
pio device list | findstr /i "CH340\|CP210\|ESP\|UART"

echo.
echo Available COM ports:
pio device list

echo.
set /p CHOICE="Do you want to (1) Auto-monitor demo_esp8266, (2) Manual COM port, or (3) List devices only? [1-3]: "

if "%CHOICE%"=="1" goto auto_monitor
if "%CHOICE%"=="2" goto manual_monitor
if "%CHOICE%"=="3" goto list_devices
goto auto_monitor

:auto_monitor
echo.
echo Starting automatic monitor for demo_esp8266 environment...
echo Press Ctrl+C to stop monitoring
echo.
pio device monitor -e demo_esp8266
goto end

:manual_monitor
echo.
set /p COMPORT="Enter COM port (e.g., COM3): "
if "%COMPORT%"=="" (
    echo Invalid COM port
    goto end
)
echo.
echo Starting monitor on %COMPORT% at 115200 baud...
echo Press Ctrl+C to stop monitoring
echo.
pio device monitor --port %COMPORT% --baud 115200
goto end

:list_devices
echo.
echo Detailed device information:
pio device list
echo.
echo Hardware info:
pio device list --serial
goto end

:end
echo.
echo Monitor session ended.
pause
