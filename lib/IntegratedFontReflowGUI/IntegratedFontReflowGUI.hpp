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

class IntegratedFontReflowGUI {
public:
    IntegratedFontReflowGUI();
    
    // Setup and main loop
    static void setup();
    static void loop();

    // Make these public so main can access them
    static LGFX display;
    int touchX, touchY;
    
    // Touch input handling
    static void handleTouch(int x, int y);
    
    // Screen navigation functions


    
    // Make uiManager public so TextSetup.hpp can access it
    UIManager uiManager;
    
    // Font management variables (moved to public)
    static int currentFontIndex;
    static const GFXfont* fonts[];
    static const char* fontNames[];
    static const int fontCount;
    
    // Text setup methods
    static void setupTextElements();
    static void setupTemperatureElements();
    static void updateTemperatureDisplay();
    // Button setup methods
    static void setupButtons();
    static void setupFontScreenButtons();
    
    // Temperature settings (moved to public)
    static int soakTemp;
    static int reflowTemp;
    
    static void increaseSoakTemp(bool coarse = false);
    static void decreaseSoakTemp(bool coarse = false);
    static void increaseReflowTemp(bool coarse = false);
    static void decreaseReflowTemp(bool coarse = false);
    static void updateFontDisplay();
    static void updateButtonVisibility();
    static void redrawCurrentScreen();
    
private:
    // Display and UI components
    GraphManager graphManager;
    BBCapTouch touch;
    
    // Touch handling
    unsigned long lastTouchTime;
    const unsigned long debounceDelay = DEBOUNCE_DELAY;

    // Sample text for font display
    static const char* sampleText;
    
    // Helper functions for UI
    

    // Button setup methods
    int buttonHeight = 40; // Add this to match the constant in ButtonSetup.hpp
};
