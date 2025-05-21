#pragma once
#include "Button.hpp"
#include <Arduino.h>
#include <unordered_map>
#include <string>
#include "LGFX_Config.h"
#include "TextElement.hpp"

class UIManager {
private:
    static LGFX* _display;
    static std::unordered_map<std::string, Button> buttons;
    static std::unordered_map<std::string, TextElement> textElements;
    static int currentScreen;
    static bool lightMode;
    static bool invertAccent;
    static uint16_t outlineColor;
    static std::string currentFont;  // Track current font

    // Private constructor to prevent instantiation
    UIManager() {}

public:
    static int soakTemp;
    static int reflowTemp;

    // setup the UI
    static void setup(LGFX& tft);
    static LGFX& display(); // Getter for access
    static void loop();

    // Create a new button with a key
    static bool createButton(const std::string& key, int x, int y, int width, int height, int radius,
                             String label, int screen, void (*action)());

    static const lgfx::IFont* getFontFromNameAndSize(const std::string& baseFontName, bool size9pt);
    static const lgfx::IFont *getFontFromNameDefault(const std::string &baseFontName);

    // Create a new text element with a key
    static bool createTextElement(const std::string& key, int x, int y, uint16_t color, String content,
                                  int screen, const lgfx::IFont* font = nullptr);

    // New overloaded method using font string and size
    static bool createTextElement(const std::string& key, int x, int y, uint16_t color, String content,
                                  int screen, const std::string& fontString, bool size9pt);

    // New overloaded method with opt-out for font changes
    static bool createTextElement(const std::string& key, int x, int y, uint16_t color, String content,
                                  int screen, const std::string& fontString, bool size9pt, bool allowFontChange);

    // Example of a method to update all text elements' fonts
    static void updateAllTextElementFontsPreserveSize(const std::string& fontString);
    static void updateAllFontsPreserveSize(const std::string &fontString);

    // Draw all active buttons for the current screen
    static void drawButtons();

    // Draw all active text elements for the current screen
    static void drawTextElements();

    // Check if a button was pressed
    static void checkButtonPress(int touchX, int touchY);

    // Navigate to a specific screen
    static void setCurrentScreen(int screen);

    // Draw the active screen with all its elements
    static void drawActiveScreen();

    // Get button by key (returns nullptr if not found)
    static Button* getButton(const std::string& key);

    // Get text element by key (returns nullptr if not found)
    static TextElement* getTextElement(const std::string& key);
    static void updateTextElementContent(const std::string& key, const String& newText);

    // Add temperature methods
    static void increaseSoakTemp(bool coarse = false) {
        soakTemp += (coarse ? 10 : 1);
        updateTemperatureDisplay("soakTemp", soakTemp);
    }

    static void decreaseSoakTemp(bool coarse = false) {
        soakTemp -= (coarse ? 10 : 1);
        updateTemperatureDisplay("soakTemp", soakTemp);
    }

    static void increaseReflowTemp(bool coarse = false) {
        reflowTemp += (coarse ? 10 : 1);
        updateTemperatureDisplay("reflowTemp", reflowTemp);
    }

    static void decreaseReflowTemp(bool coarse = false) {
        reflowTemp -= (coarse ? 10 : 1);
        updateTemperatureDisplay("reflowTemp", reflowTemp);
    }

    static void updateTemperatureDisplay(const std::string& tempType, int tempValue) {
        TextElement* element = getTextElement(tempType);
        if (element) {
            element->content = String(tempValue) + " C";
            drawActiveScreen();
        }
    }

    static void toggleLightMode() {
        lightMode = !lightMode;
        drawActiveScreen();
    }

    static void toggleInvertAccent() {
        invertAccent = !invertAccent;
        drawActiveScreen();
    }

    static int getScreen() {
        return currentScreen;
    }

    static bool getLightMode() {
        return lightMode;
    }

    static bool getInvertAccent() {
        return invertAccent;
    }

    static void setFont(const lgfx::IFont* font);
    static const lgfx::IFont* getCurrentFont() { return TextElement::getFontFromNameDefault(currentFont); }
    static void setCurrentFont(std::string font) { currentFont = font; }
    static void redraw();

    // Add getter for text elements
    static std::unordered_map<std::string, TextElement>& getTextElements() {
        return textElements;
    }

    // Add this method to the UIManager class
    static void updateButtonText(const std::string &key, const String &newLabel) {
        Button* button = getButton(key);
        if (button) {
            button->label = newLabel;
            // Redraw the screen to show the change
            drawActiveScreen();
        } else {
            Serial.println("Warning: Button with key '" + String(key.c_str()) + "' not found!");
        }
    }

    static int getCurrentScreen() { return currentScreen; }
};
