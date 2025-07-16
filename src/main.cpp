// Demo main file - includes the actual demo implementation
// This approach allows the library to work properly when imported into other projects

#ifdef DEMO_MODE
// Include the demo implementation
#include "../examples/demo/demo_main.cpp"
#elif !defined(UNIT_TEST)
// When used as library (but not in unit tests), provide empty implementation
void setup() {}
void loop() {}
#endif
// When UNIT_TEST is defined, don't provide setup/loop - let the test file handle it
