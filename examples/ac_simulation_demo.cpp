#include <Arduino.h>
#include "enhanced_mocks.h"
#include "Appliance/AirConditioner/AirConditioner.h"

using namespace dudanov::midea::ac;

// Example: How to use the enhanced AC simulation mock

void setup() {
    Serial.begin(115200);
    delay(2000);
    
    Serial.println("=== Enhanced Midea AC Mock Demonstration ===");
    
    // Create enhanced mock that simulates AC responses
    MockACStream mockAC;
    AirConditioner ac;
    
    // Configure the mock AC's initial state
    mockAC.setSimulatedACState(false, 22, 1); // Off, 22°C, Cool mode
    mockAC.setSimulatedCurrentTemp(25); // Current room temp 25°C
    mockAC.setAutoResponse(true); // Enable automatic response generation
    
    // Connect the AC library to our mock
    ac.setStream(&mockAC);
    ac.setup();
    
    Serial.println("\n1. Initial AC Setup");
    Serial.println("   - Mock AC: OFF, Target: 22°C, Current: 25°C");
    
    // Let the AC library initialize and possibly send some commands
    for (int i = 0; i < 10; i++) {
        ac.loop();
        delay(10);
    }
    
    Serial.print("   - TX Data Length: ");
    Serial.println(mockAC.getTxLength());
    
    // Test 1: Turn on the AC
    Serial.println("\n2. Turning AC ON");
    mockAC.clearBuffers(); // Clear previous data for clean test
    
    ac.setPowerState(true);
    
    // Process the command
    for (int i = 0; i < 20; i++) {
        ac.loop();
        if (mockAC.available() > 0) {
            // The mock has generated a response!
            Serial.println("   ✓ Mock AC responded to power command");
            break;
        }
        delay(10);
    }
    
    Serial.print("   - Command sent, TX bytes: ");
    Serial.println(mockAC.getTxLength());
    Serial.print("   - Response received, RX bytes: ");
    Serial.println(mockAC.available());
    
    // Test 2: Change temperature
    Serial.println("\n3. Setting temperature to 24°C");
    mockAC.clearBuffers();
    
    Control control;
    control.targetTemp = 24.0f;
    control.mode = Mode::MODE_COOL;
    ac.control(control);
    
    // Process the command
    for (int i = 0; i < 20; i++) {
        ac.loop();
        if (mockAC.available() > 0) {
            Serial.println("   ✓ Mock AC responded to temperature change");
            break;
        }
        delay(10);
    }
    
    Serial.print("   - Command sent, TX bytes: ");
    Serial.println(mockAC.getTxLength());
    
    // Test 3: Simulate temperature change in the room
    Serial.println("\n4. Simulating room temperature change");
    mockAC.setSimulatedCurrentTemp(23); // Room cooled down
    
    // The mock will now respond with the new temperature in status updates
    mockAC.clearBuffers();
    
    // Trigger a status request
    for (int i = 0; i < 30; i++) {
        ac.loop();
        delay(10);
    }
    
    // Test 4: Check if specific commands were sent
    Serial.println("\n5. Command Analysis");
    
    // Clear and send a specific command to check
    mockAC.clearBuffers();
    ac.setPowerState(false); // Turn off
    
    for (int i = 0; i < 10; i++) {
        ac.loop();
        delay(5);
    }
    
    // Check for power-off command pattern (example)
    uint8_t powerOffPattern[] = {0xAA}; // Start of Midea command
    if (mockAC.wasBytesSent(powerOffPattern, 1)) {
        Serial.println("   ✓ Power command detected in TX data");
    }
    
    Serial.println("\n6. Mock State Summary");
    Serial.println("   - The MockACStream automatically:");
    Serial.println("     • Responds to device info requests");
    Serial.println("     • Sends status updates when commands are received");
    Serial.println("     • Simulates AC state changes (power, temp, mode)");
    Serial.println("     • Generates realistic Midea protocol responses");
    
    Serial.println("\n=== Demo Complete ===");
    Serial.println("The enhanced mock enables full AC simulation without hardware!");
}

void loop() {
    // Continuous testing could go here
    
    static unsigned long lastTest = 0;
    if (millis() - lastTest > 30000) { // Every 30 seconds
        lastTest = millis();
        
        Serial.println("\n--- Periodic Test ---");
        
        MockACStream mockAC;
        AirConditioner ac;
        
        // Quick test cycle
        mockAC.setSimulatedACState(true, 20, 2); // On, 20°C, Dry mode
        mockAC.setAutoResponse(true);
        
        ac.setStream(&mockAC);
        ac.setup();
        
        // Send a few commands
        ac.setPowerState(true);
        for (int i = 0; i < 10; i++) ac.loop();
        
        Control ctrl;
        ctrl.targetTemp = 26.0f;
        ctrl.mode = Mode::MODE_HEAT;
        ac.control(ctrl);
        for (int i = 0; i < 10; i++) ac.loop();
        
        Serial.print("Commands sent: ");
        Serial.print(mockAC.getTxLength());
        Serial.print(" bytes, Responses: ");
        Serial.print(mockAC.available());
        Serial.println(" bytes");
    }
}
