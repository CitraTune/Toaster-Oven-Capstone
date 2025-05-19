#pragma once

#include "IntegratedFontReflowGUI.hpp"
#include "UIManager.hpp"

class TextSetup
{
public:
    static void setupAllTextElements()
    {
        setupTemperatureSettingsElements();
        setupTitleElements();
        setupScreenLabelElements();
        setupTemperatureDisplayElements();
    }
    static void updateTemperatureDisplays(const String &tempStr)
    {
        UIManager::updateTextElement("temp_display_settings", tempStr);
        UIManager::updateTextElement("temp_display_main", tempStr);
        UIManager::updateTextElement("temp_display_fonts", tempStr);
    }

private:
    // Set up temperature text elements in the settings screen
    static void setupTemperatureSettingsElements()
    {
        // Soak Temperature Label
        UIManager::createTextElement(
            "soak_temp_label",
            10, 50,
            TFT_WHITE,
            "Soak Temp:",
            SCREEN_SETTINGS,
            "FreeSans", true); // 9pt

        // Soak Temperature Value
        UIManager::createTextElement(
            "soak_temp_value",
            10, 75,
            TFT_YELLOW,
            String(UIManager::soakTemp) + " C",
            SCREEN_SETTINGS,
            "FreeSans", false); // 12pt

        // Reflow Temperature Label
        UIManager::createTextElement(
            "reflow_temp_label",
            10, 140,
            TFT_WHITE,
            "Reflow Temp:",
            SCREEN_SETTINGS,
            "FreeSans", true); // 9pt

        // Reflow Temperature Value
        UIManager::createTextElement(
            "reflow_temp_value",
            10, 165,
            TFT_YELLOW,
            String(UIManager::reflowTemp) + " C",
            SCREEN_SETTINGS,
            "FreeSans", false); // 12pt
    }
    // Set up title text elements for all screens
    static void setupTitleElements()
    {
        // Title for Settings Screen - Font2 opt-out
        UIManager::createTextElement(
            "title_settings",
            SCREEN_WIDTH / 2 - 50, 2, // Centered with virtually no top margin
            TFT_WHITE,
            "Bonfire 1.0.11",
            SCREEN_SETTINGS,
            "Font2", true, false); // opt-out of font changes

        // Title for Main Screen - Font2 opt-out
        UIManager::createTextElement(
            "title_main",
            SCREEN_WIDTH / 2 - 50, 2, // Centered with virtually no top margin
            TFT_WHITE,
            "Bonfire 1.0.11",
            SCREEN_MAIN,
            "Font2", true, false); // opt-out of font changes

        // Title for Fonts Screen - Font2 opt-out
        UIManager::createTextElement(
            "title_fonts",
            SCREEN_WIDTH / 2 - 50, 2, // Centered with virtually no top margin
            TFT_WHITE,
            "Bonfire 1.0.11",
            SCREEN_FONTS,
            "Font2", true, false); // opt-out of font changes
    }

    // Set up screen label elements
    static void setupScreenLabelElements()
    {
        // Main Menu Screen Label
        UIManager::createTextElement(
            "main_screen_label",
            8, 30, // Left side with 8px margin
            TFT_WHITE,
            "Main Menu",
            SCREEN_MAIN,
            "FreeSans", false); // 12pt

        // Font Test Selection Screen Label
        UIManager::createTextElement(
            "fonts_screen_label",
            8, 30, // Left side with 8px margin
            TFT_WHITE,
            "Font Test Selection",
            SCREEN_FONTS,
            "FreeSans", false); // 12pt

        // Settings Screen Label
        UIManager::createTextElement(
            "settings_screen_label",
            8, 30, // Left side with 8px margin
            TFT_WHITE,
            "Settings",
            SCREEN_SETTINGS,
            "FreeSans", false); // 12pt
    }

    // Set up temperature display elements
    static void setupTemperatureDisplayElements()
    {
        // Create temperature display for all screens
        // Settings Screen Temperature - Font2 opt-out
        UIManager::createTextElement(
            "temp_display_settings",
            SCREEN_WIDTH - 60, 2, // Top right, tight margin
            TFT_YELLOW,
            "0C", // Default value, will be updated
            SCREEN_SETTINGS,
            "Font2", true, false); // opt-out of font changes

        // Main Screen Temperature - Font2 opt-out
        UIManager::createTextElement(
            "temp_display_main",
            SCREEN_WIDTH - 60, 2, // Top right, tight margin
            TFT_YELLOW,
            "0C", // Default value, will be updated
            SCREEN_MAIN,
            "Font2", true, false); // opt-out of font changes

        // Fonts Screen Temperature - Font2 opt-out
        UIManager::createTextElement(
            "temp_display_fonts",
            SCREEN_WIDTH - 60, 2, // Top right, tight margin
            TFT_YELLOW,
            "0C", // Default value, will be updated
            SCREEN_FONTS,
            "Font2", true, false); // opt-out of font changes
    }
};