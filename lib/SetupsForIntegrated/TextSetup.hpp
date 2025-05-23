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
        setupBonfireTitleElements();

        setupTemperatureAdjustElements();

        setupScreenLabelElements();

        setupTemperatureDisplayElements();

        setupFontDisplayElements();

        setupCooldownScreenElements();
        
        setupGraphLabels();
    }

    static void updateTemperatureDisplays(const String &tempStr)
    {
        UIManager::updateTextElementContent("temp_display_settings", tempStr);
        UIManager::updateTextElementContent("temp_display_main", tempStr);
        UIManager::updateTextElementContent("temp_display_fonts", tempStr);
    }

private:
    // Add the new separate function for graph labels
    static void setupGraphLabels()
    {
        // Calculate new Y positions based on the graph position and height
        const int graphY = 55;  // Graph's Y position (unchanged)
        const int graphHeight = 16 * 10 + 1;  // Graph height
        const int graphBottom = graphY + graphHeight-1;  // Y-coordinate of the bottom of the graph
        const int graphTop = graphY;  // Y-coordinate of the top of the graph

        // We want to align 250 with the top and 0 with the bottom
        // Labels will be evenly distributed between top and bottom
        const int labelCount = 6;  // 0, 50, 100, 150, 200, 250

        // Add graph labels (Y-axis temperature values)
        for (int i = 0; i < labelCount; i++) {
            int temp = i * 50;  // Temperature value (0, 50, 100, 150, 200, 250)

            // Calculate Y position: map i from [0,5] to [graphBottom,graphTop]
            int y = graphBottom - (i * (graphHeight) / (labelCount - 1));

            // Convert the temperature to a right-aligned string with spaces
            String tempStr;
            if (temp < 10) {
                tempStr = "  " + String(temp);  // 2 spaces for single digit
            } else if (temp < 100) {
                tempStr = " " + String(temp);   // 1 space for double digit
            } else {
                tempStr = String(temp);         // No space needed for 3 digits
            }

            // Convert i to string and create the key properly
            std::string labelKey = "graph_label_" + std::to_string(i);
            UIManager::createTextElement(
                labelKey,
                5, y - 7,  // -7 to center the text vertically with the line
                TFT_WHITE,
                tempStr,
                SCREEN_MAIN,
                &lgfx::fonts::Font2);
        }
    }

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

    static void setupBonfireTitleElements()
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
            8, 24, // y position moved up 6 pixels (from 30 to 24)
            TFT_WHITE,
            "Main Menu",
            SCREEN_MAIN,
            "FreeSans", false);

        UIManager::createTextElement(
            "fonts_screen_label",
            8, 24, // y position moved up 6 pixels (from 30 to 24)
            TFT_WHITE,
            "Font Selector",
            SCREEN_FONTS,
            "FreeSans", false);

        UIManager::createTextElement(
            "settings_screen_label",
            8, 24, // y position moved up 6 pixels (from 30 to 24)
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
            TFT_RED, // Changed from TFT_YELLOW to TFT_RED
            "0C",
            SCREEN_SETTINGS,
            &lgfx::fonts::Font2);
        UIManager::createTextElement(
            "temp_display_main",
            SCREEN_WIDTH - 60, 2,
            TFT_RED, // Changed from TFT_YELLOW to TFT_RED
            "0C",
            SCREEN_MAIN,
            &lgfx::fonts::Font2);

        UIManager::createTextElement(
            "temp_display_fonts",
            SCREEN_WIDTH - 60, 2,
            TFT_RED, // Changed from TFT_YELLOW to TFT_RED
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

        // Add a new label to show the current button font - MOVED UP MORE AND LEFT-ALIGNED
        UIManager::createTextElement(
            "button_font_label",
            10,                      // x - left-aligned at 10px from left edge
            SCREEN_HEIGHT / 2 - 100, // y position unchanged from previous modification
            TFT_WHITE,
            "Button font:",
            SCREEN_FONTS,
            "FreeSans",
            true);
        UIManager::createTextElement(
            "button_font_display",
            20,                     // x - left-aligned with slight indent (10px)
            SCREEN_HEIGHT / 2 - 80, // y position unchanged
            TFT_WHITE,
            currentFontName,
            SCREEN_FONTS,
            "FreeSans",
            true);

        // Change the regular font label - MOVED LEFT
        UIManager::createTextElement(
            "global_font_label",
            10,                     // x - left-aligned at 10px from left edge
            SCREEN_HEIGHT / 2 - 60, // y position unchanged
            TFT_WHITE,
            "Global font:",
            SCREEN_FONTS,
            "FreeSans",
            true);

        // Current font name display - MOVED LEFT
        UIManager::createTextElement(
            "current_font_display",
            20,                     // x - left-aligned with slight indent (10px)
            SCREEN_HEIGHT / 2 - 40, // y position unchanged
            TFT_WHITE,
            currentFontName,
            SCREEN_FONTS,
            "FreeSans",
            true);

        // Font counter label - MOVED LEFT
        UIManager::createTextElement(
            "font_counter_display",
            10,                     // x - left-aligned at 10px from left edge
            SCREEN_HEIGHT / 2 - 20, // y position unchanged
            TFT_WHITE,
            fontCounterText,
            SCREEN_FONTS,
            "FreeSans",
            true);
    }

    static void setupCooldownScreenElements()
    {
        // Instruction text
        UIManager::createTextElement(
            "cooldown_instruction",
            SCREEN_WIDTH / 2 - 140, SCREEN_HEIGHT / 2 - 30,
            TFT_BLACK,
            "Hold Door Partially Open For:",
            SCREEN_COOLDOWN,
            "FreeSansBold", false);

        // Timer text - will be updated dynamically
        UIManager::createTextElement(
            "cooldown_timer",
            SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 30,
            TFT_BLACK,
            "30s",
            SCREEN_COOLDOWN,
            "FreeSansBold", false);
    }
};

