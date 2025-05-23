#pragma once
#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <bb_captouch.h>
#include "UIManager.hpp"

// Number of fonts in the array
#define NUM_FONTS 10

class FontSelectorGUI {
public:
  // Constructor
  FontSelectorGUI(LGFX& display, UIManager& uiManager);
  
  // setup all components
  void setup();
  
  // Main loop
  void loop();
  
  // Action methods
  void updateFont();  // New method to update font and refresh UI
  
  // Static instance for button callbacks
  static FontSelectorGUI* instance;
  static void handleLeftPress();
  static void handleRightPress();

private:
  // Components
  LGFX& display;
  UIManager& uiManager;
  BBCapTouch touch;
  int currentFontIndex;

  // Debounce variables
  unsigned long lastTouchTime;
  const unsigned long debounceDelay = 200; // 200ms debounce delay

  // Font related variables
  static const GFXfont* fonts[];
  // Font names array for display
  static const char* fontNames[];
  // Default font for the font name label
  const lgfx::IFont* labelFont;
};