// Demo main file - includes the actual demo implementation
// This approach allows the library to work properly when imported into other projects

#ifdef DEMO_MODE
// Include the demo implementation
#include "../examples/demo/demo_main.cpp"
#else
// When used as library, provide empty implementation
void setup() {}
void loop() {}
#endif
