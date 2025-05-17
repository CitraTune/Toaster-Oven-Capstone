#pragma once

#include <LovyanGFX.hpp>
#include <vector>
#include <string>
#include "Button.hpp"
#include "UIManager.hpp"
#include "GraphManager.hpp"
#include "TemperatureManager.hpp"

// Screen definitions
#define SCREEN_MAIN 0
#define SCREEN_SETTINGS 1
#define SCREEN_FONTS 2

// Display dimensions
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

// Touch debounce time
#define DEBOUNCE_DELAY 250

class IntegratedFontReflowGUI {
public:
    IntegratedFontReflowGUI();
    
    // Setup and main loop
    void setup();
    void loop();
    
    // Touch input handling
    void handleTouch(int x, int y);
    
    // Screen navigation functions
    void goToSettings();
    void goToMain();
    void goToFonts();
    
    // Theme toggle functions
    void toggleLightMode();
    void toggleInvertAccent();
    void toggleGraphSize();
    
    // Font navigation functions
    void nextFont();
    void prevFont();
    
    // Getters for component managers
    UIManager* getUIManager() { return &uiManager; }
    TemperatureManager* getTempManager() { return &tempManager; }
    
private:
    // Display and UI components
    static LGFX display;
    UIManager uiManager;
    GraphManager graphManager;
    TemperatureManager tempManager;
    
    // Touch handling
    unsigned long lastTouchTime;
    const unsigned long debounceDelay = DEBOUNCE_DELAY;
    
    // Button setup methods
    void setupButtons();
    void setupFontScreenButtons();
    
    // Font management
    int currentFontIndex;
    void updateFontDisplay();
    
    // Font definitions
    static const GFXfont* fonts[];
    static const char* fontNames[];
    static const int fontCount;
    
    // Sample text for font display
    static const char* sampleText;
    
    // Helper functions for UI
    void updateButtonVisibility();
    void redrawCurrentScreen();
};

