#include <Arduino.h>
#include "Appliance/AirConditioner/AirConditioner.h"

using namespace dudanov::midea::ac;

// Create AirConditioner instance
AirConditioner ac;

// Example callback for state changes
void onStateChange() {
    Serial.print("AC State Changed - ");
    Serial.print("Power: ");
    Serial.print(ac.getPowerState() ? "ON" : "OFF");
    Serial.print(", Mode: ");
    Serial.print((int)ac.getMode());
    Serial.print(", Target: ");
    Serial.print(ac.getTargetTemp());
    Serial.print("°C, Indoor: ");
    Serial.print(ac.getIndoorTemp());
    Serial.println("°C");
}

void setup() {
    Serial.begin(9600);     // Midea protocol uses 9600 baud
    delay(1000);
    
    Serial.println("=== MideaUART Basic Usage Example ===");
    
    // Set up the AC library
    ac.setStream(&Serial);  // Use Serial for communication with AC
    ac.addOnStateCallback(onStateChange);
    ac.setup();
    
    Serial.println("AC library initialized. Connect to Midea AC via UART.");
    Serial.println("Commands will be sent automatically.");
}

void loop() {
    // Always call ac.loop() to handle communication
    ac.loop();
    
    static unsigned long lastCommand = 0;
    if (millis() - lastCommand > 30000) { // Every 30 seconds
        lastCommand = millis();
        
        Serial.println("\n--- Sending periodic command ---");
        
        // Example: Toggle power state
        static bool powerToggle = false;
        powerToggle = !powerToggle;
        ac.setPowerState(powerToggle);
        
        // Example: Set temperature (if turning on)
        if (powerToggle) {
            Control control;
            control.mode = Mode::MODE_COOL;
            control.targetTemp = 22.0f;
            ac.control(control);
        }
    }
    
    delay(100); // Small delay for stability
}
