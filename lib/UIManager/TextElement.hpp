#pragma once

#include <Arduino.h>
#include <map>
#include <string>
#include "LGFX_Config.h"

class TextElement
{
public:
  TextElement() = default;

  // Constructor with direct font pointer - automatically opts OUT of font changes
  TextElement(int x, int y, uint16_t color, String content, int screen,
              const lgfx::IFont *font = nullptr)
      : x(x), y(y), color(color), content(content), screen(screen),
        font(font), originalFont(font), active(false),
        fontString(""), size9pt(true), allowFontChange(false) {}

  // Constructor with font string and size - automatically opts IN to font changes
  TextElement(int x, int y, uint16_t color, String content, int screen,
              const std::string &fontString, bool size9pt)
      : x(x), y(y), color(color), content(content), screen(screen),
        fontString(fontString), size9pt(size9pt), active(false), allowFontChange(true)
  {
    // Convert fontString and size9pt to actual font object
    font = getFontFromNameAndSize(fontString, size9pt);
    originalFont = font;
  }

  // Constructor with explicit allowFontChange parameter (for special cases)
  TextElement(int x, int y, uint16_t color, String content, int screen,
              const std::string &fontString, bool size9pt, bool allowFontChange)
      : x(x), y(y), color(color), content(content), screen(screen),
        fontString(fontString), size9pt(size9pt), active(false),
        allowFontChange(allowFontChange)
  {
    // Convert fontString and size9pt to actual font object
    font = getFontFromNameAndSize(fontString, size9pt);
    originalFont = font;
  }

  void draw(LGFX &tft) const
  {
    if (!active)
      return;

    // If we have a specific font for this element, use it
    if (font)
    {
      tft.setFont(font);
    }
    // Otherwise use whatever font is currently set on the TFT

    tft.setTextColor(color);
    tft.setCursor(x, y);
    tft.print(content);
  }

  // Function to update font while maintaining size

  // Update font preserving size from original font
  void updateFontPreserveSize(const std::string &baseFontName)
  {
    // Only update if this element allows font changes
    if (!allowFontChange) return;

    Serial.println("Updating font to: " + String(baseFontName.c_str()));
    Serial.println("Font Content Changed: " + content);
    fontString = baseFontName;
    font = getFontFromNameAndSize(fontString, size9pt);
  }

  static const lgfx::IFont *getFontFromNameDefault(const std::string &baseFontName)
  {
    return getFontFromNameAndSize(baseFontName, true);
  }

  // Helper function to get a font object from a font name string
  static const lgfx::IFont *getFontFromName(const std::string &fontName)
  {
    static const std::map<std::string, const lgfx::IFont *> nameToFontMap = {
        // Standard font families
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
        {"FreeSerifItalic12", &lgfx::fonts::FreeSerifItalic12pt7b}};

    auto it = nameToFontMap.find(fontName);
    if (it != nameToFontMap.end())
    {
      return it->second;
    }
    return nullptr; // Font not found
  }
  static const lgfx::IFont *getFontFromNameAndSize(const std::string &baseFontName, bool size9pt)
  {
    // Special case for Font2 which doesn't follow the 9pt/12pt pattern
    if (baseFontName == "Font2")
    {
      return &lgfx::fonts::Font2;
    }

    std::string fullFontName = baseFontName + (size9pt ? "9" : "12");
    return getFontFromName(fullFontName);
  }

public:
  int x, y;
  uint16_t color;
  String content;
  int screen;
  const lgfx::IFont *font;
  const lgfx::IFont *originalFont; // Store the original font to maintain size ratio
  bool active;

  // New properties to store font information
  std::string fontString; // Base font name without size (e.g., "FreeSerif")
  bool size9pt;           // Size flag: true for 9pt, false for 12pt
  bool allowFontChange;   // Flag to allow/disallow font changes
};