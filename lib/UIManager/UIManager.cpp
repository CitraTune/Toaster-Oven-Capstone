#include "UIManager.hpp"

// Constructor
UIManager::UIManager(LGFX& tft) : _tft(tft) {
  buttonCount = 0;
  currentScreen = SCREEN_MAIN;
  lightMode = false;
  invertAccent = false;
  outlineColor = TFT_DARKGRAY;
}

// Initialize the UI
void UIManager::init() {
  // Set the default font
  _tft.setFont(&lgfx::fonts::FreeSans9pt7b);
}

// Create a new button and return its index
int UIManager::createButton(int x, int y, int width, int height, int radius, 
                          uint16_t color, uint16_t textColor, String label, 
                          int screen, void (*action)()) {
  if (buttonCount >= MAX_BUTTONS) {
    Serial.println("Warning: Maximum button count reached!");
    return -1;
  }
  
  Button newButton(x, y, width, height, radius, color, textColor, label, screen, action);
  newButton.active = (screen == currentScreen);
  
  buttons[buttonCount] = newButton;
  return buttonCount++;
}

// Draw all active buttons for the current screen
void UIManager::drawButtons() {
  for (int i = 0; i < buttonCount; i++) {
    if (buttons[i].active) {
      buttons[i].draw(_tft);
    }
  }
}

// Check if a button was pressed
void UIManager::checkButtonPress(int touchX, int touchY) {
  for (int i = 0; i < buttonCount; i++) {
    // Only check active buttons
    if (!buttons[i].active) continue;
    
    // Check if touch is within button boundaries
    if (buttons[i].contains(touchX, touchY)) {
      // Execute button action if assigned
      if (buttons[i].action != NULL) {
        buttons[i].action();
      }
      
      break;  // Exit after handling one button press
    }
  }
}

// Navigate to a specific screen
void UIManager::navigateToScreen(int screen) {
  currentScreen = screen;
  
  // Update button active states
  for (int i = 0; i < buttonCount; i++) {
    buttons[i].active = (buttons[i].screen == currentScreen);
  }
  
  // Draw the new screen
  drawActiveScreen();
}

// Draw the active screen with all its elements
void UIManager::drawActiveScreen() {
  uint16_t bgColor = lightMode ? TFT_WHITE : TFT_BLACK;
  uint16_t textColor = lightMode ? TFT_BLACK : TFT_WHITE;
  
  _tft.fillScreen(bgColor);  // Clear screen
  
  // Define small margin for titles
  const int MARGIN = 10;
  
  if (currentScreen == SCREEN_MAIN) {
    // Draw main screen title in top left with margin
    _tft.setTextColor(textColor);
    _tft.setFont(&lgfx::fonts::FreeSans12pt7b);  // Increased font size
    _tft.setCursor(MARGIN, MARGIN);  // Adjusted position
    _tft.print("Main Menu");
    // Note: Graph will be drawn by the GraphManager after this
  } 
  else if (currentScreen == SCREEN_SETTINGS) {
    // Draw settings screen title in top left with margin
    _tft.setTextColor(textColor);
    _tft.setFont(&lgfx::fonts::FreeSans12pt7b);  // Increased font size
    _tft.setCursor(MARGIN, MARGIN);  // Adjusted position
    _tft.print("Settings");
  }

  // Draw a white outline around the text for the screen label box
  int textWidth = _tft.textWidth(currentScreen == SCREEN_MAIN ? "Main Menu" : "Settings");
  int textHeight = _tft.fontHeight();
  int outlineOffset = 5; // Offset for the outline
  
  uint16_t boxColor = lightMode ? TFT_BLACK : TFT_WHITE;

  // Draw the right angle outline
  _tft.drawLine(0, MARGIN + textHeight + outlineOffset, MARGIN + textWidth + outlineOffset, 
                MARGIN + textHeight + outlineOffset, boxColor); // Top line
  _tft.drawLine(MARGIN + textWidth + outlineOffset, 0, MARGIN + textWidth + outlineOffset, 
                MARGIN + textHeight + outlineOffset, boxColor); // Left line
  
  // Draw all active buttons for this screen
  drawButtons();
}

// Update button colors based on theme
void UIManager::updateButtonColors() {
  outlineColor = invertAccent ? TFT_ORANGE : TFT_DARKGRAY;
  
  for (int i = 0; i < buttonCount; i++) {
    buttons[i].color = invertAccent ? TFT_LIGHTGRAY : TFT_ORANGE;
  }
}