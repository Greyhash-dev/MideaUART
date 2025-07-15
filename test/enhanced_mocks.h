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
        
        // Find command start (0xAA)
        for (size_t i = 0; i <= txBuffer.length() - 3; i++) {
            if ((uint8_t)txBuffer[i] == 0xAA) {
                // Check if we have enough bytes for the length field
                if (i + 1 < txBuffer.length()) {
                    uint8_t len = (uint8_t)txBuffer[i + 1];
                    
                    // Check if we have the complete command
                    if (len >= 8 && len <= 32 && i + len + 1 <= txBuffer.length()) {
                        // Process this command
                        processCommandAt(i);
                        
                        // Remove processed command from buffer
                        txBuffer = txBuffer.substring(i + len + 1);
                        return; // Process one command at a time
                    }
                }
            }
        }
    }
    
    void processCommandAt(size_t pos) {
        uint8_t len = (uint8_t)txBuffer[pos + 1];
        uint8_t cmdType = (uint8_t)txBuffer[pos + 2];
        
        // Always send a response, regardless of command type
        bool responseGenerated = false;
        
        // Simulate different types of responses based on command type
        switch (cmdType) {
            case 0x20: // Device info request
                sendDeviceInfoResponse();
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
                sendStatusResponse();
                responseGenerated = true;
                break;
                
            case 0x41: // Status request
                sendStatusResponse();
                responseGenerated = true;
                break;
                
            default:
                // Send generic ACK for any unknown command
                sendAckResponse();
                responseGenerated = true;
                break;
        }
        
        // For debugging: always ensure we generate some response
        if (!responseGenerated) {
            sendAckResponse();
        }
    }
    
    void sendDeviceInfoResponse() {
        // Simulate device info response
        uint8_t response[] = {
            0xAA, 0x0E, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E
        };
        
        addRxBytes(response, sizeof(response));
    }
    
    void sendStatusResponse() {
        // Create realistic status response based on current simulated state
        uint8_t response[23] = {
            0xAA, 0x15, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        };
        
        // Set power state (bit 0 of byte 8)
        if (powerState) response[8] |= 0x01;
        
        // Set target temperature (bits 0-3 of byte 9)
        response[9] = (targetTemp - 16) & 0x0F;
        
        // Set mode (bits 5-7 of byte 10)
        response[10] = (mode << 5) & 0xE0;
        
        // Set fan speed (bits 1-3 of byte 10)
        response[10] |= (fanSpeed << 1) & 0x0E;
        
        // Set swing (bit 0 of byte 10)
        if (swing) response[10] |= 0x01;
        
        // Set current temperature (byte 11)
        response[11] = currentTemp;
        
        // Calculate simple checksum for last byte
        uint8_t checksum = 0;
        for (int i = 0; i < 22; i++) {
            checksum += response[i];
        }
        response[22] = (~checksum + 1) & 0xFF;
        
        addRxBytes(response, sizeof(response));
    }
    
    void sendAckResponse() {
        // Simple ACK response
        uint8_t ack[] = {0xAA, 0x06, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x7A};
        addRxBytes(ack, sizeof(ack));
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
