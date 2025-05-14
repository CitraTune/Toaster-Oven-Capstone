#include "UIManager.hpp"
//This allows you to pass in functions as arguments
typedef void (*ButtonAction)();

// Constructor
UIManager::UIManager(LGFX& tft) : _tft(tft) {
  buttonCount = 0;
  textElementCount = 0;  // Initialize text element count
  currentScreen = SCREEN_MAIN;
  lightMode = false;
  invertAccent = false;
  outlineColor = TFT_DARKGRAY;
}

// setup the UI
void UIManager::setup() {
  // Set the default font
  _tft.setFont(&lgfx::fonts::FreeSans9pt7b);
}

void UIManager::loop(){

  
}

// Create a new button and return its index
int UIManager::createButton(int x, int y, int width, int height, int radius, 
                          uint16_t color, uint16_t textColor, String label, 
                          int screen, ButtonAction action) {
  if (buttonCount >= MAX_BUTTONS) {
    Serial.println("Warning: Maximum button count reached!");
    return -1;
  }
  Serial.println("Creating button: " + label);
  Button newButton(x, y, width, height, radius, color, textColor, label, screen, action);
  newButton.active = (screen == currentScreen);
  
  buttons[buttonCount] = newButton;
  return buttonCount++;
}

// Create a new text element and return its index
int UIManager::createTextElement(int x, int y, uint16_t color, String content,
                                int screen, const lgfx::IFont* font) {
  if (textElementCount >= MAX_TEXT_ELEMENTS) {
    Serial.println("Warning: Maximum text element count reached!");
    return -1;
  }

  Serial.println("Creating text element: " + content);
  TextElement newElement(x, y, color, content, screen, font);
  newElement.active = (screen == currentScreen);

  textElements[textElementCount] = newElement;
  return textElementCount++;
}

// Draw all active buttons for the current screen
void UIManager::drawButtons() {
  for (int i = 0; i < buttonCount; i++) {
    if (buttons[i].active) {
      buttons[i].draw(_tft);
    }
  }
}

// Draw all active text elements for the current screen
void UIManager::drawTextElements() {
  for (int i = 0; i < textElementCount; i++) {
    textElements[i].draw(_tft);
  }
}

// Check if a button was pressed. Activates action if it's pressed.
void UIManager::checkButtonPress(int touchX, int touchY) {
  for (int i = 0; i < buttonCount; i++) {
    // Only check active buttons
    if (!buttons[i].active) continue;
    
    // Check if touch is within button boundaries
    if (buttons[i].contains(touchX, touchY)) {
      // Execute button action if assigned
      if (buttons[i].action != NULL) {
        Serial.println("Button pressed: " + buttons[i].label);
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
  
  // Update text element active states
  for (int i = 0; i < textElementCount; i++) {
    textElements[i].active = (textElements[i].screen == currentScreen);
  }

  // Draw the new screen
  drawActiveScreen();
}

// Draw the active screen with all its elements
void UIManager::drawActiveScreen() {
  // Clear the screen
  _tft.fillScreen(lightMode ? TFT_WHITE : TFT_BLACK);

  // First draw the text elements
  drawTextElements();

  // Then draw the buttons on top
  drawButtons();

  // Note: Other components like graphs would be drawn after this
}

// Update button colors based on theme
void UIManager::updateButtonColors() {
  outlineColor = invertAccent ? TFT_ORANGE : TFT_DARKGRAY;
  
  for (int i = 0; i < buttonCount; i++) {
    buttons[i].color = invertAccent ? TFT_LIGHTGRAY : TFT_ORANGE;
  }
}
