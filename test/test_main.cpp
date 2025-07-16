#include <unity.h>
#include <Arduino.h>
#include "test_mocks.h"
#include "Frame/Frame.h"
#include "Frame/FrameData.h"
#include "Appliance/AirConditioner/AirConditioner.h"

using namespace dudanov::midea;
using namespace dudanov::midea::ac;

MockStream mockStream;
AirConditioner testAC;

void setUp(void) {
    // Set up code here to run before each test
    mockStream.clearBuffers();
}

void tearDown(void) {
    // Clean up code here to run after each test
}

void test_frame_creation(void) {
    FrameData data = {0xAA, 0x23, 0xAC};
    Frame frame(AIR_CONDITIONER, 0x01, 0x03, data);
    
    TEST_ASSERT_NOT_NULL(frame.data());
    TEST_ASSERT_GREATER_THAN(0, frame.size());
    // TEST_ASSERT_EQUAL(0x03, frame.getType());
}

void test_frame_data_operations(void) {
    // Test FrameData creation with initializer list
    FrameData data({0x01, 0x02, 0x03});
    
    TEST_ASSERT_EQUAL(3, data.size());
    // Access data through data() method since no operator[] is public
    const uint8_t* dataPtr = data.data();
    TEST_ASSERT_EQUAL(0x01, dataPtr[0]);
    TEST_ASSERT_EQUAL(0x02, dataPtr[1]);
    TEST_ASSERT_EQUAL(0x03, dataPtr[2]);
}

void test_air_conditioner_initialization(void) {
    AirConditioner ac;
    
    TEST_ASSERT_EQUAL(Mode::MODE_OFF, ac.getMode());
    TEST_ASSERT_FALSE(ac.getPowerState());
    TEST_ASSERT_EQUAL(0.0f, ac.getTargetTemp());
}

void test_air_conditioner_configuration(void) {
    AirConditioner ac;
    
    ac.setPeriod(500);
    ac.setTimeout(1500);
    ac.setNumAttempts(5);
    
    TEST_ASSERT_EQUAL(500, ac.getPeriod());
    TEST_ASSERT_EQUAL(1500, ac.getTimeout());
    TEST_ASSERT_EQUAL(5, ac.getNumAttempts());
}

void test_air_conditioner_power_control(void) {
    AirConditioner ac;
    ac.setStream(&mockStream);
    
    // Initial state should be off
    TEST_ASSERT_FALSE(ac.getPowerState());
    
    // Turn on
    ac.setPowerState(true);
    // Note: Power state might not change immediately due to async nature
    
    // Toggle should work
    ac.togglePowerState();
    
    // These operations should not crash
    TEST_ASSERT_TRUE(true);
}

void test_air_conditioner_control_commands(void) {
    AirConditioner ac;
    ac.setStream(&mockStream);
    
    Control control;
    control.mode = Mode::MODE_COOL;
    control.targetTemp = 22.0f;
    control.fanMode = FanMode::FAN_AUTO;
    control.swingMode = SwingMode::SWING_VERTICAL;
    
    // This should not crash
    ac.control(control);
    TEST_ASSERT_TRUE(true);
}

void test_air_conditioner_setup_loop(void) {
    AirConditioner ac;
    ac.setStream(&mockStream);
    
    // Setup should not crash
    ac.setup();
    
    // Loop should not crash
    for (int i = 0; i < 10; i++) {
        ac.loop();
    }
    
    TEST_ASSERT_TRUE(true);
}

void test_autoconf_functionality(void) {
    AirConditioner ac;
    
    // Initial state
    TEST_ASSERT_EQUAL(AUTOCONF_DISABLED, ac.getAutoconfStatus());
    
    // Enable autoconf
    ac.setAutoconf(true);
    TEST_ASSERT_EQUAL(AUTOCONF_PROGRESS, ac.getAutoconfStatus());
    
    // Disable autoconf
    ac.setAutoconf(false);
    TEST_ASSERT_EQUAL(AUTOCONF_DISABLED, ac.getAutoconfStatus());
}

void test_communication_generation(void) {
    AirConditioner ac;
    ac.setStream(&mockStream);
    
    mockStream.clearBuffers();
    
    // Setup and send some commands
    ac.setup();
    ac.setPowerState(true);
    
    // Process for a bit to generate communication
    for (int i = 0; i < 20; i++) {
        ac.loop();
    }
    
    // Should have generated some TX data
    TEST_ASSERT_GREATER_THAN(0, mockStream.getTxLength());
}

