#include <Arduino.h>

// Simplified mock stream for standalone demo
class SimpleMockAC {
private:
    String txBuffer;
    bool powerState = false;
    uint8_t targetTemp = 22;
    uint8_t currentTemp = 25;
    uint8_t mode = 1; // Cool mode
    
public:
    void sendCommand(const String& command) {
        txBuffer += command;
        Serial.print("→ Sent: ");
        Serial.println(command);
        
        // Simulate AC response
        delay(50);
        processCommand(command);
    }
    
    void processCommand(const String& cmd) {
        if (cmd.indexOf("POWER_ON") >= 0) {
            powerState = true;
            Serial.println("← AC Response: Power ON confirmed");
            sendStatus();
        }
        else if (cmd.indexOf("POWER_OFF") >= 0) {
            powerState = false;
            Serial.println("← AC Response: Power OFF confirmed");
            sendStatus();
        }
        else if (cmd.indexOf("SET_TEMP_") >= 0) {
            int pos = cmd.indexOf("SET_TEMP_") + 9;
            targetTemp = cmd.substring(pos, pos + 2).toInt();
            Serial.print("← AC Response: Temperature set to ");
            Serial.print(targetTemp);
            Serial.println("°C");
            sendStatus();
        }
        else if (cmd.indexOf("SET_MODE_") >= 0) {
            int pos = cmd.indexOf("SET_MODE_") + 9;
            mode = cmd.substring(pos, pos + 1).toInt();
            String modes[] = {"AUTO", "COOL", "DRY", "HEAT", "FAN"};
            Serial.print("← AC Response: Mode set to ");
            Serial.println(modes[mode]);
            sendStatus();
        }
        else if (cmd.indexOf("GET_STATUS") >= 0) {
            sendStatus();
        }
    }
    
    void sendStatus() {
        Serial.println("--- AC STATUS ---");
        Serial.print("Power: ");
        Serial.println(powerState ? "ON" : "OFF");
        Serial.print("Target Temp: ");
        Serial.print(targetTemp);
        Serial.println("°C");
        Serial.print("Current Temp: ");
        Serial.print(currentTemp);
        Serial.println("°C");
        String modes[] = {"AUTO", "COOL", "DRY", "HEAT", "FAN"};
        Serial.print("Mode: ");
        Serial.println(modes[mode]);
        Serial.println("----------------");
    }
    
    void simulateRoomTempChange() {
        if (powerState && mode == 1) { // Cool mode
            if (currentTemp > targetTemp) {
                currentTemp--;
                Serial.print("🌡️  Room cooling: ");
                Serial.print(currentTemp);
                Serial.println("°C");
            }
        }
        else if (powerState && mode == 3) { // Heat mode
            if (currentTemp < targetTemp) {
                currentTemp++;
                Serial.print("🌡️  Room heating: ");
                Serial.print(currentTemp);
                Serial.println("°C");
            }
        }
    }
    
    String getTxData() { return txBuffer; }
    void clearTx() { txBuffer = ""; }
};

SimpleMockAC mockAC;

void setup() {
    Serial.begin(115200);
    delay(2000);
    
    Serial.println("🏠 === ESP8266 AC Emulator Demo ===");
    Serial.println("Running on NodeMCU v2 (ESP8266)");
    Serial.println("Simulating Midea AC control without hardware");
    Serial.println();
    
    // Initialize LED
    pinMode(LED_BUILTIN, OUTPUT);
    
    // Initial status
    Serial.println("1. Initial AC Status");
    mockAC.sendCommand("GET_STATUS");
    delay(1000);
    
    // Turn on AC
    Serial.println("\n2. Turning AC ON");
    mockAC.sendCommand("POWER_ON");
    delay(1000);
    
    // Set temperature
    Serial.println("\n3. Setting temperature to 20°C");
    mockAC.sendCommand("SET_TEMP_20");
    delay(1000);
    
    // Change mode to Cool
    Serial.println("\n4. Setting mode to COOL");
    mockAC.sendCommand("SET_MODE_1");
    delay(1000);
    
    Serial.println("\n5. Starting continuous simulation...");
    Serial.println("The AC will now simulate cooling the room from 25°C to 20°C");
    Serial.println("Watch the temperature change every few seconds!");
}

void loop() {
    static unsigned long lastTempUpdate = 0;
    static unsigned long lastStatusUpdate = 0;
    static unsigned long lastDemo = 0;
    
    // Update room temperature every 3 seconds
    if (millis() - lastTempUpdate > 3000) {
        lastTempUpdate = millis();
        mockAC.simulateRoomTempChange();
    }
    
    // Show status every 10 seconds
    if (millis() - lastStatusUpdate > 10000) {
        lastStatusUpdate = millis();
        Serial.println("\n📊 Periodic Status Check");
        mockAC.sendCommand("GET_STATUS");
    }
    
    // Run different demo scenarios every 30 seconds
    if (millis() - lastDemo > 30000) {
        lastDemo = millis();
        
        Serial.println("\n🎭 === New Demo Scenario ===");
        
        // Random scenario
        int scenario = (millis() / 1000) % 4;
        
        switch (scenario) {
            case 0:
                Serial.println("Scenario: Hot summer day - Cool to 18°C");
                mockAC.sendCommand("POWER_ON");
                mockAC.sendCommand("SET_TEMP_18");
                mockAC.sendCommand("SET_MODE_1"); // Cool
                break;
                
            case 1:
                Serial.println("Scenario: Cold winter day - Heat to 25°C");
                mockAC.sendCommand("POWER_ON");
                mockAC.sendCommand("SET_TEMP_25");
                mockAC.sendCommand("SET_MODE_3"); // Heat
                break;
                
            case 2:
                Serial.println("Scenario: Turn OFF AC");
                mockAC.sendCommand("POWER_OFF");
                break;
                
            case 3:
                Serial.println("Scenario: Auto mode at 22°C");
                mockAC.sendCommand("POWER_ON");
                mockAC.sendCommand("SET_TEMP_22");
                mockAC.sendCommand("SET_MODE_0"); // Auto
                break;
        }
    }
    
    // Blink built-in LED to show ESP8266 is running
    static unsigned long lastBlink = 0;
    if (millis() - lastBlink > 500) {
        lastBlink = millis();
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
}
