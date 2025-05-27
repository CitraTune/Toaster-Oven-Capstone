#pragma once

#include <Arduino.h>
#include "LGFX_Config.h"
#include <string>

class Button {
public:
  // Constructors
  Button();
  Button(int x, int y, int width, int height, int radius, 
         String label, int screen, void (*action)());
  
  // Static method to set the global display reference
  static void setGlobalDisplay(LGFX& display);

  // Draw without requiring display parameter
  void draw() const;
  // Check if a point is inside the button
  bool contains(int touchX, int touchY);
  
  // Public members - could be made private with getters/setters
  int x, y, width, height, radius;
  String label;
  int screen;
  bool active;
  void (*action)();
  
  // Store the current font family but always use 9pt size
  const lgfx::IFont* buttonFont;
  std::string buttonFontString;
  
  // Static method to get a 9pt font from the font family
  static const lgfx::IFont* get9ptFont(const std::string &fontFamily);

  // Update the font and size of the button
  void updateFontPreserveSize(const std::string &baseFontName);

  // Update the label of the button
  void updateLabel(const String &newLabel);

private:
  // Static method to get a 9pt font from the font family

};