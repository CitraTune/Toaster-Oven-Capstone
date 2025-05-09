#include "Button.h"

// Default constructor
Button::Button() {
  x = 0;
  y = 0;
  width = 0;
  height = 0;
  radius = 0;
  color = TFT_ORANGE;
  textColor = TFT_WHITE;
  label = "";
  screen = 0;
  active = false;
  action = NULL;
}

// Constructor with parameters
Button::Button(int x, int y, int width, int height, int radius, 
               uint16_t color, uint16_t textColor, String label, 
               int screen, void (*action)()) {
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
  this->radius = radius;
  this->color = color;
  this->textColor = textColor;
  this->label = label;
  this->screen = screen;
  this->active = (screen == 0); // Default to active on main screen
  this->action = action;
}

// Draw the button on the TFT
void Button::draw(LGFX& tft) {
  // Calculate text dimensions for this particular button text
  tft.setFont(&lgfx::fonts::FreeSans9pt7b);
  int textWidth = tft.textWidth(label);
  int textHeight = tft.fontHeight();
  
  // Draw filled rounded rectangle for button
  tft.fillRoundRect(x, y, width, height, radius, color);
  
  // Draw thicker outline
  uint16_t outlineColor = TFT_DARKGRAY; // You might want to make this configurable
  tft.drawRoundRect(x, y, width, height, radius, outlineColor);
  tft.drawRoundRect(x - 1, y - 1, width + 2, height + 2, radius + 1, outlineColor);
  
  // Calculate text position to center it in the button (horizontally and vertically)
  int textX = x + (width - textWidth) / 2;
  int textY = y + (height - textHeight) / 2;
  
  // Draw text
  tft.setTextColor(textColor);
  tft.setCursor(textX, textY);
  tft.print(label);
}

// Check if a point is inside the button
bool Button::contains(int touchX, int touchY) {
  return (touchX >= x && touchX <= x + width &&
          touchY >= y && touchY <= y + height);
}