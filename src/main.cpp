#include <Arduino.h>
#include "ReflowGUI.hpp"

// Create the main controller instance
ReflowGUI controller;

void setup() {
  // setup the controller
  controller.setup();
}

void loop() {
  // Run the controller's main loop
  controller.loop();
}
