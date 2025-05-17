#include "IntegratedFontReflowGUI.hpp"

// Define constants
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320
#define TOUCH_SDA 33
#define TOUCH_SCL 32
#define TOUCH_INT 21
#define TOUCH_RST 25

// Initialize LGFX display
LGFX IntegratedFontReflowGUI::display;

// Define the static font array and font names
const GFXfont* IntegratedFontReflowGUI::fonts[] = {
    &FreeMono12pt7b,
    &FreeSansBoldOblique12pt7b,
    &FreeSans12pt7b,
    &FreeSansBold12pt7b,
    &FreeMonoBold12pt7b,
    &FreeMonoOblique12pt7b,
    &FreeSerif12pt7b,
    &FreeSerifBold12pt7b,
    &FreeSerifBoldItalic12pt7b,
    &FreeSerifItalic12pt7b
};

// Define font names array for display ALL 12PT
const char* IntegratedFontReflowGUI::fontNames[] = {
    "FreeMono",
    "FreeSansBoldOblique",
    "FreeSans",
    "FreeSansBold",
    "FreeMonoBold",
    "FreeMonoOblique",
    "FreeSerif",
    "FreeSerifBold",
    "FreeSerifBoldItalic",
    "FreeSerifItalic"
};

// Initialize font count
const int IntegratedFontReflowGUI::fontCount = sizeof(fonts) / sizeof(fonts[0]);

// Global reference to the controller instance for callbacks
IntegratedFontReflowGUI* g_controller = nullptr;

// Button action callbacks
void goToSettingsCallback() {
  Serial.println("Settings button pressed!");
  if (g_controller) g_controller->goToSettings();
}

void goToMainCallback() {
  Serial.println("Back/Main button pressed!");
  if (g_controller) g_controller->goToMain();
}

void goToFontsCallback() {
  Serial.println("Font Test button pressed!");
  if (g_controller) g_controller->goToFonts();
}

void toggleLightModeCallback() {
  if (g_controller) g_controller->toggleLightMode();
}

void toggleInvertAccentCallback() {
  if (g_controller) g_controller->toggleInvertAccent();
}

void nextFontCallback() {
  if (g_controller) g_controller->nextFont();
}

void prevFontCallback() {
  if (g_controller) g_controller->prevFont();
}

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
IntegratedFontReflowGUI::IntegratedFontReflowGUI() 
  : uiManager(display), graphManager(display) {
  lastTouchTime = 0;
  currentFontIndex = 0;
  soakTemp = 150;
  reflowTemp = 230;
  
  // Set the global reference
  g_controller = this;
}

// Setup all components
void IntegratedFontReflowGUI::setup() {
  Serial.begin(115200);
  Serial.println("Starting setup...");
  
  // Setup the TFT display
  display.begin();
  display.setRotation(7);      // Adjust based on your display orientation
  display.invertDisplay(true); // Optionally invert colors
  display.setBrightness(128);
  display.setColorDepth(24);
  
  // Initialize touch
  Serial.println("Initializing touch...");
  touch.init(TOUCH_SDA, TOUCH_SCL, TOUCH_RST, TOUCH_INT);
  if (display.touch()) {
    Serial.println("Touch initialized successfully");
  } else {
    Serial.println("ERROR: Touch initialization failed!");
  }
  
  // Setup UI manager
  uiManager.setup();
  
  // Set up main and settings screen buttons
  setupButtons();
  
  // Set up font screen buttons and elements
  setupFontScreenButtons();
  
  // Set up temperature labels
  setupTemperatureElements();
  
  // Start with the main screen
  uiManager.navigateToScreen(SCREEN_MAIN);
  Serial.println("Setup complete!");
}

