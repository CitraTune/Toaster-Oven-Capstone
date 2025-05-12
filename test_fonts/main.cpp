#include <LovyanGFX.hpp>
//#include "UIManager.hpp"
#include "Button.hpp"
#include "FontSelector.hpp"
#include <ReflowController.hpp>


void setup() {
  // Initialize the controller
  FontSelector.init();
}

void loop() {
  // Run the controller's main loop
  FontSelector.loop();
}