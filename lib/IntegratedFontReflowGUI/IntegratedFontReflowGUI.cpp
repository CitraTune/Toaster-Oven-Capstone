#include "IntegratedFontReflowGUI.hpp"

// Define constants
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

// Initialize LGFX display
LGFX IntegratedFontReflowGUI::display;

// Initialize sample text
const char* IntegratedFontReflowGUI::sampleText = "bonfire";

// Define the static font array and font names
const GFXfont* IntegratedFontReflowGUI::fonts[] = {
    &FreeMono9pt7b,
    &FreeMono12pt7b,
    &FreeSans9pt7b,
    &FreeSans12pt7b,
    &FreeSansBold9pt7b,
    &FreeSansBold12pt7b,
    &FreeSansBoldOblique9pt7b,
    &FreeSansBoldOblique12pt7b,
    &FreeMonoBold9pt7b,
    &FreeMonoBold12pt7b
};

// Define font names array for display
const char* IntegratedFontReflowGUI::fontNames[] = {
    "FreeMono9pt",
    "FreeMono12pt",
    "FreeSans9pt",
    "FreeSans12pt",
    "FreeSansBold9pt",
    "FreeSansBold12pt",
    "FreeSansBoldOblique9pt",
    "FreeSansBoldOblique12pt",
    "FreeMonoBold9pt",
    "FreeMonoBold12pt"
};

// Initialize font count
const int IntegratedFontReflowGUI::fontCount = sizeof(fonts) / sizeof(fonts[0]);

// Global reference to the controller instance for callbacks
IntegratedFontReflowGUI* g_controller = nullptr;

// Button action callbacks
void goToSettingsCallback() {
  if (g_controller) g_controller->goToSettings();
}

void goToMainCallback() {
  if (g_controller) g_controller->goToMain();
}

