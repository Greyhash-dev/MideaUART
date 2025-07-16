#include <Arduino.h>

// Compile-time verification
#ifdef DEMO_MODE
#pragma message "DEMO_MODE is defined - compiling demo version"
#else
#pragma message "DEMO_MODE not defined - compiling library example"
#endif

// For ESP8266 AC simulation demo
#ifdef DEMO_MODE
#include "enhanced_mocks.h"
#include "Appliance/AirConditioner/AirConditioner.h"

using namespace dudanov::midea::ac;

// Global objects to persist across setup() and loop()
MockACStream mockAC;
AirConditioner ac;

void setup()
{
    Serial.begin(115200);
    delay(2000);

    Serial.println("=== ESP8266 Midea AC Simulation Demo ===");
    Serial.println("Running on real ESP8266 hardware with mock AC!");

    // Configure the mock AC's initial state
    mockAC.setSimulatedACState(false, 22, 1); // Off, 22°C, Cool mode
    mockAC.setSimulatedCurrentTemp(25);       // Current room temp 25°C
    mockAC.setAutoResponse(true);             // Enable automatic response generation

    // Connect the AC library to our mock
    ac.setStream(&mockAC);
    ac.setup();

    Serial.println("\n1. Initial AC Setup");
    Serial.println("   - Mock AC: OFF, Target: 22°C, Current: 25°C");

    // Let the AC library initialize and possibly send some commands
    for (int i = 0; i < 10; i++)
    {
        ac.loop();
        delay(10);
    }

    Serial.print("   - TX Data Length: ");
    Serial.println(mockAC.getTxLength());
    
    // Trigger a response manually to test the system
    if (mockAC.getTxLength() > 0) {
        Serial.println("   - Commands were sent, triggering response...");
        mockAC.triggerStatusResponse();
    }

    // Test 1: Turn on the AC
    Serial.println("\n2. Turning AC ON");
    mockAC.clearBuffers(); // Clear previous data for clean test

    ac.setPowerState(true);

    // Process the command and wait for response
    for (int i = 0; i < 50; i++)
    {
        ac.loop();
        delay(50); // Give more time for processing
        if (mockAC.getRxLength() > 0)
        {
            Serial.println("   ✓ Mock AC responded to power command");
            break;
        }
    }

    Serial.print("   - Command sent, TX bytes: ");
    Serial.println(mockAC.getTxLength());
    Serial.print("   - Response received, RX bytes: ");
    Serial.println(mockAC.getRxLength());

    // Test 2: Change temperature
    Serial.println("\n3. Setting temperature to 24°C");
    mockAC.clearBuffers();

    Control control;
    control.targetTemp = 24.0f;
    control.mode = Mode::MODE_COOL;
    ac.control(control);

    // Process the command and wait for response
    for (int i = 0; i < 50; i++)
    {
        ac.loop();
        delay(50);
        if (mockAC.getRxLength() > 0)
        {
            Serial.println("   ✓ Mock AC responded to temperature change");
            break;
        }
    }

    Serial.print("   - Command sent, TX bytes: ");
    Serial.println(mockAC.getTxLength());

    Serial.println("\n=== Setup Demo Complete ===");
    Serial.println("The enhanced mock enables full AC simulation without hardware!");
    Serial.println("\nStarting continuous demo in loop()...");
}

