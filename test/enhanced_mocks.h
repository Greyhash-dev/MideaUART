#pragma once
#include <Arduino.h>
#include <String.h>

/**
 * Enhanced Mock Stream implementation that simulates Midea AC responses
 */
class MockACStream : public Stream {
private:
    String txBuffer;
    String rxBuffer;
    size_t rxPos = 0;
    
    // Simulated AC state
    bool powerState = false;
    uint8_t targetTemp = 22;
    uint8_t currentTemp = 25;
    uint8_t mode = 0; // 0=Auto, 1=Cool, 2=Dry, 3=Heat, 4=Fan
    uint8_t fanSpeed = 0; // 0=Auto, 1=Low, 2=Med, 3=High
    bool swing = false;
    
    // Auto-response feature
    bool autoResponse = true;
    
public:
    // Stream interface implementation
    size_t write(uint8_t data) override {
        txBuffer += (char)data;
        
        // If auto-response is enabled, check for complete commands and respond
        if (autoResponse) {
            processCommand();
        }
        
        return 1;
    }
    
    size_t write(const uint8_t *buffer, size_t size) override {
        for (size_t i = 0; i < size; i++) {
            write(buffer[i]);
        }
        return size;
    }
    
    int available() override {
        return rxBuffer.length() - rxPos;
    }
    
    int read() override {
        if (rxPos < rxBuffer.length()) {
            return rxBuffer[rxPos++];
        }
        return -1;
    }
    
    int peek() override {
        if (rxPos < rxBuffer.length()) {
            return rxBuffer[rxPos];
        }
        return -1;
    }
    
    void flush() override {
        // No-op for mock
    }
    
    // Mock-specific methods for testing
    void addRxData(const String& data) {
        rxBuffer += data;
    }
    
    void addRxBytes(const uint8_t* data, size_t length) {
        for (size_t i = 0; i < length; i++) {
            rxBuffer += (char)data[i];
        }
    }
    
    String getTxBuffer() const {
        return txBuffer;
    }
    
    void clearBuffers() {
        txBuffer = "";
        rxBuffer = "";
        rxPos = 0;
    }
    
    size_t getTxLength() const {
        return txBuffer.length();
    }
    
    size_t getRxLength() const {
        return rxBuffer.length() - rxPos;
    }
    
    // Enhanced AC simulation methods
    void setAutoResponse(bool enabled) {
        autoResponse = enabled;
    }
    
    void setSimulatedACState(bool power, uint8_t temp, uint8_t acMode) {
        powerState = power;
        targetTemp = temp;
        mode = acMode;
    }
    
    void setSimulatedCurrentTemp(uint8_t temp) {
        currentTemp = temp;
    }
    
    // Force a response for testing
    void triggerStatusResponse() {
        sendStatusResponse();
    }
    
    // Force command generation by checking if we need to send periodic status requests
    void triggerPeriodicCommand() {
        // Generate a status request command that the AC library would normally send
        uint8_t statusCmd[] = {0xAA, 0x0B, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5E};
        for (size_t i = 0; i < sizeof(statusCmd); i++) {
            txBuffer += (char)statusCmd[i];
        }
        // Process the command to generate response
        processCommand();
    }
    
