#include <Arduino.h>
#include "ReflowController.hpp"

// Create the main controller instance
ReflowController controller;

void setup() {
  // Initialize the controller
  controller.init();
}

void loop() {
  // Run the controller's main loop
  controller.loop();
}