#include "FontSelectorGUI.hpp"
#include "LGFX_Config.h"
#include <lgfx/v1/misc/enum.hpp>

// Pin definitions for capacitive touch
#define TOUCH_SDA 33
#define TOUCH_SCL 32
#define TOUCH_INT 21
#define TOUCH_RST 25

// setup static instance pointer
FontSelectorGUI *FontSelectorGUI::instance = nullptr;

// Define the static font array and font names
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

// Define font names array for display
const char* FontSelectorGUI::fontNames[] = {
    "FreeMono12pt",
    "FreeSansBoldOblique12pt",
    "FreeSans12pt",
    "FreeSansBold12pt",
    "FreeMonoBold12pt",
    "FreeMonoOblique12pt",
    "FreeSerif12pt",
    "FreeSerifBold12pt",
    "FreeSerifBoldItalic12pt",
    "FreeSerifItalic12pt"
};

FontSelectorGUI::FontSelectorGUI(LGFX &display, UIManager &uiManager)
    : display(display), uiManager(uiManager), currentFontIndex(0)
{
    lastTouchTime = 0;
    // Use a small font for the label
    labelFont = &lgfx::fonts::FreeSans9pt7b; // Changed to 9pt for consistency
    // Set the instance pointer in the constructor
    instance = this;
}

void FontSelectorGUI::handleLeftPress()
{
    if (instance)
    {
        instance->currentFontIndex = (instance->currentFontIndex - 1 + NUM_FONTS) % NUM_FONTS;
        instance->updateFont();
    }
}

void FontSelectorGUI::handleRightPress()
{
    if (instance)
    {
        instance->currentFontIndex = (instance->currentFontIndex + 1) % NUM_FONTS;
        instance->updateFont();
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

    // Setup UI manager with initial font
    uiManager.setup(display);
    uiManager.setFont(fonts[currentFontIndex]);

    // Start with the main screen
    uiManager.setCurrentScreen(SCREEN_MAIN);

    // Create navigation buttons using string keys
    // Button colors are now managed by UIManager's light/dark mode
    uiManager.createButton("left_btn", 10, display.height() - 60, 60, 40, 8,
                         "<", SCREEN_MAIN, handleLeftPress);
    uiManager.createButton("right_btn", display.width() - 70, display.height() - 60, 60, 40, 8,
                         ">", SCREEN_MAIN, handleRightPress);

    // Create text elements with initial font
    const int numFonts = sizeof(fonts) / sizeof(fonts[0]);
    String counterText = String(currentFontIndex + 1) + "/" + String(numFonts);
    
    // Create the sample text element for font preview
    uiManager.createTextElement("sample_text", display.width() / 2, 30, TFT_WHITE, "Sample Text", 
                             SCREEN_MAIN, fonts[currentFontIndex]);

    // Create the font name label and counter
    uiManager.createTextElement("font_label", display.width() / 2, 60, TFT_WHITE,
                             fontNames[currentFontIndex], SCREEN_MAIN, labelFont);

    // Create the font counter label (e.g., "1/10")
    uiManager.createTextElement("font_counter", 10, 10, TFT_WHITE, 
                             counterText, SCREEN_MAIN, labelFont);

    // Draw the initial screen
    uiManager.drawActiveScreen();
}

void FontSelectorGUI::updateFont()
{
    // Update the font in UI manager
    uiManager.setFont(fonts[currentFontIndex]);

    // Update text elements
    const int numFonts = sizeof(fonts) / sizeof(fonts[0]);
    String counterText = String(currentFontIndex + 1) + "/" + String(numFonts);
    
    // Update counter and label texts
    auto* counterElement = uiManager.getTextElement("font_counter");
    if (counterElement) {
        counterElement->content = counterText;
    }
    
    auto* labelElement = uiManager.getTextElement("font_label");
    if (labelElement) {
        labelElement->content = fontNames[currentFontIndex];
    }
    
    // Redraw everything
    uiManager.redraw();
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