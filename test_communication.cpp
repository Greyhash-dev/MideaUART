#include <Arduino.h>

// Simple communication test for ESP-12E
// This will continuously output messages to help verify serial communication

void setup() {
    Serial.begin(115200);
    delay(2000); // Wait for serial monitor to connect
    
    Serial.println();
    Serial.println("================================================================");
    Serial.println("ESP-12E Communication Test - MideaUART Library");
    Serial.println("================================================================");
    Serial.println();
    
    Serial.print("ESP Chip ID: ");
    Serial.println(ESP.getChipId(), HEX);
    
    Serial.print("Flash Chip ID: ");
    Serial.println(ESP.getFlashChipId(), HEX);
    
    Serial.print("Flash Size: ");
    Serial.println(ESP.getFlashChipSize());
    
    Serial.print("Free Heap: ");
    Serial.println(ESP.getFreeHeap());
    
    Serial.println();
    Serial.println("If you can see this message, ESP-12E communication is working!");
    Serial.println("Starting continuous output test...");
    Serial.println();
}

unsigned long lastMessage = 0;
int messageCount = 0;

void loop() {
    unsigned long currentTime = millis();
    
    if (currentTime - lastMessage >= 2000) { // Every 2 seconds
        lastMessage = currentTime;
        messageCount++;
        
        Serial.print("[");
        Serial.print(messageCount);
        Serial.print("] Time: ");
        Serial.print(currentTime);
        Serial.print(" ms, Free Heap: ");
        Serial.print(ESP.getFreeHeap());
        Serial.println(" bytes");
        
        if (messageCount % 10 == 0) {
            Serial.println("--- ESP-12E is running normally ---");
        }
    }
    
    delay(10);
}
