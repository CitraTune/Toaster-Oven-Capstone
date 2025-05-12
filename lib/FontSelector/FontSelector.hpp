// font_selector/include/font_selector.h
#pragma once

#include <LovyanGFX.hpp>
#include "UIManager.hpp"

class FontSelector {
    private:
        static const GFXfont* fonts[];
        LGFX& _display;
        UIManager& _uiManager;
        int currentFontIndex;
        
        void drawBonfireText();
        
        // Add button handlers
        static FontSelector* instance;
        static void handleLeftPress();
        static void handleRightPress();

    public:
        FontSelector(LGFX& display, UIManager& uiManager);
        void setup();
        void loop();
        void updateDisplay();
        void decrementFont();
        void incrementFont();
};