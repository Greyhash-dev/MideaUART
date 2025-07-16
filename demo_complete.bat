@echo off
echo MideaUART Complete Demo Workflow
echo =================================
echo.

REM Check if PlatformIO is available
where pio >nul 2>&1
if %errorlevel% neq 0 (
    echo Error: PlatformIO CLI not found. Please install PlatformIO.
    pause
    exit /b 1
)

echo Select demo to run:
echo 1. AC Simulation Demo (demo_esp8266)
echo 2. Simple AC Emulator (simple_demo_esp8266)
echo 3. Protocol Simulation Demo (protocol demo)
echo 4. Hardware Tests (test_esp8266)
echo 5. Monitor existing device
echo.

set /p CHOICE="Enter your choice [1-5]: "

if "%CHOICE%"=="1" goto demo_esp8266
if "%CHOICE%"=="2" goto simple_demo
if "%CHOICE%"=="3" goto protocol_demo
if "%CHOICE%"=="4" goto test_esp8266
if "%CHOICE%"=="5" goto monitor_only
goto demo_esp8266

:demo_esp8266
echo.
echo Building and uploading AC Simulation Demo...
echo pio run -e demo_esp8266 -t upload
pio run -e demo_esp8266 -t upload
if %errorlevel% neq 0 (
    echo Upload failed!
    pause
    exit /b 1
)
echo.
echo Upload successful! Starting monitor...
goto start_monitor

:simple_demo
echo.
echo Building and uploading Simple AC Emulator...
echo pio run -e simple_demo_esp8266 -t upload
pio run -e simple_demo_esp8266 -t upload
if %errorlevel% neq 0 (
    echo Upload failed!
    pause
    exit /b 1
)
echo.
echo Upload successful! Starting monitor...
goto start_monitor

:protocol_demo
echo.
echo Note: Protocol demo needs to be compiled as demo_esp8266 with protocol focus
echo Building and uploading Protocol Demo...
echo pio run -e demo_esp8266 -t upload
pio run -e demo_esp8266 -t upload
if %errorlevel% neq 0 (
    echo Upload failed!
    pause
    exit /b 1
)
echo.
echo Upload successful! Starting monitor...
goto start_monitor

:test_esp8266
echo.
echo Building and uploading Hardware Tests...
echo pio run -e test_esp8266 -t upload
pio run -e test_esp8266 -t upload
if %errorlevel% neq 0 (
    echo Upload failed!
    pause
    exit /b 1
)
echo.
echo Upload successful! Starting monitor...
goto start_monitor

:monitor_only
echo.
echo Starting monitor for existing device...
goto start_monitor

:start_monitor
echo.
echo ============================================
echo Starting Serial Monitor (115200 baud)
echo Press Ctrl+C to stop monitoring
echo ============================================
echo.

REM Try multiple approaches for monitoring
echo Attempting PlatformIO device monitor...
pio device monitor --baud 115200
if %errorlevel% neq 0 (
    echo.
    echo PlatformIO monitor failed. Trying alternative...
    
    REM Try to get the device port
    for /f "tokens=*" %%i in ('pio device list ^| findstr /r "COM[0-9]*"') do (
        echo Found device: %%i
        REM Extract COM port
        for /f "tokens=1" %%j in ("%%i") do set COMPORT=%%j
    )
    
    if defined COMPORT (
        echo Trying direct connection to %COMPORT%...
        pio device monitor --port %COMPORT% --baud 115200
    ) else (
        echo.
        echo Could not auto-detect COM port.
        echo Please check device connection and try manual monitor:
        echo   pio device monitor --port COMx --baud 115200
        echo.
        echo Available devices:
        pio device list
    )
)

echo.
echo Demo session ended.
pause
