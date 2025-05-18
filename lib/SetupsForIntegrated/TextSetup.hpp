#pragma once

#include "IntegratedFontReflowGUI.hpp"
#include "UIManager.hpp"

class TextSetup
{
public:
  // Set up temperature text elements in the settings screen
  static void setupTemperatureElements()
  {
    // Soak Temperature Label
    UIManager::createTextElement(
        "soak_temp_label",
        10, 50,
        TFT_WHITE,
        "Soak Temp:",
        SCREEN_SETTINGS,
        &lgfx::fonts::FreeSans9pt7b);

    // Soak Temperature Value
    UIManager::createTextElement(
        "soak_temp_value",
        10, 75,
        TFT_YELLOW,
        String(UIManager::soakTemp) + " C",
        SCREEN_SETTINGS,
        &lgfx::fonts::FreeSans12pt7b);

    // Reflow Temperature Label
    UIManager::createTextElement(
        "reflow_temp_label",
        10, 140,
        TFT_WHITE,
        "Reflow Temp:",
        SCREEN_SETTINGS,
        &lgfx::fonts::FreeSans9pt7b);

    // Reflow Temperature Value
    UIManager::createTextElement(
        "reflow_temp_value",
        10, 165,
        TFT_YELLOW,
        String(UIManager::reflowTemp) + " C",
        SCREEN_SETTINGS,
        &lgfx::fonts::FreeSans12pt7b);
  }
};
