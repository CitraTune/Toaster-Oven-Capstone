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
  if (g_controller) g_controller->increaseSoakTemp(true);
}

void decreaseSoakTempCoarseCallback() {
  if (g_controller) g_controller->decreaseSoakTemp(true);
}

void increaseSoakTempFineCallback() {
  if (g_controller) g_controller->increaseSoakTemp(false);
}

void decreaseSoakTempFineCallback() {
  if (g_controller) g_controller->decreaseSoakTemp(false);
}

void increaseReflowTempCoarseCallback() {
  if (g_controller) g_controller->increaseReflowTemp(true);
}

void decreaseReflowTempCoarseCallback() {
  if (g_controller) g_controller->decreaseReflowTemp(true);
}

void increaseReflowTempFineCallback() {
  if (g_controller) g_controller->increaseReflowTemp(false);
}

void decreaseReflowTempFineCallback() {
  if (g_controller) g_controller->decreaseReflowTemp(false);
}

// Constructor
ReflowGUI::ReflowGUI() 
  : uiManager(display), graphManager(display) {
  lastTouchTime = 0;
  soakTemp = 150;
  reflowTemp = 230;
  
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
  
  // Set up temperature display elements
  setupTemperatureElements();
  
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
}

void ReflowGUI::goToMain() {
    uiManager.navigateToScreen(SCREEN_MAIN);
    graphManager.draw(uiManager.getLightMode());
}

void ReflowGUI::toggleLightMode() {
    uiManager.toggleLightMode();
    if (graphManager.isVisibleOnScreen(SCREEN_MAIN)) {
        graphManager.draw(uiManager.getLightMode());
    }
}

void ReflowGUI::toggleInvertAccent() {
    uiManager.toggleInvertAccent();
    if (graphManager.isVisibleOnScreen(SCREEN_MAIN)) {
        graphManager.draw(uiManager.getLightMode());
    }
}

void ReflowGUI::toggleGraphSize() {
    graphManager.draw(uiManager.getLightMode());
}

// Temperature control methods
void ReflowGUI::increaseSoakTemp(bool coarse) {
    soakTemp += (coarse ? 10 : 1);
    if (TextElement* element = uiManager.getTextElement("soak_temp_value")) {
        element->content = String(soakTemp) + " C";
        uiManager.drawActiveScreen();
    }
}

void ReflowGUI::decreaseSoakTemp(bool coarse) {
    soakTemp -= (coarse ? 10 : 1);
    if (TextElement* element = uiManager.getTextElement("soak_temp_value")) {
        element->content = String(soakTemp) + " C";
        uiManager.drawActiveScreen();
    }
}

void ReflowGUI::increaseReflowTemp(bool coarse) {
    reflowTemp += (coarse ? 10 : 1);
    if (TextElement* element = uiManager.getTextElement("reflow_temp_value")) {
        element->content = String(reflowTemp) + " C";
        uiManager.drawActiveScreen();
    }
}

void ReflowGUI::decreaseReflowTemp(bool coarse) {
    reflowTemp -= (coarse ? 10 : 1);
    if (TextElement* element = uiManager.getTextElement("reflow_temp_value")) {
        element->content = String(reflowTemp) + " C";
        uiManager.drawActiveScreen();
    }
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

// Setup temperature display elements
void ReflowGUI::setupTemperatureElements() {
    // Create temperature labels and values
    uiManager.createTextElement(
        "soak_temp_label",
        10, 50,
        TFT_WHITE,
        "Soak Temperature:",
        SCREEN_SETTINGS,
        &lgfx::fonts::FreeSans9pt7b
    );

    uiManager.createTextElement(
        "soak_temp_value",
        10, 75,
        TFT_YELLOW,
        String(soakTemp) + " C",
        SCREEN_SETTINGS,
        &lgfx::fonts::FreeSans12pt7b
    );

    uiManager.createTextElement(
        "reflow_temp_label",
        10, 140,
        TFT_WHITE,
        "Reflow Temperature:",
        SCREEN_SETTINGS,
        &lgfx::fonts::FreeSans9pt7b
    );

    uiManager.createTextElement(
        "reflow_temp_value",
        10, 165,
        TFT_YELLOW,
        String(reflowTemp) + " C",
        SCREEN_SETTINGS,
        &lgfx::fonts::FreeSans12pt7b
    );
}