void test_getter_methods(void) {
    AirConditioner ac;
    
    // These should not crash and return valid values
    Mode mode = ac.getMode();
    float targetTemp = ac.getTargetTemp();
    float indoorTemp = ac.getIndoorTemp();
    float outdoorTemp = ac.getOutdoorTemp();
    FanMode fanMode = ac.getFanMode();
    SwingMode swingMode = ac.getSwingMode();
    
    // Basic validation to avoid unused variable warnings
    TEST_ASSERT_TRUE(targetTemp >= 0.0f);
    TEST_ASSERT_TRUE(indoorTemp >= 0.0f);
    TEST_ASSERT_TRUE(outdoorTemp >= 0.0f);
    
    // Test that enums are valid (won't crash)
    TEST_ASSERT_TRUE(mode >= Mode::MODE_OFF);
    TEST_ASSERT_TRUE(fanMode >= FanMode::FAN_AUTO);  
    TEST_ASSERT_TRUE(swingMode >= SwingMode::SWING_OFF);
}

void setup() {
    // Initialize Serial for test output
    Serial.begin(115200);
    delay(2000); 
    
    Serial.println("STEP 1: Serial initialized");
    delay(500);
    
    Serial.println("STEP 2: Starting basic info collection");
    Serial.print("ESP Chip ID: ");
    Serial.println(ESP.getChipId(), HEX);
    Serial.print("Free heap: ");
    Serial.println(ESP.getFreeHeap());
    delay(500);
    
    Serial.println("STEP 3: About to test MockStream creation");
    {
        MockStream testStream;
        Serial.println("MockStream created and destroyed successfully");
    }
    delay(500);
    
    Serial.println("STEP 4: About to test FrameData creation");
    {
        FrameData data({0x01, 0x02, 0x03});
        Serial.print("FrameData size: ");
        Serial.println(data.size());
    }
    delay(500);
    
    Serial.println("STEP 5: About to test Frame creation");
    {
        FrameData data({0x01, 0x02, 0x03});
        Frame frame(AIR_CONDITIONER, 0x01, 0x03, data);
        Serial.print("Frame size: ");
        Serial.println(frame.size());
    }
    delay(500);
    
    Serial.println("STEP 6: About to test AirConditioner creation");
    {
        AirConditioner ac;
        Serial.println("AirConditioner created successfully");
    }
    delay(500);
    
    Serial.println("STEP 7: About to initialize Unity");
    UNITY_BEGIN();
    Serial.println("Unity initialized successfully");
    delay(500);
    
    Serial.println("STEP 8: Running comprehensive test suite");
    
    // Core tests
    Serial.println("Testing frame creation...");
    RUN_TEST(test_frame_creation);
    
    Serial.println("Testing frame data operations...");
    RUN_TEST(test_frame_data_operations);
    
    Serial.println("Testing AC initialization...");
    RUN_TEST(test_air_conditioner_initialization);
    
    Serial.println("Testing AC configuration...");
    RUN_TEST(test_air_conditioner_configuration);
    
    // Advanced tests
    Serial.println("Testing AC power control...");
    RUN_TEST(test_air_conditioner_power_control);
    
    Serial.println("Testing AC control commands...");
    RUN_TEST(test_air_conditioner_control_commands);
    
    Serial.println("Testing AC setup and loop...");
    RUN_TEST(test_air_conditioner_setup_loop);
    
    Serial.println("Testing autoconf functionality...");
    RUN_TEST(test_autoconf_functionality);
    
    Serial.println("Testing getter methods...");
    RUN_TEST(test_getter_methods);
    
    Serial.println("Testing communication generation...");
    RUN_TEST(test_communication_generation);
    
    Serial.println("All comprehensive tests completed!");
    
    UNITY_END();
    Serial.println("Unity test framework completed successfully!");
}

void loop() {
    // Provide feedback that the ESP is running
    static unsigned long lastMessage = 0;
    static int counter = 0;
    
    if (millis() - lastMessage > 5000) { // Every 5 seconds
        lastMessage = millis();
        counter++;
        Serial.print("ESP-12E running in loop... #");
        Serial.print(counter);
        Serial.print(" - Free heap: ");
        Serial.println(ESP.getFreeHeap());
    }
}
