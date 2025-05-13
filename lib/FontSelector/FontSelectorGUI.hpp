#pragma once
#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <bb_captouch.h>
#include "UIManager.hpp"

class FontSelectorGUI {
public:
  // Constructor
  FontSelectorGUI(LGFX& display, UIManager& uiManager);
  
  // setup all components
  void setup();
  
  // Main loop
  void loop();
  
  // Action methods
  void decrementFont();
  void incrementFont();
  void updateDisplay();
  
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
  
  // Display methods
  void drawBonfireText();
  
  // Static instance for button callbacks
  static FontSelectorGUI* instance;
  static void handleLeftPress();
  static void handleRightPress();
};