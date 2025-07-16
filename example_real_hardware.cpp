#include <Arduino.h>
#include "Appliance/AirConditioner/AirConditioner.h"

using namespace dudanov::midea::ac;

AirConditioner ac;

void setup() {
    Serial.begin(115200);
    delay(2000);
    
    Serial.println("=== Real Midea AC Test ===");
    Serial.println("Testing success/failure tracking with real hardware");
    
    // Connect to real hardware serial (adjust pins as needed)
    Serial2.begin(9600); // Use appropriate serial port for your hardware
    ac.setStream(&Serial2);
    ac.setup();
    
    Serial.println("AC library initialized. Waiting for real AC communication...");
}

void loop() {
    static unsigned long lastStats = 0;
    
    // Always call ac.loop() to process communication
    ac.loop();
    
    // Print stats every 10 seconds
    if (millis() - lastStats > 10000) {
        lastStats = millis();
        
        Serial.println("\n--- Real AC Communication Stats ---");
        Serial.print("Success: ");
        Serial.print(ac.getFrameSuccess());
        Serial.print(", Failure: ");
        Serial.print(ac.getFrameFailure());
        
        if (ac.getLastSuccessTime() > 0) {
            Serial.print(", Last Success: ");
            Serial.print((millis() - ac.getLastSuccessTime()) / 1000);
            Serial.print("s ago");
        }
        Serial.println();
        
        // Test a simple command every 30 seconds
        static unsigned long lastCommand = 0;
        if (millis() - lastCommand > 30000) {
            lastCommand = millis();
            Serial.println("Sending test command to real AC...");
            
            // Simple power toggle test
            static bool powerState = false;
            powerState = !powerState;
            ac.setPowerState(powerState);
            
            Serial.print("Power set to: ");
            Serial.println(powerState ? "ON" : "OFF");
        }
    }
}
