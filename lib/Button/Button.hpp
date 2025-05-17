#pragma once
#include <Arduino.h>
#include "LGFX_Config.h"

// Button structure to make management easier
class Button {
public:
  int x;
  int y;
  int width;
  int height;
  int radius;          // Corner radius
  uint16_t color;      // Button color
  uint16_t textColor;  // Text color
  String label;        // Button text
  int screen;          // Which screen this button belongs to
  bool active;         // Is this button currently active/visible
  void (*action)();    // Function pointer for button action

  // Constructor
  Button();
  
  // Constructor with parameters
  Button(int x, int y, int width, int height, int radius, 
         uint16_t color, uint16_t textColor, String label, 
         int screen, void (*action)());
  
  // Method to draw the button on the TFT
  void draw(LGFX& tft, uint16_t outlineColor);
  
  // Method to check if a point is inside the button
  bool contains(int touchX, int touchY);
};

