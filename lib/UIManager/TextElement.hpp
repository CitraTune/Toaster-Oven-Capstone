#pragma once

#include <Arduino.h>
#include <unordered_map>
#include <string>
#include "LGFX_Config.h"

class TextElement {
public:
  TextElement() = default;
  
  TextElement(int x, int y, uint16_t color, String content, int screen, 
              const lgfx::IFont* font = nullptr) 
      : x(x), y(y), color(color), content(content), screen(screen), 
        font(font), active(false) {}
  
  void draw(LGFX& tft) {
  if (!active) return;

  if (font) tft.setFont(font);
  tft.setTextColor(color);
  tft.setCursor(x, y);
  tft.print(content);
}

  int x, y;
  uint16_t color;
  String content;
  int screen;
  const lgfx::IFont* font;
  bool active;
};