#include "IntegratedFontReflowGUI.hpp"
#include "ButtonSetup.hpp"
#include "TextSetup.hpp"
#include "UIManager.hpp"
#include "LineArtManager.hpp"

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
const GFXfont *IntegratedFontReflowGUI::fonts[] = {
    &FreeMono12pt7b,
    &FreeSansBoldOblique12pt7b,
    &FreeSans12pt7b,
    &FreeSansBold12pt7b,
    &FreeMonoBold12pt7b,
    &FreeMonoOblique12pt7b,
    &FreeSerif12pt7b,
    &FreeSerifBold12pt7b,
    &FreeSerifBoldItalic12pt7b,
    &FreeSerifItalic12pt7b};

// Define font names array for display ALL 12PT
const char *IntegratedFontReflowGUI::fontNames[] = {
    "FreeSans",
    "FreeSansBoldOblique",
    "FreeSansBold",
    "FreeMono",
    "FreeMonoBold",
    "FreeMonoOblique",
    "FreeSerif",
    "FreeSerifBold",
    "FreeSerifBoldItalic",
    "FreeSerifItalic"};

// Initialize font count
const int IntegratedFontReflowGUI::fontCount = sizeof(fonts) / sizeof(fonts[0]);
int IntegratedFontReflowGUI::lastTouchTime = 0;
int IntegratedFontReflowGUI::currentFontIndex = 0;
int IntegratedFontReflowGUI::soakTemp = 150;
int IntegratedFontReflowGUI::reflowTemp = 230;
int IntegratedFontReflowGUI::buttonHeight = BUTTON_HEIGHT;
int IntegratedFontReflowGUI::debounceDelay = DEBOUNCE_DELAY;

// Add a new static variable to track whether buttons should be affected
bool IntegratedFontReflowGUI::affectButtons = false;

// Function pointer type for class member functions
typedef void (IntegratedFontReflowGUI::*MemberFunctionPtr)();
typedef void (IntegratedFontReflowGUI::*MemberFunctionPtrBool)(bool);

// Single callback handler for IntegratedFontReflowGUI member functions with bool parameter
void callMemberFunctionBool(IntegratedFontReflowGUI *instance, MemberFunctionPtrBool func, bool param)
{
  if (instance)
  {
    (instance->*func)(param);
  }
}

// Setup all components
void IntegratedFontReflowGUI::setup()
{
  Serial.begin(115200);
  Serial.println("Starting setup...");

  // Setup the TFT display
  display.begin();
  display.setRotation(7);      // Adjust based on your display orientation
  display.invertDisplay(true); // Optionally invert colors
  display.setBrightness(128);
  display.setColorDepth(24);

  // Initialize touch
  
  touch.init(TOUCH_SDA, TOUCH_SCL, TOUCH_RST, TOUCH_INT);

  // Setup UI manager
  UIManager::setup(display);
  LineArtManager::setup(display);
  ButtonSetup::setupAllButtons();
  TextSetup::setupAllTextElements();
  UIManager::setCurrentScreen(SCREEN_MAIN);
}

unsigned long cooldownStartTime = 0;
bool cooldownActive = false;

void cooldownScreen() {
    // Change to cooldown screen
    UIManager::setCurrentScreen(SCREEN_COOLDOWN);
    
    // Start the timer
    cooldownStartTime = millis();
    cooldownActive = true;
    
    // Initial timer text update
    UIManager::updateTextElementContent("cooldown_timer", "30s");
}

// Add this to your main loop to handle the timer countdown
void handleCooldownTimer() {
    if (cooldownActive) {
        // Calculate remaining time
        unsigned long elapsedTime = (millis() - cooldownStartTime) / 1000; // in seconds
        
        if (elapsedTime >= 30) {
            // Timer completed
            cooldownActive = false;
            UIManager::setCurrentScreen(SCREEN_MAIN); // Return to main screen
        } else {
            // Update timer display
            int remainingTime = 30 - elapsedTime;
            UIManager::updateTextElementContent("cooldown_timer", String(remainingTime) + "s");
        }
    }
}
// Main loop
void IntegratedFontReflowGUI::loop()
{
  // Check for touch input
  TOUCHINFO ti;
  // Get touch samples
  if (IntegratedFontReflowGUI::touch.getSamples(&ti))
  {
    unsigned long currentTime = millis();
    if (currentTime - IntegratedFontReflowGUI::lastTouchTime > IntegratedFontReflowGUI::debounceDelay)
    {
      lastTouchTime = currentTime;

      int x = ti.x[0];
      int y = ti.y[0];

      // Debugging output
      Serial.printf("Touch x=%d y=%d\n", x, y);

      // Check if any button was pressed
      UIManager::checkButtonPress(x, y);
    }
  }
  handleCooldownTimer();
  delay(10); // Reduced delay for smoother UI
}

// Update font display elements
void IntegratedFontReflowGUI::updateFontDisplay()
{
  // Create a string for the font counter display (showing which font out of total)
  String fontCounterText = String(currentFontIndex + 1) + "/" + String(fontCount);

  const char* currentFontName = fontNames[currentFontIndex];
  UIManager::updateTextElementContent("current_font_display", currentFontName);
  UIManager::updateTextElementContent("font_counter_display", fontCounterText);
  UIManager::updateTextElementContent("button_font_display", currentFontName);
  
  // Update text elements or buttons based on the toggle state
  if (affectButtons) {
    // Update all buttons to use the new font
    UIManager::updateAllFontsPreserveSize(currentFontName);
  } else {
    // Update only text elements to use the new font
    UIManager::updateAllTextElementFontsPreserveSize(currentFontName);
  }

  UIManager::drawActiveScreen();
}

// Add a new function to toggle the affectButtons state
void IntegratedFontReflowGUI::toggleAffectButtons()
{
  affectButtons = !affectButtons;
  String buttonText = "Affect buttons: ";
  buttonText += (affectButtons ? "True" : "False");
  UIManager::updateButtonText("toggle_affect_buttons", buttonText);
  UIManager::drawActiveScreen();
}
  