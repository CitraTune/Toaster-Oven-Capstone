#pragma once

#include <Arduino.h>
#include <map>
#include <string>
#include "LGFX_Config.h"

class TextElement
{
public:
  TextElement() = default;

  TextElement(int x, int y, uint16_t color, String content, int screen,
              const lgfx::IFont *font = nullptr,
              int rotation = 0, int datum = 0)
      : x(x), y(y), color(color), content(content), screen(screen),
        font(font), originalFont(font), active(false),
        fontString(""), size9pt(true), allowFontChange(false),
        textRotation(rotation), textDatum(datum) {}

  TextElement(int x, int y, uint16_t color, String content, int screen,
              const std::string &fontString, bool size9pt,
              int rotation = 0, int datum = 0)
      : x(x), y(y), color(color), content(content), screen(screen),
        fontString(fontString), size9pt(size9pt), active(false), allowFontChange(true),
        textRotation(rotation), textDatum(datum)
  {
    font = getFontFromNameAndSize(fontString, size9pt);
    originalFont = font;
  }

  TextElement(int x, int y, uint16_t color, String content, int screen,
              const std::string &fontString, bool size9pt, bool allowFontChange,
              int rotation = 0, int datum = 0)
      : x(x), y(y), color(color), content(content), screen(screen),
        fontString(fontString), size9pt(size9pt), active(false),
        allowFontChange(allowFontChange), textRotation(rotation), textDatum(datum)
  {
    font = getFontFromNameAndSize(fontString, size9pt);
    originalFont = font;
  }

  void draw(LGFX &tft) const
  {
    if (!active) return;

    if (textRotation != 0)
    {
      // Create a sprite to draw rotated text
      LGFX_Sprite sprite(&tft);
      sprite.setFont(font);
      sprite.setTextColor(color);
      sprite.setTextDatum(textDatum);
      sprite.setColorDepth(8); // 8-bit color is a good balance

      // Estimate width/height of the text (safely)
      int16_t textWidth = sprite.textWidth(content);
      int16_t textHeight = 12; // fallback if font is null

      // Add margin to avoid cutoff
      int margin = 10;
      int spriteW = textWidth + margin;
      int spriteH = textHeight + margin;

      sprite.createSprite(spriteW, spriteH);
      sprite.fillSprite(TFT_TRANSPARENT);
      sprite.setPivot(spriteW / 2, spriteH / 2);

      sprite.drawString(content, spriteW / 2, spriteH / 2);
      sprite.pushRotateZoom(x, y, textRotation * DEG_TO_RAD, 1.0, 1.0);

      sprite.deleteSprite();
    }
    else
    {
      tft.setFont(font);
      tft.setTextColor(color);
      tft.setTextDatum(textDatum);
      tft.drawString(content, x, y);
    }
  }

  void updateFontPreserveSize(const std::string &baseFontName)
  {
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

  static const lgfx::IFont *getFontFromName(const std::string &fontName)
  {
    static const std::map<std::string, const lgfx::IFont *> nameToFontMap = {
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
    return nullptr;
  }

  static const lgfx::IFont *getFontFromNameAndSize(const std::string &baseFontName, bool size9pt)
  {
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
  const lgfx::IFont *originalFont;
  bool active;

  std::string fontString;
  bool size9pt;
  bool allowFontChange;

  int textRotation;
  int textDatum;
};