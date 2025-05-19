#pragma once

#include <Arduino.h>
#include <map>
#include <string>
#include "LGFX_Config.h"

class TextElement {
public:
  TextElement() = default;
  

  
  
  void draw(LGFX& tft) const {
    if (!active) return;

    // If we have a specific font for this element, use it
    if (font) {
      tft.setFont(font);
    }
    // Otherwise use whatever font is currently set on the TFT
    
    tft.setTextColor(color);
    tft.setCursor(x, y);
    tft.print(content);
  }

  // Helper function to get a font object from a font name string and size
  static const lgfx::IFont* getFontFromNameAndSize(const std::string& baseFontName, bool size9pt) {
    std::string fullFontName = baseFontName + (size9pt ? "9" : "12");
    return getFontFromName(fullFontName);
  }

  // Helper function to get a font object from a font name string
  static const lgfx::IFont* getFontFromName(const std::string& fontName) {
    static const std::map<std::string, const lgfx::IFont*> nameToFontMap = {
      {"FreeMono9", &lgfx::fonts::FreeMono9pt7b},
      {"FreeMono12", &lgfx::fonts::FreeMono12pt7b},
      {"FreeMonoBold9", &lgfx::fonts::FreeMonoBold9pt7b},
      {"FreeMonoBold12", &lgfx::fonts::FreeMonoBold12pt7b},
      {"FreeMonoOblique9", &lgfx::fonts::FreeMonoOblique9pt7b},
      {"FreeMonoOblique12", &lgfx::fonts::FreeMonoOblique12pt7b},
      {"FreeSans9", &lgfx::fonts::FreeSans9pt7b},
      {"FreeSans12", &lgfx::fonts::FreeSans12pt7b},
      {"FreeSansBold9", &lgfx::fonts::FreeSansBold9pt7b},
      {"FreeSansBold12", &lgfx::fonts::FreeSansBold12pt7b},
      {"FreeSansBoldOblique9", &lgfx::fonts::FreeSansBoldOblique9pt7b},
      {"FreeSansBoldOblique12", &lgfx::fonts::FreeSansBoldOblique12pt7b},
      {"FreeSerif9", &lgfx::fonts::FreeSerif9pt7b},
      {"FreeSerif12", &lgfx::fonts::FreeSerif12pt7b},
      {"FreeSerifBold9", &lgfx::fonts::FreeSerifBold9pt7b},
      {"FreeSerifBold12", &lgfx::fonts::FreeSerifBold12pt7b},
      {"FreeSerifBoldItalic9", &lgfx::fonts::FreeSerifBoldItalic9pt7b},
      {"FreeSerifBoldItalic12", &lgfx::fonts::FreeSerifBoldItalic12pt7b},
      {"FreeSerifItalic9", &lgfx::fonts::FreeSerifItalic9pt7b},
      {"FreeSerifItalic12", &lgfx::fonts::FreeSerifItalic12pt7b}
    };

    auto it = nameToFontMap.find(fontName);
    if (it != nameToFontMap.end()) {
      return it->second;
    }
    return nullptr; // Font not found
  }

  // Helper function to get a font name string from a font object
  static std::string getFontName(const lgfx::IFont* font) {
    static const std::map<const lgfx::IFont*, std::string> fontToNameMap = {
      {&lgfx::fonts::FreeMono9pt7b, "FreeMono9"},
      {&lgfx::fonts::FreeMono12pt7b, "FreeMono12"},
      {&lgfx::fonts::FreeMonoBold9pt7b, "FreeMonoBold9"},
      {&lgfx::fonts::FreeMonoBold12pt7b, "FreeMonoBold12"},
      {&lgfx::fonts::FreeMonoOblique9pt7b, "FreeMonoOblique9"},
      {&lgfx::fonts::FreeMonoOblique12pt7b, "FreeMonoOblique12"},
      {&lgfx::fonts::FreeSans9pt7b, "FreeSans9"},
      {&lgfx::fonts::FreeSans12pt7b, "FreeSans12"},
      {&lgfx::fonts::FreeSansBold9pt7b, "FreeSansBold9"},
      {&lgfx::fonts::FreeSansBold12pt7b, "FreeSansBold12"},
      {&lgfx::fonts::FreeSansBoldOblique9pt7b, "FreeSansBoldOblique9"},
      {&lgfx::fonts::FreeSansBoldOblique12pt7b, "FreeSansBoldOblique12"},
      {&lgfx::fonts::FreeSerif9pt7b, "FreeSerif9"},
      {&lgfx::fonts::FreeSerif12pt7b, "FreeSerif12"},
      {&lgfx::fonts::FreeSerifBold9pt7b, "FreeSerifBold9"},
      {&lgfx::fonts::FreeSerifBold12pt7b, "FreeSerifBold12"},
      {&lgfx::fonts::FreeSerifBoldItalic9pt7b, "FreeSerifBoldItalic9"},
      {&lgfx::fonts::FreeSerifBoldItalic12pt7b, "FreeSerifBoldItalic12"},
      {&lgfx::fonts::FreeSerifItalic9pt7b, "FreeSerifItalic9"},
      {&lgfx::fonts::FreeSerifItalic12pt7b, "FreeSerifItalic12"}
    };

    auto it = fontToNameMap.find(font);
    if (it != fontToNameMap.end()) {
      return it->second;
    }
    return ""; // Unknown font
  }

  // Function to update font size and family
  void updateFontSettings(const std::string& newFontString, bool newSize9pt) {
    // Only update if this element allows font changes
    if (!allowFontChange) return;
    
    // Store new font settings
    fontString = newFontString;
    size9pt = newSize9pt;
    
    // Update the font object
    font = getFontFromNameAndSize(fontString, size9pt);
    originalFont = font; // Also update original font reference
  }
  
  // Function to update font while maintaining size
  void updateFont(const std::string& baseFontName, bool use9pt = true) {
    // Only update if this element allows font changes
    if (!allowFontChange) return;
    
    std::string fullFontName = baseFontName + (use9pt ? "9" : "12");
    const lgfx::IFont* newFont = getFontFromName(fullFontName);
    
    if (newFont) {
      font = newFont;
      originalFont = newFont;
      
      // Update stored settings
      fontString = baseFontName;
      size9pt = use9pt;
    }
  }
  
  // Function to update font from a font pointer
  void updateFont(const lgfx::IFont* newFont) {
    // Only update if this element allows font changes
    if (!allowFontChange) return;
    
    if (newFont) {
      // Try to extract font name and size
      std::string newFontName = getFontName(newFont);
      if (!newFontName.empty()) {
        // Extract base name and size
        bool is9pt = newFontName.find("9") != std::string::npos;
        size_t digitPos = newFontName.find_last_of("0123456789");
        if (digitPos != std::string::npos) {
          fontString = newFontName.substr(0, digitPos);
          size9pt = is9pt;
        }
      }
      
      // Update the font object
      font = newFont;
      originalFont = newFont;
    }
  }

  // Update font preserving size from original font
  void updateFontPreserveSize(const std::string& baseFontName) {
    // Only update if this element allows font changes
    if (!allowFontChange) return;
    
    // Update using the stored size setting
    updateFont(baseFontName, size9pt);
  }

public:
  int x, y;
  uint16_t color;
  String content;
  int screen;
  const lgfx::IFont* font;
  const lgfx::IFont* originalFont; // Store the original font to maintain size ratio
  bool active;
  
  // New properties to store font information
  std::string fontString;  // Base font name without size (e.g., "FreeSerif")
  bool size9pt;           // Size flag: true for 9pt, false for 12pt
  bool allowFontChange;   // Flag to allow/disallow font changes
};

