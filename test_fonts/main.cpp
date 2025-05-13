#include <LovyanGFX.hpp>
// #include "UIManager.hpp"
#include "Button.hpp"
#include "FontSelector.hpp"
#include <ReflowController.hpp>

// You need to create or access LGFX and UIManager objects first
LGFX display;           // Create display instance
//This display instance is for the LGFX library. 
//It will be used for all classes that want to interact with the display.
UIManager uiManager(display);    
// Create UIManager instance that handles the touchscreen interface,
// manages multiple screens (main and settings),
// also creates the buttons and handles the touch events and actions
// also manages switching lightmode and dark mode

// Create an instance of FontSelector with the required parameters
FontSelector fontSelector(display, uiManager);

void setup() {
  // setup the controller
  fontSelector.setup(); 
}

void loop() {
  // Run the controller's main loop
  fontSelector.loop();
}

