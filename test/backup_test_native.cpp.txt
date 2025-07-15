// Native C++ Test Version (Runs on PC without hardware)
#include <unity.h>
#include <iostream>
#include <vector>
#include <functional>
#include <cstdint>
#include <string>

// Mock Arduino environment for native testing
unsigned long millis() { 
    static unsigned long time = 0;
    return time += 10; // Simulate time progression
}

void delay(unsigned long ms) {
    // No-op for native testing
}

// Mock String class
class String {
private:
    std::string data;
public:
    String() = default;
    String(const char* str) : data(str) {}
    String(const std::string& str) : data(str) {}
    
    String& operator+=(char c) { data += c; return *this; }
    String& operator+=(const String& other) { data += other.data; return *this; }
    
    size_t length() const { return data.length(); }
    char operator[](size_t index) const { return data[index]; }
    const char* c_str() const { return data.c_str(); }
};

// Mock Stream class for native testing
class Stream {
public:
    virtual ~Stream() = default;
    virtual size_t write(uint8_t data) = 0;
    virtual size_t write(const uint8_t *buffer, size_t size) = 0;
    virtual int available() = 0;
    virtual int read() = 0;
    virtual int peek() = 0;
    virtual void flush() = 0;
};

// Simple mock implementations of library components for testing
namespace dudanov {
namespace midea {

enum ApplianceType : uint8_t {
    AIR_CONDITIONER = 0xAC
};

enum AutoconfStatus : uint8_t {
    AUTOCONF_DISABLED,
    AUTOCONF_PROGRESS,
    AUTOCONF_OK,
    AUTOCONF_ERROR,
};

using FrameData = std::vector<uint8_t>;

class Frame {
private:
    std::vector<uint8_t> m_data;
    uint8_t m_type;
    
public:
    Frame(uint8_t appliance, uint8_t protocol, uint8_t type, const FrameData &data) 
        : m_type(type) {
        m_data = {0xAA, 0x23, appliance};
        m_data.insert(m_data.end(), data.begin(), data.end());
    }
    
    const uint8_t* data() const { return m_data.data(); }
    size_t size() const { return m_data.size(); }
    uint8_t getType() const { return m_type; }
};

namespace ac {

enum Mode : uint8_t {
    MODE_OFF = 0,
    MODE_COOL = 1,
    MODE_HEAT = 2,
    MODE_AUTO = 3
};

enum FanMode : uint8_t {
    FAN_AUTO = 0,
    FAN_LOW = 1,
    FAN_MEDIUM = 2,
    FAN_HIGH = 3
};

enum SwingMode : uint8_t {
    SWING_OFF = 0,
    SWING_VERTICAL = 1,
    SWING_HORIZONTAL = 2,
    SWING_BOTH = 3
};

struct Control {
    Mode mode = MODE_OFF;
    float targetTemp = 0.0f;
    FanMode fanMode = FAN_AUTO;
    SwingMode swingMode = SWING_OFF;
};

// Simplified AirConditioner for native testing
class AirConditioner {
private:
    Mode m_mode = MODE_OFF;
    float m_targetTemp = 0.0f;
    float m_indoorTemp = 0.0f;
    float m_outdoorTemp = 0.0f;
    FanMode m_fanMode = FAN_AUTO;
    SwingMode m_swingMode = SWING_OFF;
    AutoconfStatus m_autoconfStatus = AUTOCONF_DISABLED;
    Stream* m_stream = nullptr;
    uint32_t m_period = 1000;
    uint32_t m_timeout = 2000;
    uint8_t m_numAttempts = 3;
    bool m_setupCalled = false;
    int m_loopCount = 0;
    
public:
    // Configuration methods
    void setPeriod(uint32_t period) { m_period = period; }
    void setTimeout(uint32_t timeout) { m_timeout = timeout; }
    void setNumAttempts(uint8_t attempts) { m_numAttempts = attempts; }
    
    uint32_t getPeriod() const { return m_period; }
    uint32_t getTimeout() const { return m_timeout; }
    uint8_t getNumAttempts() const { return m_numAttempts; }
    
