#include "UIManager.hpp"
//This allows you to pass in functions as arguments
typedef void (*ButtonAction)();

// Constructor
UIManager::UIManager(LGFX& tft) : _tft(tft) {
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

void UIManager::loop() {
  // Implementation here
}

// Create a new button with a key
bool UIManager::createButton(const std::string& key, int x, int y, int width, int height, int radius,
                          uint16_t color, uint16_t textColor, String label, 
                          int screen, ButtonAction action) {
  // Check if key already exists
  if (buttons.find(key) != buttons.end()) {
    Serial.println("Warning: Button with key '" + String(key.c_str()) + "' already exists!");
    return false;
  }
  Serial.println("Creating button: " + label + " with key: " + String(key.c_str()));
  Button newButton(x, y, width, height, radius, color, textColor, label, screen, action);
  newButton.active = (screen == currentScreen);
  
  buttons[key] = newButton;
  return true;
}

// Create a new text element with a key
bool UIManager::createTextElement(const std::string& key, int x, int y, uint16_t color, String content,
                                int screen, const lgfx::IFont* font) {
  // Check if key already exists
  if (textElements.find(key) != textElements.end()) {
    Serial.println("Warning: Text element with key '" + String(key.c_str()) + "' already exists!");
    return false;
  }

  Serial.println("Creating text element: " + content + " with key: " + String(key.c_str()));
  TextElement newElement(x, y, color, content, screen, font);
  newElement.active = (screen == currentScreen);

  textElements[key] = newElement;
  return true;
}

// Draw all active buttons for the current screen
void UIManager::drawButtons() {
  for (auto& pair : buttons) {
    Button& button = pair.second;
    if (button.active) {
      button.draw(_tft);
    }
  }
}

// Draw all active text elements for the current screen
void UIManager::drawTextElements() {
  for (auto& pair : textElements) {
    TextElement& element = pair.second;
    if (element.active) {
      element.draw(_tft);
    }
  }
}

// Check if a button was pressed. Activates action if it's pressed.
void UIManager::checkButtonPress(int touchX, int touchY) {
  for (auto& pair : buttons) {
    Button& button = pair.second;
    // Only check active buttons
    if (!button.active) continue;
    
    // Check if touch is within button boundaries
    if (button.contains(touchX, touchY)) {
      // Execute button action if assigned
      if (button.action != NULL) {
        Serial.println("Button pressed: " + button.label);
        button.action();
      }
      
      break;  // Exit after handling one button press
    }
  }
}

// Navigate to a specific screen
void UIManager::navigateToScreen(int screen) {
  currentScreen = screen;
  
  // Update button active states
  for (auto& pair : buttons) {
    Button& button = pair.second;
    button.active = (button.screen == currentScreen);
  }
  
  // Update text element active states
  for (auto& pair : textElements) {
    TextElement& element = pair.second;
    element.active = (element.screen == currentScreen);
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
  
  for (auto& pair : buttons) {
    Button& button = pair.second;
    button.color = invertAccent ? TFT_LIGHTGRAY : TFT_ORANGE;
  }
}

// Get button by key (returns nullptr if not found)
Button* UIManager::getButton(const std::string& key) {
  auto it = buttons.find(key);
  if (it != buttons.end()) {
    return &(it->second);
  }
  return nullptr;
}

// Get text element by key (returns nullptr if not found)
TextElement* UIManager::getTextElement(const std::string& key) {
  auto it = textElements.find(key);
  if (it != textElements.end()) {
    return &(it->second);
  }
  return nullptr;
}
