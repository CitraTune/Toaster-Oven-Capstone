#include "FontSelectorGUI.hpp"
// Pin definitions for capacitive touch
#define TOUCH_SDA 33
#define TOUCH_SCL 32
#define TOUCH_INT 21
#define TOUCH_RST 25

// setup static instance pointer
FontSelectorGUI *FontSelectorGUI::instance = nullptr;

// Define the static font array
const GFXfont *FontSelectorGUI::fonts[] = {
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

FontSelectorGUI::FontSelectorGUI(LGFX &display, UIManager &uiManager)
    : display(display), uiManager(uiManager), currentFontIndex(0)
{
    lastTouchTime = 0;
    // Set the instance pointer in the constructor
    instance = this;
}

void FontSelectorGUI::handleLeftPress()
{
    if (instance)
    {
        instance->decrementFont();
    }
}

void FontSelectorGUI::handleRightPress()
{
    if (instance)
    {
        instance->incrementFont();
    }
}

void FontSelectorGUI::setup()
{
    Serial.begin(115200);
    Serial.println("Setting up FontSelectorGUI");

    // Setup the TFT display
    display.begin();
    display.setRotation(7);      // Adjust based on your display orientation
    display.invertDisplay(true); // Optionally invert colors
    display.setBrightness(128);
    display.setColorDepth(24);

    // Setup touch controller
    touch.init(TOUCH_SDA, TOUCH_SCL, TOUCH_RST, TOUCH_INT);

    // Setup UI manager
    uiManager.setup();

    // Start with the main screen
    uiManager.navigateToScreen(SCREEN_MAIN);

    // Create navigation buttons using string keys
    uiManager.createButton("left_btn", 10, display.height() - 60, 60, 40, 8,
                           TFT_DARKGREY, TFT_WHITE, "<", SCREEN_MAIN, handleLeftPress);
    uiManager.createButton("right_btn", display.width() - 70, display.height() - 60, 60, 40, 8,
                           TFT_DARKGREY, TFT_WHITE, ">", SCREEN_MAIN, handleRightPress);

    // Create the "bonfire" text element with a specific key
    uiManager.createTextElement("bonfire", 0, 0, TFT_WHITE, "bonfire", SCREEN_MAIN, fonts[currentFontIndex]);

    // Draw the initial screen
    uiManager.drawActiveScreen();
}

void FontSelectorGUI::decrementFont()
{
    int fontCount = sizeof(fonts) / sizeof(fonts[0]);
    currentFontIndex = (currentFontIndex - 1 + fontCount) % fontCount;
    updateDisplay();
}

void FontSelectorGUI::incrementFont()
{
    int fontCount = sizeof(fonts) / sizeof(fonts[0]);
    currentFontIndex = (currentFontIndex + 1) % fontCount;
    updateDisplay();
}

void FontSelectorGUI::loop()
{
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

    delay(10); // Reduced delay for smoother UI
}

void FontSelectorGUI::updateDisplay()
{
    // Update the font of the "bonfire" text element using its key
    TextElement* bonfireText = uiManager.getTextElement("bonfire");
    if (bonfireText) {
        bonfireText->font = fonts[currentFontIndex];
    // Redraw the active screen to reflect the font change
    uiManager.drawActiveScreen();
    } else {
        Serial.println("Error: 'bonfire' text element not found!");
}
}
