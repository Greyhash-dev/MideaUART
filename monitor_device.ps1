# MideaUART ESP8266 Monitor (PowerShell)
# Enhanced monitoring script for Windows compatibility

Write-Host "MideaUART ESP8266 Device Monitor" -ForegroundColor Green
Write-Host "==================================" -ForegroundColor Green
Write-Host ""

# Check if PlatformIO is available
try {
    $pioVersion = pio --version 2>$null
    Write-Host "PlatformIO CLI found: $pioVersion" -ForegroundColor Green
} catch {
    Write-Host "Error: PlatformIO CLI not found. Please install PlatformIO." -ForegroundColor Red
    Read-Host "Press Enter to exit"
    exit 1
}

# Function to list available devices
function List-Devices {
    Write-Host "Available devices:" -ForegroundColor Yellow
    try {
        pio device list
    } catch {
        Write-Host "Error listing devices" -ForegroundColor Red
    }
}

# Function to start monitoring
function Start-Monitor {
    param(
        [string]$Environment = "demo_esp8266",
        [string]$Port = "",
        [int]$Baud = 115200
    )
    
    Write-Host ""
    Write-Host "Starting Serial Monitor ($Baud baud)" -ForegroundColor Yellow
    Write-Host "Press Ctrl+C to stop monitoring" -ForegroundColor Yellow
    Write-Host "========================================" -ForegroundColor Yellow
    Write-Host ""
    
    if ($Port -ne "") {
        # Use specific port
        Write-Host "Connecting to $Port..." -ForegroundColor Cyan
        try {
            pio device monitor --port $Port --baud $Baud --eol CRLF
        } catch {
            Write-Host "Failed to connect to $Port" -ForegroundColor Red
        }
    } else {
        # Try environment-based monitoring
        Write-Host "Connecting using environment: $Environment..." -ForegroundColor Cyan
        try {
            pio device monitor -e $Environment
        } catch {
            Write-Host "Environment-based monitoring failed, trying auto-detection..." -ForegroundColor Yellow
            
            # Try to auto-detect COM port
            $devices = pio device list 2>$null | Select-String "COM\d+"
            if ($devices) {
                $comPort = ($devices[0] -split "\s+")[0]
                Write-Host "Auto-detected port: $comPort" -ForegroundColor Green
                try {
                    pio device monitor --port $comPort --baud $Baud --eol CRLF
                } catch {
                    Write-Host "Auto-detection failed" -ForegroundColor Red
                }
            } else {
                Write-Host "No COM ports detected" -ForegroundColor Red
            }
        }
    }
}

# Main menu
Write-Host "Select monitoring option:" -ForegroundColor Cyan
Write-Host "1. Auto-monitor demo_esp8266"
Write-Host "2. Auto-monitor simple_demo_esp8266"
Write-Host "3. Manual COM port entry"
Write-Host "4. List devices only"
Write-Host "5. Advanced troubleshooting"
Write-Host ""

$choice = Read-Host "Enter your choice [1-5]"

switch ($choice) {
    "1" {
        Start-Monitor -Environment "demo_esp8266"
    }
    "2" {
        Start-Monitor -Environment "simple_demo_esp8266"
    }
    "3" {
        List-Devices
        Write-Host ""
        $port = Read-Host "Enter COM port (e.g., COM3)"
        if ($port -ne "") {
            Start-Monitor -Port $port
        } else {
            Write-Host "Invalid COM port" -ForegroundColor Red
        }
    }
    "4" {
        List-Devices
    }
    "5" {
        Write-Host ""
        Write-Host "Troubleshooting Information:" -ForegroundColor Yellow
        Write-Host "===========================" -ForegroundColor Yellow
        
        Write-Host "PlatformIO version:"
        pio --version
        
        Write-Host ""
        Write-Host "Available devices:"
        List-Devices
        
        Write-Host ""
        Write-Host "Serial device details:"
        try {
            pio device list --serial
        } catch {
            Write-Host "Could not get serial device details" -ForegroundColor Red
        }
        
        Write-Host ""
        Write-Host "If monitoring fails, try these alternatives:" -ForegroundColor Green
        Write-Host "1. Use Arduino IDE Serial Monitor"
        Write-Host "2. Use PuTTY or Tera Term with the detected COM port"
        Write-Host "3. Try: pio device monitor --port COMx --baud 115200 --eol CRLF"
        Write-Host "4. Check Windows Device Manager for COM port conflicts"
    }
    default {
        Start-Monitor -Environment "demo_esp8266"
    }
}

Write-Host ""
Write-Host "Monitor session ended." -ForegroundColor Green
Read-Host "Press Enter to exit"