    // Check if specific bytes were transmitted
    bool wasBytesSent(const uint8_t* expected, size_t length) const {
        if (length > txBuffer.length()) return false;
        
        for (size_t i = 0; i <= txBuffer.length() - length; i++) {
            bool match = true;
            for (size_t j = 0; j < length; j++) {
                if ((uint8_t)txBuffer[i + j] != expected[j]) {
                    match = false;
                    break;
                }
            }
            if (match) return true;
        }
        return false;
    }
    
private:
    void processCommand() {
        // Look for Midea command patterns in TX buffer
        // Midea commands typically start with 0xAA and have specific structure
        
        if (txBuffer.length() < 3) return; // Need at least header
        
        // Debug: Show what's in the TX buffer
        if (txBuffer.length() > 0) {
            Serial.print("   🔧 TX Buffer len=");
            Serial.print(txBuffer.length());
            Serial.print(": ");
            for (int i = 0; i < min(16, (int)txBuffer.length()); i++) {
                Serial.print("0x");
                if ((uint8_t)txBuffer[i] < 0x10) Serial.print("0");
                Serial.print((uint8_t)txBuffer[i], HEX);
                Serial.print(" ");
            }
            if (txBuffer.length() > 16) Serial.print("...");
            Serial.println();
        }
        
        // Find command start (0xAA)
        for (size_t i = 0; i <= txBuffer.length() - 3; i++) {
            if ((uint8_t)txBuffer[i] == 0xAA) {
                // Check if we have enough bytes for the length field
                if (i + 1 < txBuffer.length()) {
                    uint8_t len = (uint8_t)txBuffer[i + 1];
                    
                    Serial.print("   🔧 Found 0xAA at pos ");
                    Serial.print(i);
                    Serial.print(", len=");
                    Serial.println(len);
                    
                    // Check if we have the complete command
                    if (len >= 8 && len <= 50 && i + len + 1 <= txBuffer.length()) {
                        // Process this command
                        Serial.println("   🔧 Processing command...");
                        processCommandAt(i);
                        
                        // Remove processed command from buffer
                        txBuffer = txBuffer.substring(i + len + 1);
                        return; // Process one command at a time
                    } else {
                        Serial.print("   🔧 Command incomplete: len=");
                        Serial.print(len);
                        Serial.print(", bufLen=");
                        Serial.print(txBuffer.length());
                        Serial.print(", needed=");
                        Serial.println(i + len + 1);
                    }
                }
            }
        }
    }
    
    void processCommandAt(size_t pos) {
        uint8_t len = (uint8_t)txBuffer[pos + 1];
        uint8_t cmdType = (uint8_t)txBuffer[pos + 2];
        uint8_t frameType = 0x03; // Default to DEVICE_QUERY response
        
        // Extract frame type from incoming command if available
        if (len >= 10 && pos + 9 < txBuffer.length()) {
            frameType = (uint8_t)txBuffer[pos + 9]; // OFFSET_TYPE = 9
        }
        
        // Debug: Log the extracted frame information and full frame
        Serial.print("   🔍 Mock RX: AppType=0x");
        Serial.print(cmdType, HEX);
        Serial.print(", FrameType=0x");
        Serial.print(frameType, HEX);
        Serial.print(", Len=");
        Serial.print(len);
        Serial.print(" | Full: ");
        for (int i = 0; i < min(12, (int)min(len + 1, (int)(txBuffer.length() - pos))); i++) {
            Serial.print("0x");
            if ((uint8_t)txBuffer[pos + i] < 0x10) Serial.print("0");
            Serial.print((uint8_t)txBuffer[pos + i], HEX);
            Serial.print(" ");
        }
        Serial.println();
        
        // Always send a response, regardless of command type
        bool responseGenerated = false;
        
        // Simulate different types of responses based on command type
        switch (cmdType) {
            case 0x20: // Device info request
                sendDeviceInfoResponse(frameType);
                responseGenerated = true;
                break;
                
            case 0x40: // Set command
                if (len >= 10) {
                    // Extract command data and update simulated state
                    uint8_t cmd1 = (uint8_t)txBuffer[pos + 5];
                    uint8_t cmd2 = (uint8_t)txBuffer[pos + 6];
                    
                    // Update simulated AC state based on command
                    if (cmd1 & 0x01) powerState = true;
                    else powerState = false;
                    
                    if ((cmd1 & 0x0F) != 0x0F) {
                        targetTemp = (cmd1 & 0x0F) + 16; // Typical temp encoding
                    }
                    
                    mode = (cmd2 >> 5) & 0x07; // Mode in upper bits
                    fanSpeed = (cmd2 >> 1) & 0x07; // Fan speed
                    swing = (cmd2 & 0x01) != 0; // Swing in LSB
                }
                sendStatusResponse(frameType);
                responseGenerated = true;
                break;
                
            case 0x41: // Status request
                sendStatusResponse(frameType);
                responseGenerated = true;
                break;
                
            default:
                // Send generic ACK for any unknown command
                sendAckResponse(frameType);
                responseGenerated = true;
                break;
        }
        
        // For debugging: always ensure we generate some response
        if (!responseGenerated) {
            sendAckResponse(frameType);
        }
    }
    
