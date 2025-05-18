#pragma once

#include <Arduino.h>
#include <unordered_map>
#include <string>
#include "LGFX_Config.h"
#include "Button.hpp"
#include "TextElement.hpp"

// Screen constants
#define SCREEN_MAIN 0
#define SCREEN_SETTINGS 1

// Screen dimensions
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

class UIManager {
private:
    static LGFX* _tft;
    static std::unordered_map<std::string, Button> buttons;
    static std::unordered_map<std::string, TextElement> textElements;
    static int currentScreen;
    static bool lightMode;
    static bool invertAccent;
    static uint16_t outlineColor;
    static const lgfx::IFont* currentFont;  // Track current font
    static LGFX _display; // The global display object

    
    // Private constructor to prevent instantiation
    UIManager() {}
public:

    static int soakTemp;
    static int reflowTemp;
    // Initialize the UI system
    static void initialize(LGFX& tft);
  // setup the UI
    static void setup();
    static LGFX& display(); // Getter for access
    static void loop();
  
  // Create a new button with a key
    static bool createButton(const std::string& key, int x, int y, int width, int height, int radius,
                   String label, int screen, void (*action)());
                   
  // Create a new text element with a key
    static bool createTextElement(const std::string& key, int x, int y, uint16_t color, String content,
                       int screen, const lgfx::IFont* font = nullptr);
  
  // Draw all active buttons for the current screen
    static void drawButtons();
  
  // Draw all active text elements for the current screen
    static void drawTextElements();
  
  // Check if a button was pressed
    static void checkButtonPress(int touchX, int touchY);
  
  // Navigate to a specific screen
    static void navigateToScreen(int screen);
  
  // Draw the active screen with all its elements
    static void drawActiveScreen();
  
  // Get button by key (returns nullptr if not found)
    static Button* getButton(const std::string& key);

  // Get text element by key (returns nullptr if not found)
    static TextElement* getTextElement(const std::string& key);

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
    static const lgfx::IFont* getCurrentFont() { return currentFont; }
    static void redraw();

  // Add getter for text elements
    static std::unordered_map<std::string, TextElement>& getTextElements() {
    return textElements;
  }
};