#include <LovyanGFX.hpp>
#include "UIManager.hpp"
#include "Button.hpp"
#include "FontSelectorGUI.hpp"
#include "ReflowGUI.hpp"


LGFX display;
//This display instance is for the LGFX library. 
//It will be used for all classes that want to interact with the display.
UIManager uiManager(display);
// Create UIManager instance that handles the touchscreen interface,
// manages multiple screens (main and settings),
// also creates the buttons and handles the touch events and actions
// also manages switching lightmode and dark mode

// Create an instance of FontSelectorGUI with the required parameters
FontSelectorGUI _FontSelectorGUI(display, uiManager);


void testAction() {
    Serial.println("testAction called");
}

void setup(){
  
  Serial.println("Font Selector Begin");
  // setup the controller
  //UI manager test
  uiManager.createButton(10, display.height() - 60, 60, 40, 8, 
                          TFT_DARKGREY, TFT_WHITE, "<", 0, testAction);
  


  _FontSelectorGUI.setup();
}

void loop() {
  // Run the controller's main loop
  //FontSelectorGUI.loop();
}

