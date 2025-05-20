#pragma once

#include "IntegratedFontReflowGUI.hpp"
#include "UIManager.hpp"

#define pt12 false
#define pt9 true
class TextSetup
{
public:
    static void setupAllTextElements()
    {
        setupTemperatureAdjustElements();
        setupTitleElements();
        setupScreenLabelElements();
        setupTemperatureDisplayElements();
        setupFontDisplayElements();
    }

    static void updateTemperatureDisplays(const String &tempStr)
    {
        UIManager::updateTextElementContent("temp_display_settings", tempStr);
        UIManager::updateTextElementContent("temp_display_main", tempStr);
        UIManager::updateTextElementContent("temp_display_fonts", tempStr);
    }

private:
    static void setupTemperatureAdjustElements()
    {
        UIManager::createTextElement(
            "soak_temp_label",
            10, 50,
            TFT_WHITE,
            "Soak Temp:",
            SCREEN_SETTINGS,
            "FreeSans", true);

        UIManager::createTextElement(
            "soak_temp_value",
            10, 75,
            TFT_YELLOW,
            String(UIManager::soakTemp) + " C",
            SCREEN_SETTINGS,
            "FreeSans", false);

        UIManager::createTextElement(
            "reflow_temp_label",
            10, 140,
            TFT_WHITE,
            "Reflow Temp:",
            SCREEN_SETTINGS,
            "FreeSans", true);

        UIManager::createTextElement(
            "reflow_temp_value",
            10, 165,
            TFT_YELLOW,
            String(UIManager::reflowTemp) + " C",
            SCREEN_SETTINGS,
            "FreeSans", false);
    }

    static void setupTitleElements()
    {
        UIManager::createTextElement(
            "title_settings",
            SCREEN_WIDTH / 2 - 50, 2,
            TFT_WHITE,
            "Bonfire 1.0.11",
            SCREEN_SETTINGS,
            &lgfx::fonts::Font2);

        UIManager::createTextElement(
            "title_main",
            SCREEN_WIDTH / 2 - 50, 2,
            TFT_WHITE,
            "Bonfire 1.0.11",
            SCREEN_MAIN,
            &lgfx::fonts::Font2);

        UIManager::createTextElement(
            "title_fonts",
            SCREEN_WIDTH / 2 - 50, 2,
            TFT_WHITE,
            "Bonfire 1.0.11",
            SCREEN_FONTS,
            &lgfx::fonts::Font2);
    }

    static void setupScreenLabelElements()
    {
        UIManager::createTextElement(
            "main_screen_label",
            8, 30,
            TFT_WHITE,
            "Main Menu",
            SCREEN_MAIN,
            "FreeSans", false);

        UIManager::createTextElement(
            "fonts_screen_label",
            8, 30,
            TFT_WHITE,
            "Font Test Selection",
            SCREEN_FONTS,
            "FreeSans", false);

        UIManager::createTextElement(
            "settings_screen_label",
            8, 30,
            TFT_WHITE,
            "Settings",
            SCREEN_SETTINGS,
            "FreeSans", false);
    }

    static void setupTemperatureDisplayElements()
    {
        UIManager::createTextElement(
            "temp_display_settings",
            SCREEN_WIDTH - 60, 2,
            TFT_YELLOW,
            "0C",
            SCREEN_SETTINGS,
            &lgfx::fonts::Font2);

        UIManager::createTextElement(
            "temp_display_main",
            SCREEN_WIDTH - 60, 2,
            TFT_YELLOW,
            "0C",
            SCREEN_MAIN,
            &lgfx::fonts::Font2);

        UIManager::createTextElement(
            "temp_display_fonts",
            SCREEN_WIDTH - 60, 2,
            TFT_YELLOW,
            "0C",
            SCREEN_FONTS,
            &lgfx::fonts::Font2);
    }

    static void setupFontDisplayElements()
    {
        // Get the current font name from the array
        const char *currentFontName = IntegratedFontReflowGUI::fontNames[IntegratedFontReflowGUI::currentFontIndex];

        // Create a string for the font counter display (showing which font out of total)
        String fontCounterText = String(IntegratedFontReflowGUI::currentFontIndex + 1) + "/" + String(IntegratedFontReflowGUI::fontCount);
    // Create the label in the middle of the screen to display the current font name
    UIManager::createTextElement(
      "current_font_display",
      SCREEN_WIDTH / 2 - 70,  // x - centered (adjusted for text width)
      SCREEN_HEIGHT / 2 - 20, // y - middle of screen, slightly above center
      TFT_WHITE,             // color
      currentFontName,       // text content (current font name)
      SCREEN_FONTS,          // screen ID (font selection screen)
      "FreeSans",            // using FreeSans to display the name consistently
      true);                 // using 9pt size
    
    // Create the font counter label below the font name
    UIManager::createTextElement(
      "font_counter_display",
      SCREEN_WIDTH / 2 - 20,  // x - centered (adjusted for counter width)
      SCREEN_HEIGHT / 2 + 20, // y - below the font name
      TFT_YELLOW,            // color
      fontCounterText,       // text content (font counter)
      SCREEN_FONTS,          // screen ID
      "FreeSans",            // using FreeSans
      true);                 // using 9pt size
    }
};
