#include "FontSelectorGUI.hpp"
// Pin defsetupions for capacitive touch
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

    // setup the TFT display
    display.begin();
    display.setRotation(7);      // Adjust based on your display orientation
    display.invertDisplay(true); // Optionally invert colors
    display.setBrightness(128);
    display.setColorDepth(24);

    // setup touch controller
    touch.init(TOUCH_SDA, TOUCH_SCL, TOUCH_RST, TOUCH_INT);

    // setup UI manager
    uiManager.setup();

    // Start with the main screen

    //from testing, it looked like this flashed, then we went to the bonfire screen, 
    // the bonfire screen was blank, then the bonfire text was drawn
    //no buttons appeared
    //lets make sure UIManager buttons work?
    uiManager.navigateToScreen(SCREEN_MAIN);


    // Create navigation buttons
    uiManager.createButton(10, display.height() - 60, 60, 40, 8,
                           TFT_DARKGREY, TFT_WHITE, "<", 0, handleLeftPress);
    
    uiManager.drawActiveScreen();

    // uiManager.createButton(display.width() - 70, display.height() - 60, 60, 40, 8,
    //                        TFT_DARKGREY, TFT_WHITE, ">", 0, handleRightPress);

    //drawBonfireText();
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

void FontSelectorGUI::drawBonfireText()
{
    Serial.println("Drawing bonfire text with font index: " + String(currentFontIndex));
    display.setFont(fonts[currentFontIndex]);
    display.fillScreen(TFT_BLACK);
    display.setTextColor(TFT_WHITE);

    // Get text dimensions using LovyanGFX methods
    const char *text = "bonfire";
    int textWidth = display.textWidth(text);
    int textHeight = display.fontHeight();

    // Calculate center position
    int centerX = (display.width() - textWidth) / 2;
    int centerY = (display.height() + textHeight) / 2;

    display.setCursor(centerX, centerY);
    display.print(text);
}

void FontSelectorGUI::updateDisplay()
{
    drawBonfireText();
}