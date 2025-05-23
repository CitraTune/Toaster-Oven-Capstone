#include "UIManager.hpp"
#include "LineArtManager.hpp"

// This allows you to pass in functions as arguments
typedef void (*ButtonAction)();

// Static member initialization
LGFX *UIManager::_display = nullptr;
std::unordered_map<std::string, Button> UIManager::buttons;
std::unordered_map<std::string, TextElement> UIManager::textElements;
int UIManager::currentScreen = SCREEN_MAIN;
bool UIManager::lightMode = false;
bool UIManager::invertAccent = false;
uint16_t UIManager::outlineColor = TFT_DARKGRAY;
std::string UIManager::currentFont = "FreeSans";
int UIManager::soakTemp = 150;
int UIManager::reflowTemp = 230;


LGFX &UIManager::display()
{
    return *UIManager::_display;
}

// setup the UI
void UIManager::setup(LGFX &tft)
{
    _display = &tft;
    currentScreen = SCREEN_MAIN;
    lightMode = false;
    invertAccent = false;
    outlineColor = TFT_DARKGRAY;
    _display->setFont(TextElement::getFontFromNameDefault(currentFont));
}

// Create a new button with a key
bool UIManager::createButton(const std::string &key, int x, int y, int width, int height, int radius,
                             String label, int screen, ButtonAction action)
{
    // Check if key already exists
    if (buttons.find(key) != buttons.end())
    {
        Serial.println("Warning: Button with key '" + String(key.c_str()) + "' already exists!");
        return false;
    }
    // Serial.println("Creating button: " + label + " with key: " + String(key.c_str()));
    Button newButton(x, y, width, height, radius, label, screen, action);
    newButton.active = (screen == currentScreen);

    buttons[key] = newButton;
    return true;
}

// UIManager methods for text element creation

bool UIManager::createTextElement(const std::string &key, int x, int y, uint16_t color, String content,
                                  int screen, const lgfx::IFont *font)
{
    // Check if key already exists
    if (textElements.find(key) != textElements.end())
    {
        Serial.println("Warning: Text element with key '" + String(key.c_str()) + "' already exists!");
        return false;
    }

    Serial.println("Creating text element: " + content + " with key: " + String(key.c_str()));
    // Call constructor with explicit rotation=0, datum=0
    TextElement newElement(x, y, color, content, screen, font, 0, 0);
    newElement.active = (screen == currentScreen);

    textElements[key] = newElement;
    return true;
}

bool UIManager::createTextElement(const std::string &key, int x, int y, uint16_t color, String content,
                                  int screen, const std::string &fontString, bool size9pt)
{
    // Check if key already exists
    if (textElements.find(key) != textElements.end())
    {
        Serial.println("Warning: Text element with key '" + String(key.c_str()) + "' already exists!");
        return false;
    }

    Serial.println("Creating text element: " + content + " with key: " + String(key.c_str()));
    // Call constructor with explicit rotation=0, datum=0
    TextElement newElement(x, y, color, content, screen, fontString, size9pt, 0, 0);
    newElement.active = (screen == currentScreen);

    textElements[key] = newElement;
    return true;
}

bool UIManager::createTextElementWithFontControl(const std::string &key, int x, int y, uint16_t color, String content,
                              int screen, const std::string &fontString, bool size9pt, bool allowFontChange)
{
    // Check if key already exists
    if (textElements.find(key) != textElements.end())
    {
        Serial.println("Warning: Text element with key '" + String(key.c_str()) + "' already exists!");
        return false;
    }

    Serial.println("Creating text element with font control: " + content + " with key: " + String(key.c_str()));
    TextElement newElement(x, y, color, content, screen, fontString, size9pt, allowFontChange, 0, 0);
    newElement.active = (screen == currentScreen);
    textElements[key] = newElement;
    return true;
}

bool UIManager::createRotatedTextElement(const std::string &key, int x, int y, uint16_t color, String content,
                              int screen, const lgfx::IFont *font, bool allowFontChange,
                              int rotation, int datum)
{
    // Check if key already exists
    if (textElements.find(key) != textElements.end())
    {
        Serial.println("Warning: Text element with key '" + String(key.c_str()) + "' already exists!");
        return false;
    }

    Serial.println("Creating rotated text element: " + content + " with key: " + String(key.c_str()));
    TextElement newElement(x, y, color, content, screen, font, rotation, datum);
    newElement.active = (screen == currentScreen);
    newElement.allowFontChange = allowFontChange;

    textElements[key] = newElement;
    return true;
}

