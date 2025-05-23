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
        // Calculate positions based on the graph position and height


        const int yOffset = 8;   // Shift everything down by 8 pixels
        const int xOffset = -4;  // Shift graph left by 4 pixels
        const int graphX = 40 + xOffset;  // X position with offset
        const int graphY = 55 + yOffset;  // Y position with offset

        const int squareSize = 15;  // Size of each square
        const int graphWidth = squareSize * 12 + 1;   // Width for 12 columns
        const int graphHeight = squareSize * 10 + 1;  // Height for 10 rows
        const int graphBottom = graphY + graphHeight-1;  // Y-coordinate of the bottom of the graph
        const int graphTop = graphY;      // Y-coordinate of the top of the graph


        // Original X position for Y-axis labels (don't shift these)
        const int labelX = 5;  // This stays the same (no xOffset)

        // Add a "C" label at the top of the Y-axis - keep at original position
        UIManager::createTextElement(
            "temp_unit_label",


            labelX,           // X position (no offset - stays at original position)
            graphTop - 15,    // Position above the graph
                TFT_WHITE,
            "C",
                SCREEN_MAIN,
            &lgfx::fonts::Font2);

        // Labels will be evenly distributed between top and bottom
        const int labelCount = 6;  // 0, 50, 100, 150, 200, 250


        // Add graph labels (Y-axis temperature values) - keep at original position
        for (int i = 0; i < labelCount; i++) {
            int temp = i * 50;  // Temperature value (0, 50, 100, 150, 200, 250)

            // Skip label for 0
            if (temp == 0) continue;

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

                labelX, y - 7,  // X position remains at original position (no offset)
                TFT_WHITE,
                tempStr,
                SCREEN_MAIN,
            &lgfx::fonts::Font2);
        }

        // Now add X-axis time labels (simplified to just 1-6)
        const int timeLabelsCount = 6;

        // Add the time labels (1-6)
        for (int i = 1; i <= timeLabelsCount; i++) {
            // Calculate time value (1 to 6)
            String timeStr = String(i);

            // Calculate X position: evenly distribute across the graph width
            int x = graphX + (i * graphWidth / timeLabelsCount);

            // Create a unique key for each time label
            std::string labelKey = "time_label_" + std::to_string(i-1);

            // Position the label below the graph
        UIManager::createTextElement(
                labelKey,
                x - 3,                     // Offset to center the text with the tick
                graphY + graphHeight + 6,  // Position below the graph
                TFT_WHITE,
                timeStr,
                SCREEN_MAIN,
            &lgfx::fonts::Font2);
        }

        // Add "min" label to the right of "6"
        UIManager::createTextElement(
            "time_unit_label",
            graphX + graphWidth + 5,     // Position to the right of the graph
            graphY + graphHeight + 6,    // Position below the graph
            TFT_WHITE,
            "min",
            SCREEN_MAIN,
            &lgfx::fonts::Font2);
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
