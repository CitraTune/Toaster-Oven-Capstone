#include "FontSelectorGUI.hpp"
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
    labelFont = &fonts::Font2;
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

    // Create the "bonfire" sample text element, calculating X to center text
    // We'll calculate X position for centering in updateDisplay() since it depends on font
    uiManager.createTextElement("bonfire", 0, 30, TFT_WHITE, "bonfire", 
                               SCREEN_MAIN, fonts[currentFontIndex]);

    // Create the font name label beneath the sample text
    uiManager.createTextElement("font_label", 0, 60, TFT_YELLOW, 
                               fontNames[currentFontIndex], SCREEN_MAIN, labelFont);
                               
    // Create the font counter label (e.g., "1/10")
    int fontCount = sizeof(fonts) / sizeof(fonts[0]);
    String counterText = String(currentFontIndex + 1) + "/" + String(fontCount);
    uiManager.createTextElement("font_counter", 10, 10, TFT_CYAN, 
                               counterText, SCREEN_MAIN, labelFont);

    // Initialize positions for text elements (center them)
    updateDisplay();
    
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
    int fontCount = sizeof(fonts) / sizeof(fonts[0]);
    
    // Update the font of the "bonfire" text element
    TextElement* bonfireText = uiManager.getTextElement("bonfire");
    if (bonfireText) {
        bonfireText->font = fonts[currentFontIndex];
        
        // Center text horizontally - calculate position based on text width
        display.setFont(fonts[currentFontIndex]);
        int textWidth = display.textWidth("bonfire");
        bonfireText->x = (display.width() - textWidth) / 2;
    } else {
        Serial.println("Error: 'bonfire' text element not found!");
        return;
    }
    
    // Update the font label content
    TextElement* fontLabel = uiManager.getTextElement("font_label");
    if (fontLabel) {
        fontLabel->content = fontNames[currentFontIndex];
        
        // Center the font label using our label font
        display.setFont(labelFont);
        int labelWidth = display.textWidth(fontNames[currentFontIndex]);
        fontLabel->x = (display.width() - labelWidth) / 2;
    } else {
        Serial.println("Error: 'font_label' text element not found!");
        return;
    }
    
    // Update the font counter (e.g., "1/10")
    TextElement* fontCounter = uiManager.getTextElement("font_counter");
    if (fontCounter) {
        String counterText = String(currentFontIndex + 1) + "/" + String(fontCount);
        fontCounter->content = counterText;
    } else {
        Serial.println("Error: 'font_counter' text element not found!");
        return;
    }
    
    // Redraw the active screen to reflect the changes
    uiManager.drawActiveScreen();
}