    void sendDeviceInfoResponse(uint8_t frameType = 0x03) {
        // Create minimal valid device info response
        uint8_t response[11] = {
            0xAA,        // START_BYTE
            0x0B,        // LENGTH (11 bytes total)
            0xAC,        // APPTYPE (AIR_CONDITIONER)
            0x00,        // SYNC (will be calculated)
            0x00, 0x00, 0x00, 0x00,  // Reserved bytes
            0x01,        // PROTOCOL
            frameType,   // TYPE (echo the request type)
            0x00         // CHECKSUM (will be calculated)
        };
        
        // Set sync field (length ^ apptype)
        response[3] = response[1] ^ response[2];  // 0x0B ^ 0xAC
        
        // Calculate checksum using library's method
        uint8_t checksum = 0;
        for (int i = 1; i < 10; i++) {  // Start from length field, exclude checksum
            checksum -= response[i];
        }
        response[10] = checksum;
        
        addRxBytes(response, sizeof(response));
    }
    
    void sendStatusResponse(uint8_t frameType = 0x03) {
        // Create hardcoded minimal valid response
        uint8_t response[] = {0xAA, 0x0B, 0xAC, 0xA7, 0x00, 0x00, 0x00, 0x00, 0x01, frameType, 0x9A};
        
        // Recalculate checksum for the actual frameType
        uint8_t checksum = 0;
        for (int i = 1; i < 10; i++) {
            checksum -= response[i];
        }
        response[10] = checksum;
        
        // Debug: Log the response being sent
        Serial.print("   📤 Mock TX: FrameType=0x");
        Serial.print(frameType, HEX);
        Serial.print(", Checksum=0x");
        Serial.println(checksum, HEX);
        
        addRxBytes(response, sizeof(response));
    }
    
    void sendAckResponse(uint8_t frameType = 0x03) {
        // Create minimal valid ACK response
        uint8_t response[11] = {
            0xAA,        // START_BYTE
            0x0B,        // LENGTH (11 bytes total)
            0xAC,        // APPTYPE (AIR_CONDITIONER)
            0x00,        // SYNC (will be calculated)
            0x00, 0x00, 0x00, 0x00,  // Reserved bytes
            0x01,        // PROTOCOL
            frameType,   // TYPE (echo the request type)
            0x00         // CHECKSUM (will be calculated)
        };
        
        // Set sync field (length ^ apptype)
        response[3] = response[1] ^ response[2];  // 0x0B ^ 0xAC
        
        // Calculate checksum using library's method
        uint8_t checksum = 0;
        for (int i = 1; i < 10; i++) {  // Start from length field, exclude checksum
            checksum -= response[i];
        }
        response[10] = checksum;
        
        addRxBytes(response, sizeof(response));
    }
};

/**
 * Simple Mock Stream for basic testing (original implementation)
 */
class MockStream : public Stream {
private:
    String txBuffer;
    String rxBuffer;
    size_t rxPos = 0;
    
public:
    // Stream interface implementation
    size_t write(uint8_t data) override {
        txBuffer += (char)data;
        return 1;
    }
    
    size_t write(const uint8_t *buffer, size_t size) override {
        for (size_t i = 0; i < size; i++) {
            txBuffer += (char)buffer[i];
        }
        return size;
    }
    
    int available() override {
        return rxBuffer.length() - rxPos;
    }
    
    int read() override {
        if (rxPos < rxBuffer.length()) {
            return rxBuffer[rxPos++];
        }
        return -1;
    }
    
    int peek() override {
        if (rxPos < rxBuffer.length()) {
            return rxBuffer[rxPos];
        }
        return -1;
    }
    
    void flush() override {
        // No-op for mock
    }
    
    // Mock-specific methods for testing
    void addRxData(const String& data) {
        rxBuffer += data;
    }
    
    void addRxBytes(const uint8_t* data, size_t length) {
        for (size_t i = 0; i < length; i++) {
            rxBuffer += (char)data[i];
        }
    }
    
    String getTxBuffer() const {
        return txBuffer;
    }
    
    void clearBuffers() {
        txBuffer = "";
        rxBuffer = "";
        rxPos = 0;
    }
    
    size_t getTxLength() const {
        return txBuffer.length();
    }
    
    // Check if specific bytes were transmitted
    bool wasBytesSent(const uint8_t* expected, size_t length) const {
        if (length > txBuffer.length()) return false;
        
        for (size_t i = 0; i <= txBuffer.length() - length; i++) {
            bool match = true;
            for (size_t j = 0; j < length; j++) {
                if ((uint8_t)txBuffer[i + j] != expected[j]) {
                    match = false;
                    break;
                }
            }
            if (match) return true;
        }
        return false;
    }
};
