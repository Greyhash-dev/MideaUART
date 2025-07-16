#include <Arduino.h>
#include <unity.h>

void test_basic_communication(void) {
    Serial.println("TEST: Basic communication test running");
    TEST_ASSERT_TRUE(true);
    Serial.println("TEST: Basic communication test PASSED");
}

void test_serial_output(void) {
    Serial.println("TEST: Serial output test running");
    Serial.print("Free heap: ");
    Serial.println(ESP.getFreeHeap());
    TEST_ASSERT_TRUE(ESP.getFreeHeap() > 1000);
    Serial.println("TEST: Serial output test PASSED");
}

void setup() {
    Serial.begin(115200);
    delay(3000); // Extra delay for serial monitor
    
    Serial.println();
    Serial.println("========================================");
    Serial.println("ESP-12E Unity Test Framework Debug");
    Serial.println("========================================");
    Serial.println();
    
    Serial.print("ESP Chip ID: ");
    Serial.println(ESP.getChipId(), HEX);
    Serial.print("Free heap: ");
    Serial.println(ESP.getFreeHeap());
    Serial.println();
    
    Serial.println("Starting Unity tests...");
    Serial.println();
    
    UNITY_BEGIN();
    
    Serial.println("Running test 1...");
    RUN_TEST(test_basic_communication);
    
    Serial.println("Running test 2...");
    RUN_TEST(test_serial_output);
    
    Serial.println("All tests completed!");
    UNITY_END();
    
    Serial.println();
    Serial.println("========================================");
    Serial.println("Test Framework Debug Complete!");
    Serial.println("========================================");
}

void loop() {
    // Blink to show it's running
    static unsigned long lastBlink = 0;
    static bool ledState = false;
    
    if (millis() - lastBlink > 1000) {
        lastBlink = millis();
        ledState = !ledState;
        Serial.print("Loop running... Free heap: ");
        Serial.println(ESP.getFreeHeap());
    }
}
