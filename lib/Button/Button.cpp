#include "Button.hpp"
#include "UIManager.hpp"

// Initialize static members
UIManager* Button::uiManager = nullptr;

// Set the UI manager reference
void Button::setUIManager(UIManager* manager) {
    uiManager = manager;
}

// Default constructor
Button::Button() {
  x = 0;
  y = 0;
  width = 0;
  height = 0;
  radius = 0;
  label = "";
  screen = 0;
  active = false;
  action = NULL;
}

// Constructor with parameters
Button::Button(int x, int y, int width, int height, int radius, 
               String label, int screen, void (*action)()) {
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
  this->radius = radius;
  this->label = label;
  this->screen = screen;
  this->active = (screen == 0); // Default to active on main screen
  this->action = action;
}

// Draw the button on the TFT
void Button::draw(LGFX& tft) const {
  if (!uiManager) return;  // Safety check
  
  // Get current state from UIManager
  bool isLightMode = uiManager->getLightMode();
  bool isInvertedAccent = uiManager->getInvertAccent();
  
  // Calculate text dimensions using current font (no need to set font)
  int textWidth = tft.textWidth(label);
  int textHeight = tft.fontHeight();
  
  // Determine colors based on current state
  uint16_t fillColor = isInvertedAccent ? TFT_RED : TFT_DARKGRAY;
  uint16_t outlineColor = isInvertedAccent ? TFT_DARKGRAY : TFT_RED;
  uint16_t textColor = isLightMode ? TFT_BLACK : TFT_WHITE;
  
  // Draw filled rounded rectangle for button
  tft.fillRoundRect(x, y, width, height, radius, fillColor);
  
  // Draw thicker outline
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
  bool xInBounds = (touchX >= x && touchX <= x + width);
  bool yInBounds = (touchY >= y && touchY <= y + height);
  Serial.printf("Button bounds check for '%s': X(%d-%d) Y(%d-%d) Touch(X:%d Y:%d) -> X:%s Y:%s\n",
                label.c_str(), x, x + width, y, y + height, touchX, touchY,
                xInBounds ? "true" : "false", yInBounds ? "true" : "false");
  return xInBounds && yInBounds;
}