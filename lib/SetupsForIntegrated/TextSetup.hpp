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
        setupTemperatureTableLabels();
    }

    static void updateTemperatureDisplays(const String &tempStr)
    {
        UIManager::updateTextElementContent("temp_display_settings", tempStr);
        UIManager::updateTextElementContent("temp_display_main", tempStr);
        UIManager::updateTextElementContent("temp_display_fonts", tempStr);
    }

private:
    static void setupGraphLabels()
    {
        const int yOffset = 18;
        const int xOffset = -4;
        const int graphX = 40 + xOffset;
        const int graphY = 55 + yOffset;

        const int squareSize = 15;
        const int graphWidth = squareSize * 12 + 1;
        const int graphHeight = squareSize * 10 + 1;
        const int graphBottom = graphY + graphHeight-1;
        const int graphTop = graphY;

        const int labelX = 5;
        UIManager::createTextElement(
            "temp_unit_label",
            labelX,
            graphTop - 22,
                TFT_WHITE,
            "Temperature (C)",
                SCREEN_MAIN,
            &lgfx::fonts::Font2);

        const int labelCount = 6;
        for (int i = 0; i < labelCount; i++) {
            int temp = i * 50;
            if (temp == 0) continue;

            int y = graphBottom - (i * (graphHeight) / (labelCount - 1));

            String tempStr;
            if (temp < 10) {
                tempStr = "  " + String(temp);
            } else if (temp < 100) {
                tempStr = " " + String(temp);
            } else {
                tempStr = String(temp);
        }

            std::string labelKey = "graph_label_" + std::to_string(i);
        UIManager::createTextElement(
                labelKey,
                labelX, y - 7,
                TFT_WHITE,
                tempStr,
                SCREEN_MAIN,
            &lgfx::fonts::Font2);
        }

        const int timeLabelsCount = 6;

        for (int i = 1; i <= timeLabelsCount; i++) {
            String timeStr = String(i);
            int x = graphX + (i * graphWidth / timeLabelsCount);
            std::string labelKey = "time_label_" + std::to_string(i-1);

        UIManager::createTextElement(
                labelKey,
                x - 3,
                graphY + graphHeight + 6,
                TFT_WHITE,
                timeStr,
            SCREEN_MAIN,
            &lgfx::fonts::Font2);
    }

        int timeUnitX = graphX + (graphWidth / 2) - 30;
        int timeUnitY = graphY + graphHeight + 20;
        UIManager::createTextElement(
            "time_unit_label",
            timeUnitX,
            timeUnitY,
            TFT_WHITE,
            "Time (minutes)",
            SCREEN_MAIN,
            &lgfx::fonts::Font2);
    }

    static void setupTemperatureAdjustElements()
    {
        UIManager::createTextElement(
            "soak_temp_label",
            10, 75,  // Changed from 50 to 75
            TFT_WHITE,
            "Soak Temp:",
            SCREEN_SETTINGS,
            "FreeSans", true);
        UIManager::createTextElement(
            "soak_temp_value",
            10, 100,  // Changed from 75 to 100
            TFT_YELLOW,
            String(UIManager::soakTemp) + " C",
            SCREEN_SETTINGS,
            "FreeSans", false);
        UIManager::createTextElement(
            "reflow_temp_label",
            10, 165,  // Changed from 140 to 165
            TFT_WHITE,
            "Reflow Temp:",
            SCREEN_SETTINGS,
            "FreeSans", true);
        UIManager::createTextElement(
            "reflow_temp_value",
            10, 190,  // Changed from 165 to 190
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
            8, 24,
            TFT_WHITE,
            "Main Menu",
            SCREEN_MAIN,
            "FreeSans", false);

        UIManager::createTextElement(
            "fonts_screen_label",
            8, 24,
            TFT_WHITE,
            "Font Selector",
            SCREEN_FONTS,
            "FreeSans", false);

        UIManager::createTextElement(
            "settings_screen_label",
            8, 24,
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
            TFT_RED,
            "0C",
            SCREEN_SETTINGS,
            &lgfx::fonts::Font2);
        UIManager::createTextElement(
            "temp_display_main",
            SCREEN_WIDTH - 60, 2,
            TFT_RED,
            "0C",
            SCREEN_MAIN,
            &lgfx::fonts::Font2);

        UIManager::createTextElement(
            "temp_display_fonts",
            SCREEN_WIDTH - 60, 2,
            TFT_RED,
            "0C",
            SCREEN_FONTS,
            &lgfx::fonts::Font2);
    }

    static void setupFontDisplayElements()
    {
        const char *currentFontName = IntegratedFontReflowGUI::fontNames[IntegratedFontReflowGUI::currentFontIndex];
        String fontCounterText = String(IntegratedFontReflowGUI::currentFontIndex + 1) + "/" + String(IntegratedFontReflowGUI::fontCount);

        UIManager::createTextElement(
            "button_font_label",
            10,
            SCREEN_HEIGHT / 2 - 100,
            TFT_WHITE,
            "Button font:",
            SCREEN_FONTS,
            "FreeSans",
            true);
        UIManager::createTextElement(
            "button_font_display",
            20,
            SCREEN_HEIGHT / 2 - 80,
            TFT_WHITE,
            currentFontName,
            SCREEN_FONTS,
            "FreeSans",
            true);

        UIManager::createTextElement(
            "global_font_label",
            10,
            SCREEN_HEIGHT / 2 - 60,
            TFT_WHITE,
            "Global font:",
            SCREEN_FONTS,
            "FreeSans",
            true);

        UIManager::createTextElement(
            "current_font_display",
            20,
            SCREEN_HEIGHT / 2 - 40,
            TFT_WHITE,
            currentFontName,
            SCREEN_FONTS,
            "FreeSans",
            true);

        UIManager::createTextElement(
            "font_counter_display",
            10,
            SCREEN_HEIGHT / 2 - 20,
            TFT_WHITE,
            fontCounterText,
            SCREEN_FONTS,
            "FreeSans",
            true);
    }

    static void setupCooldownScreenElements()
    {
        UIManager::createTextElement(
            "cooldown_instruction",
            SCREEN_WIDTH / 2 - 140, SCREEN_HEIGHT / 2 - 30,
            TFT_BLACK,
            "Hold Door Partially Open For:",
            SCREEN_COOLDOWN,
            "FreeSansBold", false);

        UIManager::createTextElement(
            "cooldown_timer",
            SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 30,
            TFT_BLACK,
            "30s",
            SCREEN_COOLDOWN,
            "FreeSansBold", false);
    }

    static void setupTemperatureTableLabels()
    {
        // Table positioning (matching the button layout in ButtonSetup)
        const int tableX = 120;
        const int tableY = 55;
        const int cellWidth = 50;
        const int cellHeight = 40;

        // Add column headers - using Font2 (9pt) and shifted left by 15px
        UIManager::createTextElement(
            "header_10",
            tableX - 5, tableY - 20,  // Shifted left by 15px (from tableX + 15 to tableX - 15)
            TFT_WHITE,
            "+/- 10",
            SCREEN_SETTINGS,
            &lgfx::fonts::Font2  // Changed to 9pt font
        );

        UIManager::createTextElement(
            "header_1",
            tableX + cellWidth - 5, tableY - 20,  // Shifted left by 15px (from tableX + cellWidth + 15 to tableX + cellWidth - 15)
            TFT_WHITE,
            "+/- 1",
            SCREEN_SETTINGS,
            &lgfx::fonts::Font2  // Changed to 9pt font
        );
    }
};