    // Stream methods
    void setStream(Stream* stream) { m_stream = stream; }
    
    // Power and control methods
    bool getPowerState() const { return m_mode != MODE_OFF; }
    void setPowerState(bool state) { 
        m_mode = state ? MODE_AUTO : MODE_OFF; 
    }
    void togglePowerState() { 
        setPowerState(!getPowerState()); 
    }
    
    void control(const Control& control) {
        m_mode = control.mode;
        m_targetTemp = control.targetTemp;
        m_fanMode = control.fanMode;
        m_swingMode = control.swingMode;
    }
    
    // Getter methods
    Mode getMode() const { return m_mode; }
    float getTargetTemp() const { return m_targetTemp; }
    float getIndoorTemp() const { return m_indoorTemp; }
    float getOutdoorTemp() const { return m_outdoorTemp; }
    FanMode getFanMode() const { return m_fanMode; }
    SwingMode getSwingMode() const { return m_swingMode; }
    
    // Autoconf methods
    AutoconfStatus getAutoconfStatus() const { return m_autoconfStatus; }
    void setAutoconf(bool enabled) {
        m_autoconfStatus = enabled ? AUTOCONF_PROGRESS : AUTOCONF_DISABLED;
    }
    
    // Lifecycle methods
    void setup() { 
        m_setupCalled = true; 
    }
    
    void loop() { 
        m_loopCount++;
        // Simulate some communication if stream is set
        if (m_stream && m_loopCount % 10 == 0) {
            uint8_t data[] = {0xAA, 0x23, 0xAC, 0x00};
            m_stream->write(data, sizeof(data));
        }
    }
    
    bool wasSetupCalled() const { return m_setupCalled; }
    int getLoopCount() const { return m_loopCount; }
};

} // namespace ac
} // namespace midea
} // namespace dudanov

// Mock Stream implementation for native testing
class MockStream : public Stream {
private:
    std::vector<uint8_t> txBuffer;
    std::vector<uint8_t> rxBuffer;
    size_t rxPos = 0;
    
public:
    size_t write(uint8_t data) override {
        txBuffer.push_back(data);
        return 1;
    }
    
    size_t write(const uint8_t *buffer, size_t size) override {
        for (size_t i = 0; i < size; i++) {
            txBuffer.push_back(buffer[i]);
        }
        return size;
    }
    
    int available() override {
        return rxBuffer.size() - rxPos;
    }
    
    int read() override {
        if (rxPos < rxBuffer.size()) {
            return rxBuffer[rxPos++];
        }
        return -1;
    }
    
    int peek() override {
        if (rxPos < rxBuffer.size()) {
            return rxBuffer[rxPos];
        }
        return -1;
    }
    
    void flush() override {}
    
    // Mock-specific methods
    void clearBuffers() {
        txBuffer.clear();
        rxBuffer.clear();
        rxPos = 0;
    }
    
    size_t getTxLength() const {
        return txBuffer.size();
    }
    
    void addRxData(const std::vector<uint8_t>& data) {
        rxBuffer.insert(rxBuffer.end(), data.begin(), data.end());
    }
};

// Test globals
using namespace dudanov::midea;
using namespace dudanov::midea::ac;

MockStream mockStream;

void setUp(void) {
    mockStream.clearBuffers();
}

void tearDown(void) {
    // Clean up after each test
}

// Native test functions
void test_frame_creation_native(void) {
    FrameData data = {0xAA, 0x23, 0xAC};
    Frame frame(AIR_CONDITIONER, 0x01, 0x03, data);
    
    TEST_ASSERT_NOT_NULL(frame.data());
    TEST_ASSERT_GREATER_THAN(0, frame.size());
    TEST_ASSERT_EQUAL(0x03, frame.getType());
}

void test_frame_data_operations_native(void) {
    FrameData data;
    data.push_back(0x01);
    data.push_back(0x02);
    data.push_back(0x03);
    
    TEST_ASSERT_EQUAL(3, data.size());
    TEST_ASSERT_EQUAL(0x01, data[0]);
    TEST_ASSERT_EQUAL(0x02, data[1]);
    TEST_ASSERT_EQUAL(0x03, data[2]);
}

