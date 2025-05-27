#pragma once
#include <Arduino.h>
#include <map>
#include <string>
#include "LGFX_Config.h"

class TextElement {
public:
  // Static reference to the global display
  static LGFX* globalDisplay;
  
  // Static method to set the global display reference
  static void setGlobalDisplay(LGFX& display);
  
  // Constructors
  TextElement();
  
  TextElement(int x, int y, uint16_t color, String content, int screen,
              const lgfx::IFont *font = nullptr);
              
  TextElement(int x, int y, uint16_t color, String content, int screen,
              const std::string &fontString, bool size9pt);
  
  // Draw method that uses the global display
  void draw() const;
  
  // Font management methods
  void updateFontPreserveSize(const std::string &baseFontName);
  static const lgfx::IFont *getFontFromNameDefault(const std::string &baseFontName);
  static const lgfx::IFont *getFontFromName(const std::string &fontName);
  static const lgfx::IFont *getFontFromNameAndSize(const std::string &baseFontName, bool size9pt);
  const lgfx::IFont* getFont() const;
  
  // Member variables
  int x, y;
  uint16_t color;
  String content;
  int screen;
  const lgfx::IFont *font;
  const lgfx::IFont *originalFont;
  bool active;
  std::string fontString;
  bool size9pt;
  bool allowFontChange;
};