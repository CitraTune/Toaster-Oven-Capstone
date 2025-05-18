#pragma once

#include <Arduino.h>
#include "LGFX_Config.h"

// Forward declaration to avoid circular dependency
class UIManager;

class Button {
public:
  // Constructors
  Button();
  Button(int x, int y, int width, int height, int radius, 
         String label, int screen, void (*action)());
  
  // Draw the button on the display
  void draw(LGFX& tft) const;
  
  // Check if a point is inside the button
  bool contains(int touchX, int touchY);
  
  // Set the UI manager reference
  static void setUIManager(UIManager* manager);
  
  // Public members - could be made private with getters/setters
  int x, y, width, height, radius, screen;
  String label;
  bool active;
  void (*action)();
  
private:
  // Static reference to the UI manager
  static UIManager* uiManager;
  
  // Store the current font family but always use 9pt size
  const lgfx::IFont* buttonFont;
  
  // Static method to get a 9pt font from the font family
  static const lgfx::IFont* get9ptFont(const lgfx::IFont* font);
};