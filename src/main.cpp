#include <Arduino.h>
#include "ReflowController.hpp"

// Create the main controller instance
ReflowController controller;

void setup() {
  // setup the controller
  controller.setup();
}

void loop() {
  // Run the controller's main loop
  controller.loop();
}