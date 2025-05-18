#pragma once

#include "IntegratedFontReflowGUI.hpp"

// Setup menu labels and text elements
inline void IntegratedFontReflowGUI::setupTextElements() {
  // Add menu labels for each screen
  uiManager.createTextElement(
      "main_menu_label",
      10,                    // x - left margin
      10,                    // y - top margin
      TFT_WHITE,            // color
      "Main Menu",          // content
      SCREEN_MAIN,          // screen
      &lgfx::fonts::FreeSans9pt7b  // font
  );

  uiManager.createTextElement(
      "settings_menu_label",
      10,                    // x
      10,                    // y
      TFT_WHITE,            // color
      "Settings",           // content
      SCREEN_SETTINGS,      // screen
      &lgfx::fonts::FreeSans9pt7b  // font
  );

  uiManager.createTextElement(
      "font_menu_label",
      10,                    // x
      25,                    // y
      TFT_WHITE,            // color
      "Font Selection",     // content
      SCREEN_FONTS,         // screen
      &lgfx::fonts::FreeSans9pt7b  // font
  );
  
  // Create sample text element for font screen
  uiManager.createTextElement(
    "sample_text",            // key
    0,                        // x - will be calculated in updateFontDisplay
    80,                       // y - about 1/4 down the screen
    TFT_WHITE,                // color
    "Sample Text",            // content
    SCREEN_FONTS,             // screen
    fonts[currentFontIndex]   // font
  );
  
  // Create font name label
  uiManager.createTextElement(
    "font_name",              // key
    0,                        // x - will be calculated in updateFontDisplay
    120,                      // y - below sample text
    TFT_YELLOW,               // color
    fontNames[currentFontIndex], // content
    SCREEN_FONTS,             // screen
    &lgfx::fonts::Font2       // font (small font for label)
  );
  
  // Create font counter label (e.g., "1/10")
  String counterText = String(currentFontIndex + 1) + "/" + String(fontCount);
  uiManager.createTextElement(
    "font_counter",           // key
    10,                       // x - left side
    10,                       // y - top
    TFT_CYAN,                 // color
    counterText,              // content
    SCREEN_FONTS,             // screen
    &lgfx::fonts::Font2       // font (small font for counter)
  );
  
  // Temperature labels in settings screen
  setupTemperatureElements();
}

// Setup temperature display elements in settings screen
inline void IntegratedFontReflowGUI::setupTemperatureElements() {
  // Create temperature labels and values
  uiManager.createTextElement(
      "soak_temp_label",
      10, 50,
      TFT_WHITE,
      "Soak Temp:",
      SCREEN_SETTINGS,
      &lgfx::fonts::FreeSans9pt7b
  );

  uiManager.createTextElement(
      "soak_temp_value",
      10, 75,
      TFT_YELLOW,
      String(soakTemp) + " C",
      SCREEN_SETTINGS,
      &lgfx::fonts::FreeSans12pt7b
  );

  uiManager.createTextElement(
      "reflow_temp_label",
      10, 140,
      TFT_WHITE,
      "Reflow Temp:",
      SCREEN_SETTINGS,
      &lgfx::fonts::FreeSans9pt7b
  );

  uiManager.createTextElement(
      "reflow_temp_value",
      10, 165,
      TFT_YELLOW,
      String(reflowTemp) + " C",
      SCREEN_SETTINGS,
      &lgfx::fonts::FreeSans12pt7b
  );
}