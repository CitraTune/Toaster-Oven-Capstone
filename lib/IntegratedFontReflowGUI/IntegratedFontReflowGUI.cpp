#include "IntegratedFontReflowGUI.hpp"
#include "ButtonSetup.hpp"
#include "TextSetup.hpp"

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
    "FreeMono",
    "FreeSansBoldOblique",
    "FreeSans",
    "FreeSansBold",
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
  Serial.println("Initializing touch...");
  touch.init(TOUCH_SDA, TOUCH_SCL, TOUCH_RST, TOUCH_INT);
  if (display.touch()){
    Serial.println("Touch initialized successfully");
  }
  else{
    Serial.println("ERROR: Touch initialization failed!");
  }

  // Setup UI manager
  UIManager::setup();

  // Set up text elements and buttons for all screens
  setupTextElements();
  setupButtons();

  UIManager::navigateToScreen(SCREEN_MAIN);
  Serial.println("Setup complete!");
}

// Main loop
void IntegratedFontReflowGUI::loop()
{
  auto &delay = IntegratedFontReflowGUI::debounceDelay;
  auto &last = IntegratedFontReflowGUI::lastTouchTime;

  if (currentTime - last > delay)
  {

    // Check for touch input
    TOUCHINFO ti;
    // Get touch samples
    if (touch.getSamples(&ti))
    {
      unsigned long currentTime = millis();
      if (currentTime - last > delay)
        lastTouchTime = currentTime;

      int x = ti.x[0];
      int y = ti.y[0];

      // Debugging output
      Serial.printf("Touch x=%d y=%d\n", x, y);

      // Check if any button was pressed
      UIManager::checkButtonPress(x, y);
    }
  }
  delay(10); // Reduced delay for smoother UI
}

// Update font display elements
void IntegratedFontReflowGUI::updateFontDisplay()
{
  // Keep special fonts for certain elements
  const std::unordered_map<std::string, const lgfx::IFont *> specialFonts = {
      {"font_name", &lgfx::fonts::Font2},
      {"font_counter", &lgfx::fonts::Font2},
      {"main_menu_label", fonts[currentFontIndex]},
      {"settings_menu_label", fonts[currentFontIndex]},
      {"font_menu_label", fonts[currentFontIndex]},
  };

  // Update font name and counter content
  TextElement *fontNameElement = UIManager::getTextElement("font_name");
  if (fontNameElement)
  {
    fontNameElement->content = fontNames[currentFontIndex];
  }

  TextElement *counterElement = UIManager::getTextElement("font_counter");
  if (counterElement)
  {
    String counterText = String(currentFontIndex + 1) + "/" + String(fontCount);
    counterElement->content = counterText;
  }

  // Update all text elements in all screens
  for (auto &pair : UIManager::getTextElements())
  {
    TextElement &element = const_cast<TextElement &>(pair.second);
    const std::string &key = pair.first;

    // Check if this element has a special font
    auto specialFont = specialFonts.find(key);
    if (specialFont != specialFonts.end())
    {
      element.font = specialFont->second;
    }
    else
    {
      // Use the current font from the fonts array for all other text elements
      // Adjust font size to match previous height as closely as possible
      int prevFontHeight = 0;
      if (element.font)
      {
        display.setFont(element.font);
        prevFontHeight = display.fontHeight();
      }
      const lgfx::IFont *bestFont = fonts[currentFontIndex];
      int bestDiff = 10000;
      // Try to match font height if possible
      for (int i = 0; i < fontCount; ++i)
      {
        display.setFont(fonts[i]);
        int h = display.fontHeight();
        if (prevFontHeight > 0 && abs(h - prevFontHeight) < bestDiff)
        {
          bestDiff = abs(h - prevFontHeight);
          bestFont = fonts[i];
        }
      }
      element.font = bestFont;
    }

    // Recalculate center position for certain elements
    if (element.font)
    {
      display.setFont(element.font);
      if (key == "font_name" || key == "sample_text" || key == "font_menu_label")
      {
        int textWidth = display.textWidth(element.content);
        element.x = (display.width() - textWidth) / 2;
      }
      // For main_menu_label and settings_menu_label, keep x as originally set (top left)
    }
  }

  // Redraw screen (this will redraw all buttons with the new font, since Button::draw uses display.setFont)
  UIManager::drawActiveScreen();
}

// Redraw current screen based on which screen is active
void IntegratedFontReflowGUI::redrawCurrentScreen()
{
  UIManager::drawActiveScreen();

  if (GraphManager::isVisibleOnScreen(UIManager::getScreen()))
  {
    GraphManager::draw();
  }
}

// Temperature control functions
void IntegratedFontReflowGUI::increaseSoakTemp(bool coarse)
{
  soakTemp += (coarse ? 10 : 1);
  if (TextElement *element = UIManager::getTextElement("soak_temp_value"))
  {
    element->content = String(soakTemp) + " C";
    UIManager::drawActiveScreen();
  }
}

void IntegratedFontReflowGUI::decreaseSoakTemp(bool coarse)
{
  soakTemp -= (coarse ? 10 : 1);
  if (TextElement *element = UIManager::getTextElement("soak_temp_value"))
  {
    element->content = String(soakTemp) + " C";
    UIManager::drawActiveScreen();
  }
}

// Increase or decrease reflow temperature
void IntegratedFontReflowGUI::increaseReflowTemp(bool coarse)
{
  reflowTemp += (coarse ? 10 : 1);
  if (TextElement *element = UIManager::getTextElement("reflow_temp_value"))
  {
    element->content = String(reflowTemp) + " C";
    UIManager::drawActiveScreen();
  }
}

void IntegratedFontReflowGUI::decreaseReflowTemp(bool coarse)
{
  reflowTemp -= (coarse ? 10 : 1);
  if (TextElement *element = UIManager::getTextElement("reflow_temp_value"))
  {
    element->content = String(reflowTemp) + " C";
    UIManager::drawActiveScreen();
  }
}