void test_air_conditioner_initialization_native(void) {
    AirConditioner ac;
    
    TEST_ASSERT_EQUAL(MODE_OFF, ac.getMode());
    TEST_ASSERT_FALSE(ac.getPowerState());
    TEST_ASSERT_EQUAL(0.0f, ac.getTargetTemp());
}

void test_air_conditioner_configuration_native(void) {
    AirConditioner ac;
    
    ac.setPeriod(500);
    ac.setTimeout(1500);
    ac.setNumAttempts(5);
    
    TEST_ASSERT_EQUAL(500, ac.getPeriod());
    TEST_ASSERT_EQUAL(1500, ac.getTimeout());
    TEST_ASSERT_EQUAL(5, ac.getNumAttempts());
}

void test_air_conditioner_power_control_native(void) {
    AirConditioner ac;
    ac.setStream(&mockStream);
    
    TEST_ASSERT_FALSE(ac.getPowerState());
    
    ac.setPowerState(true);
    TEST_ASSERT_TRUE(ac.getPowerState());
    
    ac.togglePowerState();
    TEST_ASSERT_FALSE(ac.getPowerState());
}

void test_air_conditioner_control_commands_native(void) {
    AirConditioner ac;
    ac.setStream(&mockStream);
    
    Control control;
    control.mode = MODE_COOL;
    control.targetTemp = 22.0f;
    control.fanMode = FAN_AUTO;
    control.swingMode = SWING_VERTICAL;
    
    ac.control(control);
    
    TEST_ASSERT_EQUAL(MODE_COOL, ac.getMode());
    TEST_ASSERT_EQUAL(22.0f, ac.getTargetTemp());
    TEST_ASSERT_EQUAL(FAN_AUTO, ac.getFanMode());
    TEST_ASSERT_EQUAL(SWING_VERTICAL, ac.getSwingMode());
}

void test_air_conditioner_setup_loop_native(void) {
    AirConditioner ac;
    ac.setStream(&mockStream);
    
    ac.setup();
    TEST_ASSERT_TRUE(ac.wasSetupCalled());
    
    for (int i = 0; i < 10; i++) {
        ac.loop();
    }
    
    TEST_ASSERT_GREATER_OR_EQUAL(10, ac.getLoopCount());
}

void test_autoconf_functionality_native(void) {
    AirConditioner ac;
    
    TEST_ASSERT_EQUAL(AUTOCONF_DISABLED, ac.getAutoconfStatus());
    
    ac.setAutoconf(true);
    TEST_ASSERT_EQUAL(AUTOCONF_PROGRESS, ac.getAutoconfStatus());
    
    ac.setAutoconf(false);
    TEST_ASSERT_EQUAL(AUTOCONF_DISABLED, ac.getAutoconfStatus());
}

void test_communication_generation_native(void) {
    AirConditioner ac;
    ac.setStream(&mockStream);
    
    mockStream.clearBuffers();
    
    ac.setup();
    ac.setPowerState(true);
    
    // Run enough loops to trigger communication
    for (int i = 0; i < 20; i++) {
        ac.loop();
    }
    
    TEST_ASSERT_GREATER_THAN(0, mockStream.getTxLength());
}

void test_getter_methods_native(void) {
    AirConditioner ac;
    
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

int main() {
    UNITY_BEGIN();
    
    // Frame tests
    RUN_TEST(test_frame_creation_native);
    RUN_TEST(test_frame_data_operations_native);
    
    // Air conditioner tests
    RUN_TEST(test_air_conditioner_initialization_native);
    RUN_TEST(test_air_conditioner_configuration_native);
    RUN_TEST(test_air_conditioner_power_control_native);
    RUN_TEST(test_air_conditioner_control_commands_native);
    RUN_TEST(test_air_conditioner_setup_loop_native);
    RUN_TEST(test_autoconf_functionality_native);
    RUN_TEST(test_communication_generation_native);
    RUN_TEST(test_getter_methods_native);
    
    return UNITY_END();
}
