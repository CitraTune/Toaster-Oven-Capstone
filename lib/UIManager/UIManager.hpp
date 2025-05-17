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
    LGFX& _tft;
    std::unordered_map<std::string, Button> buttons;
    std::unordered_map<std::string, TextElement> textElements;
    int currentScreen;
    bool lightMode;
    bool invertAccent;
    uint16_t outlineColor;
    
    // Add temperature variables
    int soakTemp = 150;
    int reflowTemp = 230;

public:
  // Constructor
  UIManager(LGFX& tft);
  
  // setup the UI
  void setup();

  void loop();
  
  // Create a new button with a key
  bool createButton(const std::string& key, int x, int y, int width, int height, int radius,
                   uint16_t color, uint16_t textColor, String label, 
                   int screen, void (*action)());
                   
  // Create a new text element with a key
  bool createTextElement(const std::string& key, int x, int y, uint16_t color, String content,
                       int screen, const lgfx::IFont* font = nullptr);
  
  // Draw all active buttons for the current screen
  void drawButtons();
  
  // Draw all active text elements for the current screen
  void drawTextElements();
  
  // Check if a button was pressed
  void checkButtonPress(int touchX, int touchY);
  
  // Navigate to a specific screen
  void navigateToScreen(int screen);
  
  // Draw the active screen with all its elements
  void drawActiveScreen();
  
  // Update button colors based on theme
  void updateButtonColors();

  // Get button by key (returns nullptr if not found)
  Button* getButton(const std::string& key);

  // Get text element by key (returns nullptr if not found)
  TextElement* getTextElement(const std::string& key);

  // Add temperature methods
  void increaseSoakTemp(bool coarse = false) {
      soakTemp += (coarse ? 10 : 1);
      updateTemperatureDisplay("soakTemp");
  }
  
  void decreaseSoakTemp(bool coarse = false) {
      soakTemp -= (coarse ? 10 : 1);
      updateTemperatureDisplay("soakTemp");
  }
  
  void increaseReflowTemp(bool coarse = false) {
      reflowTemp += (coarse ? 10 : 1);
      updateTemperatureDisplay("reflowTemp");
  }
  
  void decreaseReflowTemp(bool coarse = false) {
      reflowTemp -= (coarse ? 10 : 1);
      updateTemperatureDisplay("reflowTemp");
  }

  void updateTemperatureDisplay(const std::string& tempType) {
      TextElement* element = getTextElement(tempType);
      if (element) {
          element->content = String(tempType == "soakTemp" ? soakTemp : reflowTemp) + " C";
          drawActiveScreen();
      }
  }
  
  public:
    void toggleLightMode() {
        lightMode = !lightMode;
        drawActiveScreen();
    }
    
    void toggleInvertAccent() {
        invertAccent = !invertAccent;
        updateButtonColors();
        drawActiveScreen();
    }
    
    int getScreen() {
        return currentScreen;
    }
    
    bool getLightMode() const {
        return lightMode;
    }
};