// Main loop
void IntegratedFontReflowGUI::loop() {
  // Check for touch input
  TOUCHINFO ti;
    // Get touch samples
    if (touch.getSamples(&ti))
    {
        unsigned long currentTime = millis();
        if (currentTime - lastTouchTime > debounceDelay)
        {
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

// Action methods for screen navigation
void IntegratedFontReflowGUI::goToSettings() {
  uiManager.navigateToScreen(SCREEN_SETTINGS);
}

void IntegratedFontReflowGUI::goToMain() {
  uiManager.navigateToScreen(SCREEN_MAIN);
  if (graphManager.isVisibleOnScreen(SCREEN_MAIN)) {
      graphManager.draw();
  }
}

void IntegratedFontReflowGUI::goToFonts() {
  uiManager.navigateToScreen(SCREEN_FONTS);
  updateFontDisplay();
}

// Theme toggle functions
void IntegratedFontReflowGUI::toggleLightMode() {
  uiManager.toggleLightMode();
  redrawCurrentScreen();
}

void IntegratedFontReflowGUI::toggleInvertAccent() {
  uiManager.toggleInvertAccent();
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
  // Keep special fonts for certain elements
  const std::unordered_map<std::string, const lgfx::IFont*> specialFonts = {
    {"font_name", &lgfx::fonts::Font2},
    {"font_counter", &lgfx::fonts::Font2},
    {"main_menu_label", fonts[currentFontIndex]},
    {"settings_menu_label", fonts[currentFontIndex]},
    {"font_menu_label", fonts[currentFontIndex]},
  };

  // Update font name and counter content
  TextElement* fontNameElement = uiManager.getTextElement("font_name");
  if (fontNameElement) {
    fontNameElement->content = fontNames[currentFontIndex];
  }

  TextElement* counterElement = uiManager.getTextElement("font_counter");
  if (counterElement) {
    String counterText = String(currentFontIndex + 1) + "/" + String(fontCount);
    counterElement->content = counterText;
  }

  // Update all text elements in all screens
  for (auto& pair : uiManager.getTextElements()) {
    TextElement& element = const_cast<TextElement&>(pair.second);
    const std::string& key = pair.first;

    // Check if this element has a special font
    auto specialFont = specialFonts.find(key);
    if (specialFont != specialFonts.end()) {
      element.font = specialFont->second;
    } else {
      // Use the current font from the fonts array for all other text elements
      // Adjust font size to match previous height as closely as possible
      int prevFontHeight = 0;
      if (element.font) {
        display.setFont(element.font);
        prevFontHeight = display.fontHeight();
      }
      const lgfx::IFont* bestFont = fonts[currentFontIndex];
      int bestDiff = 10000;
      // Try to match font height if possible
      for (int i = 0; i < fontCount; ++i) {
        display.setFont(fonts[i]);
        int h = display.fontHeight();
        if (prevFontHeight > 0 && abs(h - prevFontHeight) < bestDiff) {
          bestDiff = abs(h - prevFontHeight);
          bestFont = fonts[i];
        }
      }
      element.font = bestFont;
    }

    // Recalculate center position for certain elements
    if (element.font) {
      display.setFont(element.font);
      if (key == "font_name" || key == "sample_text" || key == "font_menu_label") {
        int textWidth = display.textWidth(element.content);
        element.x = (display.width() - textWidth) / 2;
      }
      // For main_menu_label and settings_menu_label, keep x as originally set (top left)
    }
  }

  // Redraw screen (this will redraw all buttons with the new font, since Button::draw uses display.setFont)
  uiManager.drawActiveScreen();
}

// Redraw current screen based on which screen is active
void IntegratedFontReflowGUI::redrawCurrentScreen() {
  uiManager.drawActiveScreen();
  
  if (graphManager.isVisibleOnScreen(uiManager.getScreen())) {
      graphManager.draw();
  }
}

// Setup interface buttons
void IntegratedFontReflowGUI::setupButtons() {
  // Set font for button sizing calculations
  display.setFont(&lgfx::fonts::FreeSans9pt7b);
  int fontHeight = display.fontHeight();
  int buttonTextMargin = 10;
  int buttonHeight = fontHeight + 2 * buttonTextMargin;
  
  // Add menu labels for each screen
  uiManager.createTextElement(
      "main_menu_label",
      10,                    // x - left margin
      25,                    // y - top margin
      TFT_WHITE,            // color
      "Main Menu",          // content
      SCREEN_MAIN,          // screen
      &lgfx::fonts::FreeSans9pt7b  // font
  );

  uiManager.createTextElement(
      "settings_menu_label",
      10,                    // x
      25,                    // y
      TFT_WHITE,            // color
      "Settings",           // content
      SCREEN_SETTINGS,      // screen
      &lgfx::fonts::FreeSans9pt7b  // font
  );

  uiManager.createTextElement(
      "font_menu_label",
      10,                    // x
      25,                    // y
      TFT_WHITE,            // color
      "Font Selection",     // content
      SCREEN_FONTS,         // screen
      &lgfx::fonts::FreeSans9pt7b  // font
  );

  // Main screen buttons - Settings and Font Test buttons with same width
  const int buttonWidth = 160;
  const int buttonMargin = 10;
  const int buttonX = (SCREEN_WIDTH - buttonWidth) / 2;
  
  // Settings button at bottom
  uiManager.createButton(
      "settings_btn",
      buttonX,              // x - centered
      SCREEN_HEIGHT - buttonHeight - buttonMargin, // y - bottom with margin
      buttonWidth,          // width - same as back button
      buttonHeight,         // height
      10,                   // radius
      "Settings",          // label
      SCREEN_MAIN,         // screen
      goToSettingsCallback // action
  );
  
  // Font Test button above Settings
  uiManager.createButton(
      "font_test_btn",
      buttonX,             // x - centered
      SCREEN_HEIGHT - (2 * buttonHeight) - (2 * buttonMargin), // y - above settings
      buttonWidth,         // width
      buttonHeight,        // height
      10,                  // radius
      "Font Test",        // label
      SCREEN_MAIN,        // screen
      goToFontsCallback   // action
  );

  // Back buttons for Settings and Fonts screens (same position as Settings button)
  uiManager.createButton(
      "back_from_settings_btn",
      buttonX,             // x - centered
      SCREEN_HEIGHT - buttonHeight - buttonMargin,
      buttonWidth,         // width
      buttonHeight,        // height
      10,                  // radius
      "Back",             // label
      SCREEN_SETTINGS,    // screen
      goToMainCallback    // action
  );

  uiManager.createButton(
      "back_from_fonts_btn",
      buttonX,             // x - centered
      SCREEN_HEIGHT - buttonHeight - buttonMargin,
      buttonWidth,         // width
      buttonHeight,        // height
      10,                  // radius
      "Back",             // label
      SCREEN_FONTS,       // screen
      goToMainCallback    // action
  );
  
  // Temperature controls in settings screen
  int tempBoxX = 130;
  int tempBoxY = 10;
  
  // Soak Temp Controls
  uiManager.createButton("soak_temp_plus10", tempBoxX, tempBoxY, 40, 40, 5, "+10", 
                       SCREEN_SETTINGS, increaseSoakTempCoarseCallback);
  uiManager.createButton("soak_temp_minus10", tempBoxX, tempBoxY + 50, 40, 40, 5, "-10", 
                       SCREEN_SETTINGS, decreaseSoakTempCoarseCallback);
  uiManager.createButton("soak_temp_plus1", tempBoxX + 60, tempBoxY, 40, 40, 5, "+1", 
                       SCREEN_SETTINGS, increaseSoakTempFineCallback);
  uiManager.createButton("soak_temp_minus1", tempBoxX + 60, tempBoxY + 50, 40, 40, 5, "-1", 
                       SCREEN_SETTINGS, decreaseSoakTempFineCallback);
  
  // Reflow Temp Controls
  tempBoxY += 100;
  uiManager.createButton("reflow_temp_plus10", tempBoxX, tempBoxY, 40, 40, 5, "+10", 
                       SCREEN_SETTINGS, increaseReflowTempCoarseCallback);
  uiManager.createButton("reflow_temp_minus10", tempBoxX, tempBoxY + 50, 40, 40, 5, "-10", 
                       SCREEN_SETTINGS, decreaseReflowTempCoarseCallback);
  uiManager.createButton("reflow_temp_plus1", tempBoxX + 60, tempBoxY, 40, 40, 5, "+1", 
                       SCREEN_SETTINGS, increaseReflowTempFineCallback);
  uiManager.createButton("reflow_temp_minus1", tempBoxX + 60, tempBoxY + 50, 40, 40, 5, "-1", 
                       SCREEN_SETTINGS, decreaseReflowTempFineCallback);

  // Accent Invert Button
  uiManager.createButton("invert_accent_btn", 3, SCREEN_HEIGHT - 100, 112, 40, 10,
                       "Invert Accent", SCREEN_SETTINGS, toggleInvertAccentCallback);

  // Light Mode Toggle Button
  uiManager.createButton("light_mode_btn", (SCREEN_WIDTH - 112) - 3, SCREEN_HEIGHT - 100, 112, 40, 10,
                       "Light Mode", SCREEN_SETTINGS, toggleLightModeCallback);
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
    buttonHeight,             // height - dynamically sized
    10,                       // radius - rounded corners
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
    "bonfire",               // content
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

// Setup temperature display elements in settings screen
void IntegratedFontReflowGUI::setupTemperatureElements() {
    // Create temperature labels and values
    uiManager.createTextElement(
        "soak_temp_label",
        10, 50,
        TFT_WHITE,
        "Soak Temp:",
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
        "Reflow Temp:",
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

// Increase or decrease soak temperature
void IntegratedFontReflowGUI::increaseSoakTemp(bool coarse) {
    soakTemp += (coarse ? 10 : 1);
    if (TextElement* element = uiManager.getTextElement("soak_temp_value")) {
        element->content = String(soakTemp) + " C";
        uiManager.drawActiveScreen();
    }
}

void IntegratedFontReflowGUI::decreaseSoakTemp(bool coarse) {
    soakTemp -= (coarse ? 10 : 1);
    if (TextElement* element = uiManager.getTextElement("soak_temp_value")) {
        element->content = String(soakTemp) + " C";
        uiManager.drawActiveScreen();
    }
}

// Increase or decrease reflow temperature
void IntegratedFontReflowGUI::increaseReflowTemp(bool coarse) {
    reflowTemp += (coarse ? 10 : 1);
    if (TextElement* element = uiManager.getTextElement("reflow_temp_value")) {
        element->content = String(reflowTemp) + " C";
        uiManager.drawActiveScreen();
    }
}

void IntegratedFontReflowGUI::decreaseReflowTemp(bool coarse) {
    reflowTemp -= (coarse ? 10 : 1);
    if (TextElement* element = uiManager.getTextElement("reflow_temp_value")) {
        element->content = String(reflowTemp) + " C";
        uiManager.drawActiveScreen();
    }
}

