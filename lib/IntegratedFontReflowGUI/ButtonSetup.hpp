#pragma once

#include "UIManager.hpp"
#include "IntegratedFontReflowGUI.hpp"
#include "GraphManager.hpp"

// Constants for button dimensions
#define BUTTON_HEIGHT 40
#define BUTTON_MARGIN 10
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

// Forward declaration
class IntegratedFontReflowGUI;

class ButtonSetup {
private:
    UIManager& uiManager;
    int buttonHeight;

public:
    ButtonSetup(UIManager& manager) :
        uiManager(manager),
        buttonHeight(BUTTON_HEIGHT) {}

    // Setup all buttons for the application
    void setupAllButtons() {
        setupMainScreenButtons();
        setupSettingsScreenButtons();
        setupFontScreenButtons();
    }
    void goToMain() {
  UIManager::navigateToScreen(SCREEN_MAIN);
  if (GraphManager::isVisibleOnScreen(SCREEN_MAIN)) {
    GraphManager::draw();
  }
}

void goToFonts() {
  UIManager::navigateToScreen(SCREEN_FONTS);
  IntegratedFontReflowGUI::updateFontDisplay();
}

// Theme toggle functions
void toggleLightMode() {
  UIManager::toggleLightMode();
  IntegratedFontReflowGUI::redrawCurrentScreen();
}

void toggleInvertAccent() {
  UIManager::toggleInvertAccent();
  IntegratedFontReflowGUI::redrawCurrentScreen();
}

// Font navigation functions
void nextFont() {
  IntegratedFontReflowGUI::currentFontIndex = (IntegratedFontReflowGUI::currentFontIndex + 1) % IntegratedFontReflowGUI::fontCount;
  IntegratedFontReflowGUI::updateFontDisplay();
}

void prevFont() {
  IntegratedFontReflowGUI::currentFontIndex = (IntegratedFontReflowGUI::currentFontIndex - 1 + IntegratedFontReflowGUI::fontCount) % IntegratedFontReflowGUI::fontCount;
  IntegratedFontReflowGUI::updateFontDisplay();
}

private:
    // Setup main screen buttons
    void setupMainScreenButtons() {
        const int buttonMargin = BUTTON_MARGIN;
        const int buttonWidth = (SCREEN_WIDTH - (3 * buttonMargin)) / 2;
        const int leftButtonX = buttonMargin;
        const int rightButtonX = leftButtonX + buttonWidth + buttonMargin;

        // Font Test button on the left
        UIManager::createButton(
            "font_test_btn",
            leftButtonX,
            SCREEN_HEIGHT - buttonHeight - buttonMargin,
            buttonWidth,
            buttonHeight,
            10,
            "Font Test",
            SCREEN_MAIN,
            IntegratedFontReflowGUI::goToFonts
        );

        // Settings button on the right
        UIManager::createButton(
            "settings_btn",
            rightButtonX,
            SCREEN_HEIGHT - buttonHeight - buttonMargin,
            buttonWidth,
            buttonHeight,
            10,
            "Settings",
            SCREEN_MAIN,
            UIManager::goToSettings()
        );
    }

    // Setup settings screen buttons
    void setupSettingsScreenButtons() {
        const int buttonMargin = BUTTON_MARGIN;
        const int backButtonWidth = 160;
        const int backButtonX = (SCREEN_WIDTH - backButtonWidth) / 2;

        // Back button
        UIManager::createButton(
            "back_from_settings_btn",
            backButtonX,
            SCREEN_HEIGHT - buttonHeight - buttonMargin,
            backButtonWidth,
            buttonHeight,
            10,
            "Back",
            SCREEN_SETTINGS,
            UIManager::goToMain
        );

        int tempBoxX = 130;
        int tempBoxY = 10;

        // Soak Temp Controls
        UIManager::createButton("soak_temp_plus10", tempBoxX, tempBoxY, 40, 40, 5, "+10",
                                SCREEN_SETTINGS, UIManager::increaseSoakTempCoarse);
        UIManager::createButton("soak_temp_minus10", tempBoxX, tempBoxY + 50, 40, 40, 5, "-10",
                                SCREEN_SETTINGS, UIManager::decreaseSoakTempCoarse);
        UIManager::createButton("soak_temp_plus1", tempBoxX + 60, tempBoxY, 40, 40, 5, "+1",
                                SCREEN_SETTINGS, UIManager::increaseSoakTempFine);
        UIManager::createButton("soak_temp_minus1", tempBoxX + 60, tempBoxY + 50, 40, 40, 5, "-1",
                                SCREEN_SETTINGS, UIManager::decreaseSoakTempFine);

        // Reflow Temp Controls
        tempBoxY += 100;
        UIManager::createButton("reflow_temp_plus10", tempBoxX, tempBoxY, 40, 40, 5, "+10",
                                SCREEN_SETTINGS, UIManager::increaseReflowTempCoarse);
        UIManager::createButton("reflow_temp_minus10", tempBoxX, tempBoxY + 50, 40, 40, 5, "-10",
                                SCREEN_SETTINGS, UIManager::decreaseReflowTempCoarse);
        UIManager::createButton("reflow_temp_plus1", tempBoxX + 60, tempBoxY, 40, 40, 5, "+1",
                                SCREEN_SETTINGS, UIManager::increaseReflowTempFine);
        UIManager::createButton("reflow_temp_minus1", tempBoxX + 60, tempBoxY + 50, 40, 40, 5, "-1",
                                SCREEN_SETTINGS, UIManager::decreaseReflowTempFine);

        // Accent Invert Button
        UIManager::createButton("invert_accent_btn", 3, SCREEN_HEIGHT - 100, 112, 40, 10,
                                "Invert Accent", SCREEN_SETTINGS, UIManager::toggleInvertAccent);

        // Light Mode Toggle Button
        UIManager::createButton("light_mode_btn", (SCREEN_WIDTH - 112) - 3, SCREEN_HEIGHT - 100, 112, 40, 10,
                                "Light Mode", SCREEN_SETTINGS, UIManager::toggleLightMode);
    }

    // Setup font screen buttons
    void setupFontScreenButtons() {
        const int buttonMargin = BUTTON_MARGIN;
        const int backButtonWidth = 160;
        const int backButtonX = (SCREEN_WIDTH - backButtonWidth) / 2;

        // Font test screen back button
        UIManager::createButton(
            "back_from_fonts_btn",
            backButtonX,
            SCREEN_HEIGHT - buttonHeight - buttonMargin,
            backButtonWidth,
            buttonHeight,
            10,
            "Back",
            SCREEN_FONTS,
            UIManager::goToMain
        );

        // Font navigation buttons (left/right)
        UIManager::createButton(
            "prev_font_btn",
            10,
            SCREEN_HEIGHT - buttonHeight - 60,
            60,
            buttonHeight,
            10,
            "<",
            SCREEN_FONTS,
            UIManager::prevFont
        );

        UIManager::createButton(
            "next_font_btn",
            SCREEN_WIDTH - 70,
            SCREEN_HEIGHT - buttonHeight - 60,
            60,
            buttonHeight,
            10,
            ">",
            SCREEN_FONTS,
            UIManager::nextFont();
        );
    }
};

