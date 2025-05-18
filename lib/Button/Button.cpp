#include "Button.hpp"
#include "UIManager.hpp"
#include "TextElement.hpp" // Include for access to font mapping functions



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
  buttonFont = &lgfx::fonts::FreeSans9pt7b; // Default to FreeSans9pt
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
  this->buttonFont = &lgfx::fonts::FreeSans9pt7b; // Default to FreeSans9pt
}

// Helper method to get equivalent 9pt font from any font
const lgfx::IFont* Button::get9ptFont(const lgfx::IFont* font) {
  if (!font) return &lgfx::fonts::FreeSans9pt7b; // Default
  
  // Get font name using TextElement's helper
  std::string fontName = TextElement::getFontName(font);
  
  if (fontName.empty()) return &lgfx::fonts::FreeSans9pt7b; // Default if unknown
  
  // Extract base font name (without size)
  std::string baseFont;
  size_t digitPos = fontName.find_last_of("0123456789");
  if (digitPos != std::string::npos) {
    baseFont = fontName.substr(0, digitPos);
  } else {
    baseFont = fontName;  
  }
  
  // Append 9pt size and get font
  std::string font9pt = baseFont + "9";
  const lgfx::IFont* result = TextElement::getFontFromName(font9pt);
  
  // Return found font or default
  return result ? result : &lgfx::fonts::FreeSans9pt7b;
}

// Draw the button on the TFT
void Button::draw(LGFX& tft) const {
  
  // Get current state from UIManager
  bool isLightMode = UIManager::getLightMode();
  bool isInvertedAccent = UIManager::getInvertAccent();
  
  // Use current system font family but always 9pt size
  const lgfx::IFont* currentFont = UIManager::getCurrentFont();
  const lgfx::IFont* font9pt = get9ptFont(currentFont);
  
  // Set button font for text measurements
  tft.setFont(font9pt);
  
  // Calculate text dimensions using button font
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
  
  // Draw text with button font
  tft.setTextColor(textColor);
  tft.setCursor(textX, textY);
  tft.print(label);
  
  // Restore the system font
  tft.setFont(currentFont);
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