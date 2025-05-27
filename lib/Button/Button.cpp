#include "Button.hpp"
#include "../UIManager/UIManager.hpp"
#include "../TextElement/TextElement.hpp" // Include for access to font mapping functions

// Reference to the global display object
static LGFX* globalDisplay = nullptr;

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

// Set the global display reference (call this once during initialization)
void Button::setGlobalDisplay(LGFX& display) {
  globalDisplay = &display;
}

void Button::updateFontPreserveSize(const std::string &baseFontName) {
  // Store font name if needed
  this->buttonFontString = baseFontName;
  // Always use 9pt size
  this->buttonFont = TextElement::getFontFromNameDefault(baseFontName);
}

// Draw the button on the TFT - no display parameter needed
void Button::draw() const {
  // Make sure we have a valid display reference
  if (!globalDisplay) {
    Serial.println("Error: Display not initialized for Button class");
    return;
  }
  
  // Get current state from UIManager
  bool isLightMode = UIManager::getLightMode();
  bool isInvertedAccent = UIManager::getInvertAccent();
  
  // Use current system font family but always 9pt size
  const lgfx::IFont* currentFont = UIManager::getCurrentFont();

  globalDisplay->setFont(buttonFont); // Set button font for sizing
  // Calculate text dimensions using button font
  int textWidth = globalDisplay->textWidth(label);
  int textHeight = globalDisplay->fontHeight();
  
  // Determine colors based on current state
  uint16_t fillColor = isInvertedAccent ? TFT_DARKGRAY : TFT_RED;
  uint16_t outlineColor = isInvertedAccent ? TFT_RED : TFT_DARKGRAY;
  uint16_t textColor = isLightMode ? TFT_BLACK : TFT_WHITE;
  
  // Draw filled rounded rectangle for button
  globalDisplay->fillRoundRect(x, y, width, height, radius, fillColor);
  
  // Draw thicker outline
  globalDisplay->drawRoundRect(x, y, width, height, radius, outlineColor);
  globalDisplay->drawRoundRect(x - 1, y - 1, width + 2, height + 2, radius + 1, outlineColor);
  
  // Calculate text position to center it in the button (horizontally and vertically)
  int textX = x + (width - textWidth) / 2;
  int textY = y + (height - textHeight) / 2;
  
  // Draw text with button font
  globalDisplay->setTextColor(textColor);
  globalDisplay->setCursor(textX, textY);
  globalDisplay->print(label);
  // Restore the system font
  globalDisplay->setFont(currentFont);
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

// Update the button label and redraw
void Button::updateLabel(const String &newLabel) {
  // Update the label
  this->label = newLabel;

  // Only redraw if the button is active and we have a display
  if (active && globalDisplay) {
    // Clear the button area first to prevent ghosting
    globalDisplay->fillRect(x, y, width, height, TFT_BLACK); // Use appropriate background color

    // Redraw the button with the new label
    draw();
  }
}
