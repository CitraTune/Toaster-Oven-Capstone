#pragma once

#include "UIManager.hpp"
#include "IntegratedFontReflowGUI.hpp"
#include "TempManager.hpp"
#include "ReflowController.hpp"

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

    // Rename to just "Reflow" instead of "Begin Reflow"
    UIManager::createButton(
        "reflow_btn",
        leftButtonX,
        SCREEN_HEIGHT - buttonHeight - buttonMargin + 3, // Moved down by 3 pixels
        buttonWidth,
        buttonHeight,
        10,
        "Reflow",
        SCREEN_MAIN,
        beginReflow);
    UIManager::createButton(
        "settings_btn",
        rightButtonX,
        SCREEN_HEIGHT - buttonHeight - buttonMargin + 3, // Moved down by 3 pixels
        buttonWidth,
        buttonHeight,
        10,
        "Settings",
        SCREEN_MAIN,
        goToSettings);
  }

  static void setupSettingsScreenButtons()
  {
    const int backButtonWidth = SCREEN_WIDTH - 4; // Full screen width minus 4 pixels margin
    const int backButtonX = 2;                    // 2 pixels margin from left

    UIManager::createButton(
        "back_from_settings_btn",
        backButtonX,
        SCREEN_HEIGHT - buttonHeight - buttonMargin + 3, // Moved down by 3 pixels
        backButtonWidth,
        buttonHeight,
        10,
        "Back",
        SCREEN_SETTINGS,
        goToMain);

    // New temperature control table layout - moved right and down
    const int tableX = 120; // Moved right from 90
    const int tableY = 55;  // Moved down from 40
    const int cellWidth = 40;
    const int cellHeight = 40;
    const int buttonSize = 40;
    // Soak Temperature Controls
    UIManager::createButton("soak_temp_plus10", tableX, tableY,
                            buttonSize, buttonSize, 5, "+",
                            SCREEN_SETTINGS, increaseSoakTempCoarse);

    UIManager::createButton("soak_temp_plus1", tableX + cellWidth, tableY,
                            buttonSize, buttonSize, 5, "+",
                            SCREEN_SETTINGS, increaseSoakTempFine);

    UIManager::createButton("soak_temp_minus10", tableX, tableY + cellHeight,
                            buttonSize, buttonSize, 5, "-",
                            SCREEN_SETTINGS, decreaseSoakTempCoarse);

    UIManager::createButton("soak_temp_minus1", tableX + cellWidth, tableY + cellHeight,
                            buttonSize, buttonSize, 5, "-",
                            SCREEN_SETTINGS, decreaseSoakTempFine);

    // Reflow Temperature Controls
    UIManager::createButton("reflow_temp_plus10", tableX, tableY + (cellHeight * 2),
                            buttonSize, buttonSize, 5, "+",
                            SCREEN_SETTINGS, increaseReflowTempCoarse);

    UIManager::createButton("reflow_temp_plus1", tableX + cellWidth, tableY + (cellHeight * 2),
                            buttonSize, buttonSize, 5, "+",
                            SCREEN_SETTINGS, increaseReflowTempFine);

    UIManager::createButton("reflow_temp_minus10", tableX, tableY + (cellHeight * 3),
                            buttonSize, buttonSize, 5, "-",
                            SCREEN_SETTINGS, decreaseReflowTempCoarse);

    UIManager::createButton("reflow_temp_minus1", tableX + cellWidth, tableY + (cellHeight * 3),
                            buttonSize, buttonSize, 5, "-",
                            SCREEN_SETTINGS, decreaseReflowTempFine);

    // Comment out the light mode and accent buttons
    /*
    UIManager::createButton("invert_accent_btn", 3, SCREEN_HEIGHT - 100 + 3, 112, 40, 10,
                            "Gray & Red", SCREEN_SETTINGS, UIManager::toggleInvertAccent);

    UIManager::createButton("light_mode_btn", (SCREEN_WIDTH - 112) - 3, SCREEN_HEIGHT - 100 + 3, 112, 40, 10,
                            "Light Mode", SCREEN_SETTINGS, UIManager::toggleLightMode);
    */
  }

  static void setupFontScreenButtons()
  {
    const int backButtonWidth = SCREEN_WIDTH - 4; // Full screen width minus 4 pixels margin
    const int backButtonX = 2;                    // 2 pixels margin from left
    UIManager::createButton(
        "back_from_fonts_btn",
        backButtonX,
        SCREEN_HEIGHT - buttonHeight - buttonMargin + 3, // Moved down by 3 pixels
        backButtonWidth,
        buttonHeight,
        10,
        "Back",
        SCREEN_FONTS,
        goToMain);

    UIManager::createButton(
        "prev_font_btn",
        10,
        SCREEN_HEIGHT - buttonHeight - 60 + 3, // Moved down by 3 pixels
        60,
        buttonHeight,
        10,
        "<",
        SCREEN_FONTS,
        prevFont);

    UIManager::createButton(
        "next_font_btn",
        SCREEN_WIDTH - 70,
        SCREEN_HEIGHT - buttonHeight - 60 + 3, // Moved down by 3 pixels
        60,
        buttonHeight,
        10,
        ">",
        SCREEN_FONTS,
        nextFont);

    // Full width for the toggle button
    String buttonText = "Affect buttons: ";
    buttonText += (IntegratedFontReflowGUI::affectButtons ? "True" : "False");
    UIManager::createButton(
        "toggle_affect_buttons",
        2,                                      // 2 pixels margin from left
        SCREEN_HEIGHT - buttonHeight - 110 + 3, // Moved down by 3 pixels
        SCREEN_WIDTH - 4,                       // Full screen width minus 4 pixels margin
        buttonHeight,
        10,
        buttonText,
        SCREEN_FONTS,
        toggleAffectButtons);
  }

  static void goToMain()
  {
    UIManager::setCurrentScreen(SCREEN_MAIN);
    // IntegratedFontReflowGUI::updateFontDisplay();
  }

  static void goToFonts()
  {
    UIManager::setCurrentScreen(SCREEN_FONTS);
    // IntegratedFontReflowGUI::updateFontDisplay();
  }

  static void goToSettings()
  {
    UIManager::setCurrentScreen(SCREEN_SETTINGS);
    // IntegratedFontReflowGUI::updateFontDisplay();
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
    UIManager::updateTextElementContent("soak_temp_value", String(IntegratedFontReflowGUI::soakTemp) + " C");
  }

  static void decreaseSoakTempCoarse()
  {
    IntegratedFontReflowGUI::soakTemp -= 10;
    UIManager::updateTextElementContent("soak_temp_value", String(IntegratedFontReflowGUI::soakTemp) + " C");
  }

  static void increaseSoakTempFine()
  {
    IntegratedFontReflowGUI::soakTemp += 1;
    UIManager::updateTextElementContent("soak_temp_value", String(IntegratedFontReflowGUI::soakTemp) + " C");
  }

  static void decreaseSoakTempFine()
  {
    IntegratedFontReflowGUI::soakTemp -= 1;
    UIManager::updateTextElementContent("soak_temp_value", String(IntegratedFontReflowGUI::soakTemp) + " C");
  }

  static void increaseReflowTempCoarse()
  {
    IntegratedFontReflowGUI::reflowTemp += 10;
    UIManager::updateTextElementContent("reflow_temp_value", String(IntegratedFontReflowGUI::reflowTemp) + " C");
  }

  static void decreaseReflowTempCoarse()
  {
    IntegratedFontReflowGUI::reflowTemp -= 10;
    UIManager::updateTextElementContent("reflow_temp_value", String(IntegratedFontReflowGUI::reflowTemp) + " C");
  }

  static void increaseReflowTempFine()
  {
    IntegratedFontReflowGUI::reflowTemp += 1;
    UIManager::updateTextElementContent("reflow_temp_value", String(IntegratedFontReflowGUI::reflowTemp) + " C");
  }

  static void decreaseReflowTempFine()
  {
    IntegratedFontReflowGUI::reflowTemp -= 1;
    UIManager::updateTextElementContent("reflow_temp_value", String(IntegratedFontReflowGUI::reflowTemp) + " C");
  }

  static void toggleAffectButtons()
  {
    IntegratedFontReflowGUI::toggleAffectButtons();
  }

  // Updated reflow button callback to use the improved controller
  static void beginReflow()
  {
    Serial.println("Reflow button pressed");

    // Set thermal lag offset to 50°C
    ReflowController::setThermalLagOffset(50.0);

    // Set target temperature to the reflow temperature from settings
    ReflowController::setTargetTemperature(IntegratedFontReflowGUI::reflowTemp);

    // Update duty cycle coefficient to get appropriate values
    // For a target of 150°C, we want 15% duty cycle: 0.15/150 = 0.001
    ReflowController::setDutyCycleCoefficient(0.001f);

    // Start the reflow process
    ReflowController::startReflow();

    // Update the target temperature display
    UIManager::updateTextElementContent("target_temp_display",
        String(IntegratedFontReflowGUI::reflowTemp) + "C");
  }
};

