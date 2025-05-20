#pragma once

#include "UIManager.hpp"
#include "IntegratedFontReflowGUI.hpp"


// Constants for button dimensions
#define BUTTON_HEIGHT 40
#define BUTTON_MARGIN 10
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

class ButtonSetup
{
public:
  // Setup all buttons for the application
  static void setupAllButtons()
  {
    setupFontScreenButtons();
    setupMainScreenButtons();
    setupSettingsScreenButtons();
  }

private:
  static constexpr int buttonMargin = BUTTON_MARGIN;
  static constexpr int buttonHeight = BUTTON_HEIGHT;

  static void setupMainScreenButtons()
  {
    const int buttonWidth = (SCREEN_WIDTH - (3 * buttonMargin)) / 2;
    const int leftButtonX = buttonMargin;
    const int rightButtonX = leftButtonX + buttonWidth + buttonMargin;

    UIManager::createButton(
        "font_test_btn",
        leftButtonX,
        SCREEN_HEIGHT - buttonHeight - buttonMargin,
        buttonWidth,
        buttonHeight,
        10,
        "Font Test",
        SCREEN_MAIN,
        goToFonts);

    UIManager::createButton(
        "settings_btn",
        rightButtonX,
        SCREEN_HEIGHT - buttonHeight - buttonMargin,
        buttonWidth,
        buttonHeight,
        10,
        "Settings",
        SCREEN_MAIN,
        goToSettings);
  }

  static void setupSettingsScreenButtons()
  {
    const int backButtonWidth = 160;
    const int backButtonX = (SCREEN_WIDTH - backButtonWidth) / 2;

    UIManager::createButton(
        "back_from_settings_btn",
        backButtonX,
        SCREEN_HEIGHT - buttonHeight - buttonMargin,
        backButtonWidth,
        buttonHeight,
        10,
        "Back",
        SCREEN_SETTINGS,
        goToMain);

    int tempBoxX = 130;
    int tempBoxY = 10;

    // // Soak Temp Controls
    // UIManager::createButton("soak_temp_plus10", tempBoxX, tempBoxY, 40, 40, 5, "+10",
    //                         SCREEN_SETTINGS, increaseSoakTempCoarse);
    // UIManager::createButton("soak_temp_minus10", tempBoxX, tempBoxY + 50, 40, 40, 5, "-10",
    //                         SCREEN_SETTINGS, decreaseSoakTempCoarse);
    // UIManager::createButton("soak_temp_plus1", tempBoxX + 60, tempBoxY, 40, 40, 5, "+1",
    //                         SCREEN_SETTINGS, increaseSoakTempFine);
    // UIManager::createButton("soak_temp_minus1", tempBoxX + 60, tempBoxY + 50, 40, 40, 5, "-1",
    //                         SCREEN_SETTINGS, decreaseSoakTempFine);

    // // Reflow Temp Controls
    // tempBoxY += 100;
    // UIManager::createButton("reflow_temp_plus10", tempBoxX, tempBoxY, 40, 40, 5, "+10",
    //                         SCREEN_SETTINGS, increaseReflowTempCoarse);
    // UIManager::createButton("reflow_temp_minus10", tempBoxX, tempBoxY + 50, 40, 40, 5, "-10",
    //                         SCREEN_SETTINGS, decreaseReflowTempCoarse);
    // UIManager::createButton("reflow_temp_plus1", tempBoxX + 60, tempBoxY, 40, 40, 5, "+1",
    //                         SCREEN_SETTINGS, increaseReflowTempFine);
    // UIManager::createButton("reflow_temp_minus1", tempBoxX + 60, tempBoxY + 50, 40, 40, 5, "-1",
    //                         SCREEN_SETTINGS, decreaseReflowTempFine);

    UIManager::createButton("invert_accent_btn", 3, SCREEN_HEIGHT - 100, 112, 40, 10,
                            "Invert Accent", SCREEN_SETTINGS, UIManager::toggleInvertAccent);

    UIManager::createButton("light_mode_btn", (SCREEN_WIDTH - 112) - 3, SCREEN_HEIGHT - 100, 112, 40, 10,
                            "Light Mode", SCREEN_SETTINGS, UIManager::toggleLightMode);
  }

