#include "FontSelector.hpp"

// setup static instance pointer
FontSelector* FontSelector::instance = nullptr;

// Define the static font array
const GFXfont* FontSelector::fonts[] = {
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

FontSelector::FontSelector(LGFX& display, UIManager& uiManager)
    : _display(display), _uiManager(uiManager), currentFontIndex(0) {
    instance = this;
}

void FontSelector::handleLeftPress() {
    if (instance) {
        instance->decrementFont();
    }
}

void FontSelector::handleRightPress() {
    if (instance) {
        instance->incrementFont();
    }
}

void FontSelector::setup() {
    // Create buttons
    _uiManager.createButton(10, _display.height() - 60, 60, 40, 8, 
                          TFT_DARKGREY, TFT_WHITE, "<", 0, handleLeftPress);
    
    _uiManager.createButton(_display.width() - 70, _display.height() - 60, 60, 40, 8,
                           TFT_DARKGREY, TFT_WHITE, ">", 0, handleRightPress);
    
    drawBonfireText();
}

void FontSelector::decrementFont() {
    int fontCount = sizeof(fonts) / sizeof(fonts[0]);
    currentFontIndex = (currentFontIndex - 1 + fontCount) % fontCount;
    updateDisplay();
}

void FontSelector::incrementFont() {
    int fontCount = sizeof(fonts) / sizeof(fonts[0]);
    currentFontIndex = (currentFontIndex + 1) % fontCount;
    updateDisplay();
}

void FontSelector::loop() {
    // The UIManager will handle button presses now
    // No need for additional logic here
}

void FontSelector::drawBonfireText() {
    _display.setFont(fonts[currentFontIndex]);
    _display.fillScreen(TFT_BLACK);
    _display.setTextColor(TFT_WHITE);
    
    // Get text dimensions using LovyanGFX methods
    const char* text = "bonfire";
    int textWidth = _display.textWidth(text);
    int textHeight = _display.fontHeight();
    
    // Calculate center position
    int centerX = (_display.width() - textWidth) / 2;
    int centerY = (_display.height() + textHeight) / 2;
    
    _display.setCursor(centerX, centerY);
    _display.print(text);
}

void FontSelector::updateDisplay() {
    drawBonfireText();
}
