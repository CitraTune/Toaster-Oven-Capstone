#include "UIManager.hpp"
//This allows you to pass in functions as arguments
typedef void (*ButtonAction)();

// Static member initialization
LGFX* UIManager::_tft = nullptr;
std::unordered_map<std::string, Button> UIManager::buttons;
std::unordered_map<std::string, TextElement> UIManager::textElements;
int UIManager::currentScreen = SCREEN_MAIN;
bool UIManager::lightMode = false;
bool UIManager::invertAccent = false;
uint16_t UIManager::outlineColor = TFT_DARKGRAY;
const lgfx::IFont* UIManager::currentFont = nullptr;
int UIManager::soakTemp = 150;
int UIManager::reflowTemp = 230;
LGFX UIManager::_display;

LGFX& UIManager::display() {
    return _display;
}

// Initialize the UI system
void UIManager::initialize(LGFX& tft) {
    _tft = &tft;
    currentScreen = SCREEN_MAIN;
    lightMode = false;
    invertAccent = false;
    outlineColor = TFT_DARKGRAY;
    Button::setUIManager(nullptr); // We'll need to adjust Button class to use static methods
}

// setup the UI
void UIManager::setup() {
    currentFont = &lgfx::fonts::FreeSans9pt7b;
    _tft->setFont(currentFont);
}

void UIManager::loop() {
    // Implementation here
}

// Create a new button with a key
bool UIManager::createButton(const std::string& key, int x, int y, int width, int height, int radius,
                          String label, int screen, ButtonAction action) {
    // Check if key already exists
    if (buttons.find(key) != buttons.end()) {
        Serial.println("Warning: Button with key '" + String(key.c_str()) + "' already exists!");
        return false;
    }
    Serial.println("Creating button: " + label + " with key: " + String(key.c_str()));
    Button newButton(x, y, width, height, radius, label, screen, action);
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
    for (const auto& pair : buttons) {
        const Button& button = pair.second;
        if (button.active) {
            button.draw(*_tft);
        }
    }
}

// Draw all active text elements for the current screen
void UIManager::drawTextElements() {
    for (const auto& pair : textElements) {
        const TextElement& element = pair.second;
        if (element.active) {
            element.draw(*_tft);
        }
    }
}

// Check if a button was pressed. Activates action if it's pressed.
void UIManager::checkButtonPress(int touchX, int touchY) {
    Serial.printf("Checking touch at X: %d, Y: %d for screen %d\n", touchX, touchY, currentScreen);
    
    for (auto& pair : buttons) {
        Button& button = pair.second;
        // Only check active buttons
        if (!button.active) {
            Serial.printf("Skipping inactive button: %s (screen: %d)\n", button.label.c_str(), button.screen);
            continue;
        }
        
        Serial.printf("Checking button '%s' at X:%d Y:%d W:%d H:%d\n", 
                    button.label.c_str(), button.x, button.y, button.width, button.height);
        
        // Check if touch is within button boundaries
        if (button.contains(touchX, touchY)) {
            Serial.printf("Touch hit detected on button: %s!\n", button.label.c_str());
            // Execute button action if assigned
            if (button.action != NULL) {
                Serial.printf("Executing action for button: %s\n", button.label.c_str());
                button.action();
            } else {
                Serial.printf("Warning: No action assigned to button: %s\n", button.label.c_str());
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
    _tft->fillScreen(lightMode ? TFT_WHITE : TFT_BLACK);

    // First draw the text elements
    drawTextElements();

    // Then draw the buttons on top
    drawButtons();

    // Note: Other components like graphs would be drawn after this
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

void UIManager::setFont(const lgfx::IFont* font) {
    _tft->setFont(font);
    currentFont = font;
    
    // Update all text elements to use the new font while maintaining their relative sizes
    for (auto& pair : textElements) {
        pair.second.updateFont(font);
    }
    
    // Force a redraw of all active elements
    redraw();
}

void UIManager::redraw() {
    _tft->fillScreen(lightMode ? TFT_WHITE : TFT_BLACK);
    
    // Draw all active buttons
    for (const auto& pair : buttons) {
        const Button& button = pair.second;
        if (button.active) {
            button.draw(*_tft);
        }
    }
    
    // Draw all active text elements
    for (const auto& pair : textElements) {
        const TextElement& element = pair.second;
        if (element.active) {
            element.draw(*_tft);
        }
    }
}