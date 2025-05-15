#include "ReflowGUI.hpp"

// Pin definitions for capacitive touch
#define TOUCH_SDA 33
#define TOUCH_SCL 32
#define TOUCH_INT 21
#define TOUCH_RST 25

// Global reference to the controller instance
// This is needed for button callbacks
ReflowGUI* g_controller = nullptr;

// Button action callbacks
void goToSettingsCallback() {
  if (g_controller) g_controller->goToSettings();
}

void goToMainCallback() {
  if (g_controller) g_controller->goToMain();
}

void toggleLightModeCallback() {
  if (g_controller) g_controller->toggleLightMode();
}

void toggleInvertAccentCallback() {
  if (g_controller) g_controller->toggleInvertAccent();
}

void toggleGraphSizeCallback() {
  if (g_controller) g_controller->toggleGraphSize();
}

// Temperature callbacks
void increaseSoakTempCoarseCallback() {
  if (g_controller) {
    g_controller->getTempManager()->increaseSoakTempCoarse();
    g_controller->getUIManager()->drawActiveScreen();
  }
}

void decreaseSoakTempCoarseCallback() {
  if (g_controller) {
    g_controller->getTempManager()->decreaseSoakTempCoarse();
    g_controller->getUIManager()->drawActiveScreen();
  }
}

void increaseSoakTempFineCallback() {
  if (g_controller) {
    g_controller->getTempManager()->increaseSoakTempFine();
    g_controller->getUIManager()->drawActiveScreen();
  }
}

void decreaseSoakTempFineCallback() {
  if (g_controller) {
    g_controller->getTempManager()->decreaseSoakTempFine();
    g_controller->getUIManager()->drawActiveScreen();
  }
}

void increaseReflowTempCoarseCallback() {
  if (g_controller) {
    g_controller->getTempManager()->increaseReflowTempCoarse();
    g_controller->getTempManager()->updateReflowTempDisplay(g_controller->getUIManager()->lightMode);
  }
}

void decreaseReflowTempCoarseCallback() {
  if (g_controller) {
    g_controller->getTempManager()->decreaseReflowTempCoarse();
    g_controller->getTempManager()->updateReflowTempDisplay(g_controller->getUIManager()->lightMode);
  }
}

void increaseReflowTempFineCallback() {
  if (g_controller) {
    g_controller->getTempManager()->increaseReflowTempFine();
    g_controller->getTempManager()->updateReflowTempDisplay(g_controller->getUIManager()->lightMode);
  }
}

void decreaseReflowTempFineCallback() {
  if (g_controller) {
    g_controller->getTempManager()->decreaseReflowTempFine();
    g_controller->getTempManager()->updateReflowTempDisplay(g_controller->getUIManager()->lightMode);
  }
}

// Constructor
ReflowGUI::ReflowGUI() 
  : uiManager(display), graphManager(display), tempManager(display) {
  lastTouchTime = 0;
  
  // Set the global reference
  g_controller = this;
}

// setup all components
void ReflowGUI::setup() {
  Serial.begin(115200);
  
  // setup the TFT display
  display.begin();
  display.setRotation(7);     // Adjust based on your display orientation
  display.invertDisplay(true); // Optionally invert colors
  display.setBrightness(128);
  display.setColorDepth(24);
  
  // setup touch controller
  touch.init(TOUCH_SDA, TOUCH_SCL, TOUCH_RST, TOUCH_INT);
  
  // setup UI manager
  uiManager.setup();
  
  // Set up buttons
  setupButtons();
  
  // Start with the main screen
  uiManager.navigateToScreen(SCREEN_MAIN);
}

// Main loop
void ReflowGUI::loop() {
  TOUCHINFO ti;
  
  // Get touch samples
  if (touch.getSamples(&ti)) {
    unsigned long currentTime = millis();
    if (currentTime - lastTouchTime > debounceDelay) {
      lastTouchTime = currentTime;
      
      int x = ti.x[0];
      int y = ti.y[0];
      
      // Debugging output
      Serial.printf("Touch x=%d y=%d\n", x, y);
      
      // Check if any button was pressed
      uiManager.checkButtonPress(x, y);
    }
  }
  
  delay(10);  // Reduced delay for smoother UI
}

// Action methods
void ReflowGUI::goToSettings() {
  uiManager.navigateToScreen(SCREEN_SETTINGS);
  tempManager.displayTemperatures(uiManager.lightMode);
}

void ReflowGUI::goToMain() {
  uiManager.navigateToScreen(SCREEN_MAIN);
  graphManager.draw(uiManager.lightMode);
}

void ReflowGUI::toggleLightMode() {
  uiManager.lightMode = !uiManager.lightMode;
  // Find the light mode button and update its label
  Button* lightModeButton = uiManager.getButton("light_mode_btn");
  if (lightModeButton) {
    lightModeButton->label = !uiManager.lightMode ? "Light Mode" : "Dark Mode";
  }
  uiManager.drawActiveScreen();
  
  // Redraw the graph if on main screen
  if (uiManager.currentScreen == SCREEN_MAIN) {
    graphManager.draw(uiManager.lightMode);
  }
  
  // Update temperature display if on settings screen
  if (uiManager.currentScreen == SCREEN_SETTINGS) {
    tempManager.displayTemperatures(uiManager.lightMode);
  }
}

