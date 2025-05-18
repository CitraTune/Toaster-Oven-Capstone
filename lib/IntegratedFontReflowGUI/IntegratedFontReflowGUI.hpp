#pragma once

#include <LovyanGFX.hpp>
#include <vector>
#include <string>
#include "Button.hpp"
#include "UIManager.hpp"
#include <bb_captouch.h>
#include "GraphManager.hpp"

// Screen definitions
#define SCREEN_MAIN 0
#define SCREEN_SETTINGS 1
#define SCREEN_FONTS 2

// Display dimensions
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

// Touch debounce time
#define DEBOUNCE_DELAY 200

// Button dimensions
#define BUTTON_WIDTH 110
#define BUTTON_HEIGHT 40

class IntegratedFontReflowGUI {
public:
    
    // Setup and main loop
    void setup();
    void loop();

    // Make these public so main can access them
    static LGFX display;
    int touchX, touchY;
    
    // Touch input handling
    static void handleTouch(int x, int y);
    
    // Font management variables (moved to public)
    static const GFXfont* fonts[];
    static const char* fontNames[];
    static const int fontCount;

    static int lastTouchTime;
    static int currentFontIndex;
    static int soakTemp;
    static int reflowTemp;
    static int buttonHeight;
    static int debounceDelay;
    
    // Text setup methods
    static void setupTextElements();
    static void setupTemperatureElements();
    static void updateTemperatureDisplay();
    // Button setup methods
    static void setupButtons();
    static void setupFontScreenButtons();
    
    
    static void increaseSoakTemp(bool coarse = false);
    static void decreaseSoakTemp(bool coarse = false);
    static void increaseReflowTemp(bool coarse = false);
    static void decreaseReflowTemp(bool coarse = false);
    static void updateFontDisplay();
    static void updateButtonVisibility();
    static void redrawCurrentScreen();
    
    
private:
    BBCapTouch touch;
 
};
