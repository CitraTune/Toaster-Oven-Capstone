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
        // Updated offsets to move graph up and right
        const int yOffset = -8;   // Shift everything UP by 8 pixels
        const int xOffset = 0;   // Shift RIGHT by 12 pixels
        const int graphX = 40 + xOffset;  // X position with offset
        const int graphY = 55 + yOffset;  // Y position with offset

        const int squareSize = 15;  // Size of each square
        const int graphWidth = squareSize * 12 + 1;   // Width for 12 columns
        const int graphHeight = squareSize * 10 + 1;  // Height for 10 rows
        const int graphBottom = graphY + graphHeight-1;  // Y-coordinate of the bottom of the graph
        const int graphTop = graphY;      // Y-coordinate of the top of the graph

        // Original X position for Y-axis labels with slight adjustment
        const int labelX = 10;  // Adjusted slightly

        // COMMENTED OUT: Remove the "Temp(Celsius)" vertical label
        /*
        UIManager::createRotatedTextElement(
            "temp_axis_label",
            labelX - 5,                     // Position left of the Y axis
            graphY + graphHeight/2 - 40,    // Center vertically along the Y axis
                TFT_WHITE,
            "Temp(Celsius)",
                SCREEN_MAIN,
            &lgfx::fonts::Font2,
            false,                          // Don't allow font changes
            90,                             // 90 degree rotation for vertical text
            4                               // middle_center datum value
        );
        */

        // Add graph labels (Y-axis temperature values) - with updated positions
        const int labelCount = 6;  // 0, 50, 100, 150, 200, 250
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

            // Updated Y position to align with the middle of the grid lines
        UIManager::createTextElement(
                labelKey,
                labelX, y,  // Adjusted to align with grid lines
                TFT_WHITE,
                tempStr,
                SCREEN_MAIN,
            &lgfx::fonts::Font2);
        }

        // Now add X-axis time labels (simplified to just 1-6)
        const int timeLabelsCount = 6;

        // Add the time labels (1-6) with improved positioning
        for (int i = 1; i <= timeLabelsCount; i++) {
            // Calculate time value (1 to 6)
            String timeStr = String(i);

            // Calculate X position: evenly distribute across the graph width
            // Adjust to ensure they're properly positioned at grid lines
            int x = graphX + ((i * 2) * squareSize);  // Position at every 2nd grid line
            // Create a unique key for each time label
            std::string labelKey = "time_label_" + std::to_string(i-1);

            // Position the label below the graph with better visibility
        UIManager::createTextElement(
                labelKey,
                x - 3,                     // Slight offset to center
                graphY + graphHeight + 8,  // Position clearly below the graph
                TFT_WHITE,
                timeStr,
                SCREEN_MAIN,
            &lgfx::fonts::Font2);
        }

        // Changed to "Time (minutes)" and repositioned to be centered under X-axis
        UIManager::createTextElement(
            "time_unit_label",
            graphX + graphWidth/2 - 35,     // Center under the X-axis
            graphY + graphHeight + 25,      // Position further below the graph
            TFT_WHITE,
            "Time (minutes)",
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
