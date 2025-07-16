#pragma once
#include <Arduino.h>
#include <String.h>

/**
 * Mock Stream implementation for testing without hardware
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