  static void setupFontScreenButtons()
  {
    const int backButtonWidth = 160;
    const int backButtonX = (SCREEN_WIDTH - backButtonWidth) / 2;

    UIManager::createButton(
        "back_from_fonts_btn",
        backButtonX,
        SCREEN_HEIGHT - buttonHeight - buttonMargin,
        backButtonWidth,
        buttonHeight,
        10,
        "Back",
        SCREEN_FONTS,
        goToMain);

    UIManager::createButton(
        "prev_font_btn",
        10,
        SCREEN_HEIGHT - buttonHeight - 60,
        60,
        buttonHeight,
        10,
        "<",
        SCREEN_FONTS,
        prevFont);

    UIManager::createButton(
        "next_font_btn",
        SCREEN_WIDTH - 70,
        SCREEN_HEIGHT - buttonHeight - 60,
        60,
        buttonHeight,
        10,
        ">",
        SCREEN_FONTS,
        nextFont);
  }

  static void goToMain()
  {
    UIManager::navigateToScreen(SCREEN_MAIN);
    IntegratedFontReflowGUI::updateFontDisplay();
  }

  static void goToFonts()
  {
    UIManager::navigateToScreen(SCREEN_FONTS);
    IntegratedFontReflowGUI::updateFontDisplay();
  }

  static void goToSettings()
  {
    UIManager::navigateToScreen(SCREEN_SETTINGS);
    IntegratedFontReflowGUI::updateFontDisplay();
  }

  static void nextFont()
  {
    IntegratedFontReflowGUI::currentFontIndex =
        (IntegratedFontReflowGUI::currentFontIndex + 1) % IntegratedFontReflowGUI::fontCount;
    IntegratedFontReflowGUI::updateFontDisplay();
  }

  static void prevFont()
  {
    IntegratedFontReflowGUI::currentFontIndex =
        (IntegratedFontReflowGUI::currentFontIndex - 1 + IntegratedFontReflowGUI::fontCount) % IntegratedFontReflowGUI::fontCount;
    IntegratedFontReflowGUI::updateFontDisplay();
  }

  static void increaseSoakTempCoarse()
  {
    IntegratedFontReflowGUI::soakTemp += 10;
    IntegratedFontReflowGUI::updateFontDisplay();
  }

  static void decreaseSoakTempCoarse()
  {
    IntegratedFontReflowGUI::soakTemp -= 10;
    IntegratedFontReflowGUI::updateFontDisplay();
  }

  static void increaseSoakTempFine()
  {
    IntegratedFontReflowGUI::soakTemp += 1;
    IntegratedFontReflowGUI::updateFontDisplay();
  }

  static void decreaseSoakTempFine()
  {
    IntegratedFontReflowGUI::soakTemp -= 1;
    IntegratedFontReflowGUI::updateFontDisplay();
  }

  static void increaseReflowTempCoarse()
  {
    IntegratedFontReflowGUI::reflowTemp += 10;
    IntegratedFontReflowGUI::updateFontDisplay();
  }

  static void decreaseReflowTempCoarse()
  {
    IntegratedFontReflowGUI::reflowTemp -= 10;
    IntegratedFontReflowGUI::updateFontDisplay();
  }

  static void increaseReflowTempFine()
  {
    IntegratedFontReflowGUI::reflowTemp += 1;
    IntegratedFontReflowGUI::updateFontDisplay();
  }

  static void decreaseReflowTempFine()
  {
    IntegratedFontReflowGUI::reflowTemp -= 1;
    IntegratedFontReflowGUI::updateFontDisplay();
  }
};
