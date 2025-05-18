#pragma once

#include "IntegratedFontReflowGUI.hpp"
#include "UIManager.hpp"

// Setup temperature display elements in settings screen
inline void IntegratedFontReflowGUI::setupTemperatureElements() {
  // Create temperature labels and values
  UIManager::createTextElement(
      "soak_temp_label",
      10, 50,
      TFT_WHITE,
      "Soak Temp:",
      SCREEN_SETTINGS,
      &lgfx::fonts::FreeSans9pt7b
  );

  UIManager::createTextElement(
      "soak_temp_value",
      10, 75,
      TFT_YELLOW,
      String(UIManager::soakTemp) + " C",  // Access static soakTemp variable
      SCREEN_SETTINGS,
      &lgfx::fonts::FreeSans12pt7b
  );

  UIManager::createTextElement(
      "reflow_temp_label",
      10, 140,
      TFT_WHITE,
      "Reflow Temp:",
      SCREEN_SETTINGS,
      &lgfx::fonts::FreeSans9pt7b
  );

  UIManager::createTextElement(
      "reflow_temp_value",
      10, 165,
      TFT_YELLOW,
      String(UIManager::reflowTemp) + " C",  // Access static reflowTemp variable
      SCREEN_SETTINGS,
      &lgfx::fonts::FreeSans12pt7b
  );
}