void loop()
{
    static unsigned long lastTest = 0;
    static int testCounter = 0;
    
    // Always call ac.loop() to keep the AC library processing
    ac.loop();
    
    if (millis() - lastTest > 5000)
    { // Every 5 seconds
        lastTest = millis();
        testCounter++;

        Serial.println("\n--- Periodic AC Test #" + String(testCounter) + " ---");

        // Cycle through different test scenarios
        bool powerState = (testCounter % 2 == 0);
        uint8_t temp = 18 + (testCounter % 10); // 18-27°C
        Mode mode = (Mode)(testCounter % 3);     // 0=Auto, 1=Cool, 2=Heat

        // Update the mock AC's simulated state
        mockAC.setSimulatedACState(powerState, temp, (uint8_t)mode);
        mockAC.setSimulatedCurrentTemp(temp + random(-3, 4)); // ±3°C variation

        // Clear previous test data BEFORE sending new commands
        mockAC.clearBuffers();

        // Send commands using the persistent AC object
        Serial.print("Setting: ");
        Serial.print(mode == Mode::MODE_AUTO ? "AUTO" : 
                    mode == Mode::MODE_COOL ? "COOL" : 
                    mode == Mode::MODE_HEAT ? "HEAT" : "FAN");
        Serial.print(", Power: ");
        Serial.print(powerState ? "ON" : "OFF");
        Serial.print(", Target: ");
        Serial.print(temp);
        Serial.print("°C");

        // Send power command
        ac.setPowerState(powerState);
        
        // Give some time for the command to be sent
        for (int i = 0; i < 5; i++) {
            ac.loop();
            delay(50);
        }
        
        // Send control command  
        Control ctrl;
        ctrl.targetTemp = (float)temp;
        ctrl.mode = mode;
        ac.control(ctrl);

        // Process commands and wait for responses
        for (int i = 0; i < 10; i++) {
            ac.loop();
            delay(100);
        }
        
        // If no commands were sent by the library, trigger a periodic command manually
        if (mockAC.getTxLength() == 0) {
            Serial.print(" [No commands sent, triggering periodic]");
            mockAC.triggerPeriodicCommand();
        } else {
            // Don't force additional response - let the mock auto-respond to the actual command
            Serial.print(" [Commands sent, auto-responding]");
        }

        Serial.print(" -> TX: ");
        Serial.print(mockAC.getTxLength());
        Serial.print(" bytes, RX: ");
        Serial.print(mockAC.getRxLength());
        Serial.println(" bytes");

        // Show success/failure tracking statistics
        Serial.print("   📊 AC Library Stats: Success=");
        Serial.print(ac.getFrameSuccess());
        Serial.print(", Failure=");
        Serial.print(ac.getFrameFailure());
        if (ac.getLastSuccessTime() > 0) {
            Serial.print(", Last Success: ");
            Serial.print((millis() - ac.getLastSuccessTime()) / 1000);
            Serial.print("s ago");
        }
        Serial.println();

        // Show mock response status
        if (mockAC.getRxLength() > 0) {
            Serial.println("   ✓ Mock AC is responding properly");
            
            // Show some frame analysis for educational purposes
            String txBuf = mockAC.getTxBuffer();
            if (txBuf.length() > 0) {
                Serial.print("   📡 TX Frame: ");
                for (int i = 0; i < min(8, (int)txBuf.length()); i++) {
                    Serial.print("0x");
                    if ((uint8_t)txBuf[i] < 0x10) Serial.print("0");
                    Serial.print((uint8_t)txBuf[i], HEX);
                    Serial.print(" ");
                }
                if (txBuf.length() > 8) Serial.print("...");
                Serial.println();
            }
        } else {
            Serial.println("   ⚠ No response from mock AC");
            // Debug: show what was sent
            if (mockAC.getTxLength() > 0) {
                Serial.print("   Debug: TX Buffer length = ");
                Serial.println(mockAC.getTxLength());
            } else {
                Serial.println("   Debug: No commands were sent by AC library");
            }
        }
    }
}

#else
// Original demo code for other configurations
#include "Appliance/AirConditioner/AirConditioner.h"

using namespace dudanov::midea::ac;

AirConditioner ac;

// Example how can change work mode easily
static inline void switchMode(Mode mode) {
  Control control;
  control.mode = mode;
  ac.control(control);
}

// Example how can change mode and temp in same time
static inline void changeState(Mode mode, float targetTemp) {
  Control control;
  control.mode = mode;
  control.targetTemp = targetTemp;
  ac.control(control);
}

// Example how can change power state
static inline void setPowerState(bool state) {
  ac.setPowerState(state);
}

// Example how can change mode to AUTO and set target temp to 25C
static inline void changeAuto25() {
  Control control;
  control.mode = Mode::MODE_AUTO;
  control.targetTemp = 25.0f;
  ac.control(control);
}

// Here you may get new properties states
void onStateChange() {
  ac.getTargetTemp();
  ac.getIndoorTemp();
  ac.getMode();
  ac.getPreset();
  ac.getSwingMode();
  ac.getFanMode();
}

void setup() {
  Serial.begin(9600);     // set serial baudrate to 9600 8N1
  ac.setStream(&Serial);  // set stream serial interface
  ac.addOnStateCallback(onStateChange); // add callback
  ac.setup();
}

void loop() {
  ac.loop();
}

#endif
