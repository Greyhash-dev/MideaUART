#ifndef UNITY_CONFIG_H
#define UNITY_CONFIG_H

// Unity configuration for ESP8266/ESP32 testing

// Basic Unity configuration
#define UNITY_EXCLUDE_FLOAT
#define UNITY_EXCLUDE_DOUBLE
#define UNITY_EXCLUDE_TIME_H

// Memory configuration for embedded systems
#define UNITY_EXCLUDE_SETJMP_H

// Disable Unity's default output functions - we'll handle them in test files
#define UNITY_OUTPUT_CHAR(a)
#define UNITY_OUTPUT_FLUSH()
#define UNITY_OUTPUT_START()
#define UNITY_OUTPUT_COMPLETE()

// Test runner configuration
#define UNITY_EXCLUDE_DETAILS
#define UNITY_EXCLUDE_EXEC_TIME

// ESP8266/ESP32 specific configurations
#ifdef ESP8266
    #define UNITY_INT_WIDTH 32
    #define UNITY_LONG_WIDTH 32
    #define UNITY_POINTER_WIDTH 32
#endif

#ifdef ESP32
    #define UNITY_INT_WIDTH 32
    #define UNITY_LONG_WIDTH 32
    #define UNITY_POINTER_WIDTH 32
#endif

// For native testing
#ifdef NATIVE_TEST
    #include <stdio.h>
    #undef UNITY_OUTPUT_CHAR
    #undef UNITY_OUTPUT_FLUSH
    #undef UNITY_OUTPUT_START
    #undef UNITY_OUTPUT_COMPLETE
    #define UNITY_OUTPUT_CHAR(a)    putchar(a)
    #define UNITY_OUTPUT_FLUSH()    fflush(stdout)
    #define UNITY_OUTPUT_START()    
    #define UNITY_OUTPUT_COMPLETE() fflush(stdout)
#endif

#endif /* UNITY_CONFIG_H */
