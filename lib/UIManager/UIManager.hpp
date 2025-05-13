#pragma once

#include <Arduino.h>
#include "LGFX_Config.h"
#include "Button.hpp"

// Screen constants
#define SCREEN_MAIN 0
#define SCREEN_SETTINGS 1

// Screen dimensions
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

class UIManager {
public:
  // Constructor
  UIManager(LGFX& tft);
  
  // Button management
  static const int MAX_BUTTONS = 14;
  Button buttons[MAX_BUTTONS];
  int buttonCount;
  
  // Screen management
  int currentScreen;
  
  // Theme settings
  bool lightMode;
  bool invertAccent;
  uint16_t outlineColor;
  
  // setup the UI
  void setup();

  void loop();
  
  // Create a new button and return its index
  int createButton(int x, int y, int width, int height, int radius, 
                   uint16_t color, uint16_t textColor, String label, 
                   int screen, void (*action)());
  
  // Draw all active buttons for the current screen
  void drawButtons();
  
  // Check if a button was pressed
  void checkButtonPress(int touchX, int touchY);
  
  // Navigate to a specific screen
  void navigateToScreen(int screen);
  
  // Draw the active screen with all its elements
  void drawActiveScreen();
  
  // Update button colors based on theme
  void updateButtonColors();

private:
  LGFX& _tft;
};