void goToFontsCallback() {
  if (g_controller) g_controller->goToFonts();
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

void nextFontCallback() {
  if (g_controller) g_controller->nextFont();
}

void prevFontCallback() {
  if (g_controller) g_controller->prevFont();
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
IntegratedFontReflowGUI::IntegratedFontReflowGUI() 
  : uiManager(display), graphManager(display), tempManager(display) {
  lastTouchTime = 0;
  currentFontIndex = 0;
  
  // Set the global reference
  g_controller = this;
}

// Setup all components
void IntegratedFontReflowGUI::setup() {
  Serial.begin(115200);
  
  // Setup the TFT display
  display.begin();
  display.setRotation(7);      // Adjust based on your display orientation
  display.invertDisplay(true); // Optionally invert colors
  display.setBrightness(128);
  display.setColorDepth(24);
  
  // Setup UI manager
  uiManager.setup();
  
  // Set up main and settings screen buttons
  setupButtons();
  
  // Set up font screen buttons and elements
  setupFontScreenButtons();
  
  // Start with the main screen
  uiManager.navigateToScreen(SCREEN_MAIN);
}

// Main loop
void IntegratedFontReflowGUI::loop() {
  // Note: Touch detection is handled externally by your touch system
  // This loop is kept simple to ensure compatibility
  delay(10);  // Reduced delay for smoother UI
}

// Action methods for screen navigation
void IntegratedFontReflowGUI::goToSettings() {
  uiManager.navigateToScreen(SCREEN_SETTINGS);
  tempManager.displayTemperatures(uiManager.lightMode);
}

void IntegratedFontReflowGUI::goToMain() {
  uiManager.navigateToScreen(SCREEN_MAIN);
  graphManager.draw(uiManager.lightMode);
}

void IntegratedFontReflowGUI::goToFonts() {
  uiManager.navigateToScreen(SCREEN_FONTS);
  updateFontDisplay();
}

// Theme toggle functions
void IntegratedFontReflowGUI::toggleLightMode() {
  uiManager.lightMode = !uiManager.lightMode;
  // Find the light mode button and update its label
  Button* lightModeButton = uiManager.getButton("light_mode_btn");
  if (lightModeButton) {
    lightModeButton->label = !uiManager.lightMode ? "Light Mode" : "Dark Mode";
  }
  redrawCurrentScreen();
}

void IntegratedFontReflowGUI::toggleInvertAccent() {
  uiManager.invertAccent = !uiManager.invertAccent;
  uiManager.updateButtonColors();
  redrawCurrentScreen();
}

void IntegratedFontReflowGUI::toggleGraphSize() {
  graphManager.toggleFullScreen();
  redrawCurrentScreen();
}

// Font navigation functions
void IntegratedFontReflowGUI::nextFont() {
  currentFontIndex = (currentFontIndex + 1) % fontCount;
  updateFontDisplay();
}

void IntegratedFontReflowGUI::prevFont() {
  currentFontIndex = (currentFontIndex - 1 + fontCount) % fontCount;
  updateFontDisplay();
}

// Update font display elements
void IntegratedFontReflowGUI::updateFontDisplay() {
  // Update sample text font
  TextElement* sampleTextElement = uiManager.getTextElement("sample_text");
  if (sampleTextElement) {
    sampleTextElement->font = fonts[currentFontIndex];
    
    // Center text horizontally - calculate position based on text width
    display.setFont(fonts[currentFontIndex]);
    int textWidth = display.textWidth(sampleText);
    sampleTextElement->x = (display.width() - textWidth) / 2;
  }
  
  // Update font name label
  TextElement* fontNameElement = uiManager.getTextElement("font_name");
  if (fontNameElement) {
    fontNameElement->content = fontNames[currentFontIndex];
    
    // Center the font label
    display.setFont(&lgfx::fonts::Font2);  // Use a small font for the label
    int labelWidth = display.textWidth(fontNames[currentFontIndex]);
    fontNameElement->x = (display.width() - labelWidth) / 2;
  }
  
  // Update counter label
  TextElement* counterElement = uiManager.getTextElement("font_counter");
  if (counterElement) {
    String counterText = String(currentFontIndex + 1) + "/" + String(fontCount);
    counterElement->content = counterText;
  }
  
  // Redraw screen
  uiManager.drawActiveScreen();
}

// Redraw current screen based on which screen is active
void IntegratedFontReflowGUI::redrawCurrentScreen() {
  uiManager.drawActiveScreen();
  
  if (uiManager.currentScreen == SCREEN_MAIN) {
    graphManager.draw(uiManager.lightMode);
  } else if (uiManager.currentScreen == SCREEN_SETTINGS) {
    tempManager.displayTemperatures(uiManager.lightMode);
  } else if (uiManager.currentScreen == SCREEN_FONTS) {
    updateFontDisplay();
  }
}

// Setup interface buttons
void IntegratedFontReflowGUI::setupButtons() {
  // Set font for button sizing calculations
  display.setFont(&lgfx::fonts::FreeSans9pt7b);
  int fontHeight = display.fontHeight();
  int buttonTextMargin = 10; // Margin around text (top and bottom)
  int buttonHeight = fontHeight + 2 * buttonTextMargin; // Dynamic height based on font
  
  // Main screen buttons
  uiManager.createButton(
    "settings_btn",           // key - unique identifier
    10,                       // x - left side
    SCREEN_HEIGHT - buttonHeight - 10, // y - positioned from bottom with margin
    100,                      // width
    buttonHeight,             // height - dynamically sized
    10,                       // radius - rounded corners
    TFT_ORANGE,               // color - orange
    TFT_WHITE,                // text color
    "Settings",               // label
    SCREEN_MAIN,              // screen
    goToSettingsCallback      // action function
  );
  
  // Font test button on main screen
  uiManager.createButton(
    "font_test_btn",          // key
    SCREEN_WIDTH - 110,       // x - right side
    SCREEN_HEIGHT - buttonHeight - 10, // y - same height as settings
    100,                      // width
    buttonHeight,             // height
    10,                       // radius
    TFT_ORANGE,               // color
    TFT_WHITE,                // text color
    "Font Test",              // label
    SCREEN_MAIN,              // screen
    goToFontsCallback         // action function
  );
  
  // Settings screen back button
  uiManager.createButton(
    "back_from_settings_btn", // key - unique identifier
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

  // Add button for "Swap Graph" on Main Screen
  uiManager.createButton("swap_graph_btn", (SCREEN_WIDTH - 160) / 2, SCREEN_HEIGHT - 55, 160, 40, 10, 
                       TFT_ORANGE, TFT_WHITE, "Swap Graph", SCREEN_MAIN, toggleGraphSizeCallback);
}

// Setup font screen buttons and elements
void IntegratedFontReflowGUI::setupFontScreenButtons() {
  // Set font for button sizing calculations
  display.setFont(&lgfx::fonts::FreeSans9pt7b);
  int fontHeight = display.fontHeight();
  int buttonTextMargin = 10;
  int buttonHeight = fontHeight + 2 * buttonTextMargin;
  
  // Font test screen back button
  uiManager.createButton(
    "back_from_fonts_btn",    // key
    (SCREEN_WIDTH - 160) / 2, // x - centered
    SCREEN_HEIGHT - buttonHeight - 10, // y - positioned from bottom with margin
    160,                      // width
    buttonHeight,             // height
    10,                       // radius
    TFT_ORANGE,               // color
    TFT_WHITE,                // text color
    "Back",                   // label
    SCREEN_FONTS,             // screen
    goToMainCallback          // action function
  );
  
  // Font navigation buttons (left/right)
  uiManager.createButton(
    "prev_font_btn",          // key
    10,                       // x - left side
    SCREEN_HEIGHT - buttonHeight - 60, // y - above back button
    60,                       // width
    buttonHeight,             // height
    10,                       // radius
    TFT_DARKGREY,             // color
    TFT_WHITE,                // text color
    "<",                      // label
    SCREEN_FONTS,             // screen
    prevFontCallback          // action function
  );
  
  uiManager.createButton(
    "next_font_btn",          // key
    SCREEN_WIDTH - 70,        // x - right side
    SCREEN_HEIGHT - buttonHeight - 60, // y - above back button
    60,                       // width
    buttonHeight,             // height
    10,                       // radius
    TFT_DARKGREY,             // color
    TFT_WHITE,                // text color
    ">",                      // label
    SCREEN_FONTS,             // screen
    nextFontCallback          // action function
  );
  
  // Create sample text element (position will be calculated in updateFontDisplay)
  uiManager.createTextElement(
    "sample_text",            // key
    0,                        // x - will be calculated in updateFontDisplay
    80,                       // y - about 1/4 down the screen
    TFT_WHITE,                // color
    sampleText,               // content
    SCREEN_FONTS,             // screen
    fonts[currentFontIndex]   // font
  );
  
  // Create font name label
  uiManager.createTextElement(
    "font_name",              // key
    0,                        // x - will be calculated in updateFontDisplay
    120,                      // y - below sample text
    TFT_YELLOW,               // color
    fontNames[currentFontIndex], // content
    SCREEN_FONTS,             // screen
    &lgfx::fonts::Font2       // font (small font for label)
  );
  
  // Create font counter label (e.g., "1/10")
  String counterText = String(currentFontIndex + 1) + "/" + String(fontCount);
  uiManager.createTextElement(
    "font_counter",           // key
    10,                       // x - left side
    10,                       // y - top
    TFT_CYAN,                 // color
    counterText,              // content
    SCREEN_FONTS,             // screen
    &lgfx::fonts::Font2       // font (small font for counter)
  );
}