bool UIManager::createRotatedTextElement(const std::string &key, int x, int y, uint16_t color, String content,
                              int screen, const std::string &fontString, bool size9pt,
                              int rotation, int datum)
{
    // Check if key already exists
    if (textElements.find(key) != textElements.end())
    {
        Serial.println("Warning: Text element with key '" + String(key.c_str()) + "' already exists!");
        return false;
    }

    Serial.println("Creating rotated text element: " + content + " with key: " + String(key.c_str()));
    TextElement newElement(x, y, color, content, screen, fontString, size9pt, true, rotation, datum);
    newElement.active = (screen == currentScreen);

    textElements[key] = newElement;
    return true;
}

void UIManager::updateAllTextElementFontsPreserveSize(const std::string &fontString)
{
    for (auto it = textElements.begin(); it != textElements.end(); ++it)
    {
        it->second.updateFontPreserveSize(fontString);
    }
}

void UIManager::updateAllFontsPreserveSize(const std::string &fontString)
{
    for (auto it = textElements.begin(); it != textElements.end(); ++it)
    {
        it->second.updateFontPreserveSize(fontString);
    }
    for (auto it = buttons.begin(); it != buttons.end(); ++it)
    {
        it->second.updateFontPreserveSize(fontString);
    }
    UIManager::setCurrentFont(fontString);
}

// Draw all active buttons for the current screen
void UIManager::drawButtons()
{
    for (const auto &pair : buttons)
    {
        const Button &button = pair.second;
        if (button.active)
        {
            button.draw(UIManager::display());
        }
    }
}

// Draw all active text elements for the current screen
void UIManager::drawTextElements()
{
    for (const auto &pair : textElements)
    {
        const TextElement &element = pair.second;
        if (element.active)
        {
            element.draw(UIManager::display());
        }
    }
}

void UIManager::updateTextElementContent(const std::string &key, const String &newText)
{
    TextElement *element = getTextElement(key);
    if (element)
    {
        element->content = newText;
        if (element->active)
        {
            element->draw(*_display); // Redraw only if it's active on the current screen
        }
    }
    else
    {
        Serial.println("Warning: Text element key not found: " + String(key.c_str()));
    }
}

// Check if a button was pressed. Activates action if it's pressed.
void UIManager::checkButtonPress(int touchX, int touchY)
{
    Serial.printf("Checking touch at X: %d, Y: %d for screen %d\n", touchX, touchY, currentScreen);

    for (auto &pair : buttons)
    {
        Button &button = pair.second;
        // Only check active buttons
        if (!button.active)
        {
            Serial.printf("Skipping inactive button: %s (screen: %d)\n", button.label.c_str(), button.screen);
            continue;
        }

        Serial.printf("Checking button '%s' at X:%d Y:%d W:%d H:%d\n",
                      button.label.c_str(), button.x, button.y, button.width, button.height);

        // Check if touch is within button boundaries
        if (button.contains(touchX, touchY))
        {
            Serial.printf("Touch hit detected on button: %s!\n", button.label.c_str());
            // Execute button action if assigned
            if (button.action != NULL)
            {
                Serial.printf("Executing action for button: %s\n", button.label.c_str());
                button.action();
            }
            else
            {
                Serial.printf("Warning: No action assigned to button: %s\n", button.label.c_str());
            }

            break; // Exit after handling one button press
        }
    }
}

// Navigate to a specific screen
void UIManager::setCurrentScreen(int screen)
{
    currentScreen = screen;

    // Update button active states
    for (auto &pair : buttons)
    {
        Button &button = pair.second;
        button.active = (button.screen == currentScreen);
    }

    // Update text element active states
    for (auto &pair : textElements)
    {
        TextElement &element = pair.second;
        element.active = (element.screen == currentScreen);
    }
    LineArtManager::updateActiveState(screen);
    // Draw the new screen
    drawActiveScreen();
}

// Draw the active screen with all its elements
void UIManager::drawActiveScreen()
{
    // Clear the screen
    _display->fillScreen(lightMode ? TFT_WHITE : TFT_BLACK);

    // First draw the text elements
    drawTextElements();

    // Then draw the buttons on top
    drawButtons();
    
    LineArtManager::draw();

}

// Get button by key (returns nullptr if not found)
Button *UIManager::getButton(const std::string &key)
{
    auto it = buttons.find(key);
    if (it != buttons.end())
    {
        return &(it->second);
    }
    return nullptr;
}

// Get text element by key (returns nullptr if not found)
TextElement *UIManager::getTextElement(const std::string &key)
{
    auto it = textElements.find(key);
    if (it != textElements.end())
    {
        return &(it->second);
    }
    return nullptr;
}

void UIManager::redraw()
{
    _display->fillScreen(lightMode ? TFT_WHITE : TFT_BLACK);

    // Draw all active buttons
    for (const auto &pair : buttons)
    {
        const Button &button = pair.second;
        if (button.active)
        {
            button.draw(*_display);
        }
    }

    // Draw all active text elements
    for (const auto &pair : textElements)
    {
        const TextElement &element = pair.second;
        if (element.active)
        {
            element.draw(*_display);
        }
    }
}