void ReflowGUI::toggleInvertAccent() {
  uiManager.invertAccent = !uiManager.invertAccent;
  uiManager.updateButtonColors();
  uiManager.drawActiveScreen();
  
  // Redraw the graph if on main screen
  if (uiManager.currentScreen == SCREEN_MAIN) {
    graphManager.draw(uiManager.lightMode);
  }
  
  // Update temperature display if on settings screen
  if (uiManager.currentScreen == SCREEN_SETTINGS) {
    tempManager.displayTemperatures(uiManager.lightMode);
  }
}

void ReflowGUI::toggleGraphSize() {
  graphManager.toggleFullScreen();
  uiManager.drawActiveScreen();
  graphManager.draw(uiManager.lightMode);
}

// Setup interface buttons
void ReflowGUI::setupButtons() {
  // Set font for button sizing calculations
  display.setFont(&lgfx::fonts::FreeSans9pt7b);
  int fontHeight = display.fontHeight();
  int buttonTextMargin = 10; // Margin around text (top and bottom)
  int buttonHeight = fontHeight + 2 * buttonTextMargin; // Dynamic height based on font
  
  // Main screen buttons
  uiManager.createButton(
    "settings_btn",           // key - unique identifier
    (SCREEN_WIDTH - 160) / 2, // x - centered
    SCREEN_HEIGHT - buttonHeight - 10, // y - positioned from bottom with margin
    160,                      // width
    buttonHeight,             // height - dynamically sized
    10,                       // radius - rounded corners
    TFT_ORANGE,               // color - orange
    TFT_WHITE,                // text color
    "Settings",               // label
    SCREEN_MAIN,              // screen
    goToSettingsCallback      // action function
  );
  
  // Settings screen back button
  uiManager.createButton(
    "back_btn",               // key - unique identifier
    (SCREEN_WIDTH - 160) / 2, // x - centered
    SCREEN_HEIGHT - buttonHeight - 10, // y - positioned from bottom with margin
    160,                      // width
    buttonHeight,             // height - dynamically sized
    10,                       // radius - rounded corners
    TFT_ORANGE,               // color - orange
    TFT_WHITE,                // text color
    "Back",                   // label
    SCREEN_SETTINGS,          // screen
    goToMainCallback          // action function
  );
  
  // Temperature controls in settings screen
  int tempBoxX = 130;
  int tempBoxY = 10;
  
  // Soak Temp Controls
  uiManager.createButton("soak_temp_plus10", tempBoxX, tempBoxY, 40, 40, 5, TFT_ORANGE, TFT_WHITE, "+10", 
                       SCREEN_SETTINGS, increaseSoakTempCoarseCallback);
  uiManager.createButton("soak_temp_minus10", tempBoxX, tempBoxY + 50, 40, 40, 5, TFT_ORANGE, TFT_WHITE, "-10", 
                       SCREEN_SETTINGS, decreaseSoakTempCoarseCallback);
  uiManager.createButton("soak_temp_plus1", tempBoxX + 60, tempBoxY, 40, 40, 5, TFT_ORANGE, TFT_WHITE, "+1", 
                       SCREEN_SETTINGS, increaseSoakTempFineCallback);
  uiManager.createButton("soak_temp_minus1", tempBoxX + 60, tempBoxY + 50, 40, 40, 5, TFT_ORANGE, TFT_WHITE, "-1", 
                       SCREEN_SETTINGS, decreaseSoakTempFineCallback);
  
  // Reflow Temp Controls
  tempBoxY += 100;
  uiManager.createButton("reflow_temp_plus10", tempBoxX, tempBoxY, 40, 40, 5, TFT_ORANGE, TFT_WHITE, "+10", 
                       SCREEN_SETTINGS, increaseReflowTempCoarseCallback);
  uiManager.createButton("reflow_temp_minus10", tempBoxX, tempBoxY + 50, 40, 40, 5, TFT_ORANGE, TFT_WHITE, "-10", 
                       SCREEN_SETTINGS, decreaseReflowTempCoarseCallback);
  uiManager.createButton("reflow_temp_plus1", tempBoxX + 60, tempBoxY, 40, 40, 5, TFT_ORANGE, TFT_WHITE, "+1", 
                       SCREEN_SETTINGS, increaseReflowTempFineCallback);
  uiManager.createButton("reflow_temp_minus1", tempBoxX + 60, tempBoxY + 50, 40, 40, 5, TFT_ORANGE, TFT_WHITE, "-1", 
                       SCREEN_SETTINGS, decreaseReflowTempFineCallback);

  // Accent Invert Button
  uiManager.createButton("invert_accent_btn", 3, SCREEN_HEIGHT - 100, 112, 40, 10, TFT_ORANGE, TFT_WHITE, 
                       "Invert Accent", SCREEN_SETTINGS, toggleInvertAccentCallback);

  // Light Mode Toggle Button
  uiManager.createButton("light_mode_btn", (SCREEN_WIDTH - 112) - 3, SCREEN_HEIGHT - 100, 112, 40, 10, 
                       TFT_ORANGE, TFT_WHITE, "Light Mode", SCREEN_SETTINGS, toggleLightModeCallback);

  // Add buttons for "Swap Graph" and "Font Test"
  uiManager.createButton("swap_graph_btn", 3, SCREEN_HEIGHT - 100, 112, 40, 10, TFT_ORANGE, TFT_WHITE, 
                       "Swap Graph", SCREEN_MAIN, toggleGraphSizeCallback);

  uiManager.createButton("font_test_btn", (SCREEN_WIDTH - 112) - 3, SCREEN_HEIGHT - 100, 112, 40, 10, 
                       TFT_ORANGE, TFT_WHITE, "Font Test", SCREEN_MAIN, NULL);
}