#pragma once

#include <Arduino.h>
#include "LGFX_Config.h"
#include "Button.hpp"
#include "TextElement.hpp"  // Add this include

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
  
  // Text element management
  static const int MAX_TEXT_ELEMENTS = 10;
  TextElement textElements[MAX_TEXT_ELEMENTS];
  int textElementCount;
  
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
                   
  // Create a new text element and return its index
  int createTextElement(int x, int y, uint16_t color, String content, 
                       int screen, const lgfx::IFont* font = nullptr);
  
  // Draw all active buttons for the current screen
  void drawButtons();
  
  // Draw all active text elements for the current screen
  void drawTextElements();
  
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