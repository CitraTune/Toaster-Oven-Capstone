#pragma once
#include <Arduino.h>
#include "LGFX_Config.h"

class UIManager;  // Forward declaration

// Button structure to make management easier
class Button {
private:
  static UIManager* uiManager;  // Static reference to UIManager
  
public:
  int x;
  int y;
  int width;
  int height;
  int radius;          // Corner radius
  String label;        // Button text
  int screen;          // Which screen this button belongs to
  bool active;         // Is this button currently active/visible
  void (*action)();    // Function pointer for button action
  
  static void setUIManager(UIManager* manager);  // Set the UI manager reference

  // Constructor
  Button();
    // Constructor with parameters
  Button(int x, int y, int width, int height, int radius,
         String label, int screen, void (*action)());
    // Method to draw the button on the TFT
  void draw(LGFX& tft) const;
  
  // Method to check if a point is inside the button
  bool contains(int touchX, int touchY);
};

