// #include <Arduino.h>
// #include "ReflowGUI.hpp"

// // Create the main controller instance
// ReflowGUI controller;

// void setup() {
//   // setup the controller
//   controller.setup();
// }

// void loop() {
//   // Run the controller's main loop
//   controller.loop();
// }
#include <Arduino.h>
#include "GraphTest.hpp"

GraphTest graphTest;

void setup() {
    Serial.begin(115200);
    graphTest.setup();
}

void loop() {
    graphTest.loop();
}