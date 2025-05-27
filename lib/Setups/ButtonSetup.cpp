#include "ButtonSetup.hpp"

// Initialize static member variables
bool ButtonSetup::reflowConfirmationNeeded = false;
unsigned long ButtonSetup::reflowConfirmationTimestamp = 0;

void ButtonSetup::setupAllButtons()
{
  setupFontScreenButtons();
  setupMainScreenButtons();
  setupSettingsScreenButtons();
}

void ButtonSetup::setupMainScreenButtons()
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

void ButtonSetup::setupSettingsScreenButtons()
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

void ButtonSetup::setupFontScreenButtons()
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

void ButtonSetup::goToMain()
{
  UIManager::setCurrentScreen(SCREEN_MAIN);
  // IntegratedFontReflowGUI::updateFontDisplay();
}

void ButtonSetup::goToFonts()
{
  UIManager::setCurrentScreen(SCREEN_FONTS);
  // IntegratedFontReflowGUI::updateFontDisplay();
}

void ButtonSetup::goToSettings()
{
  UIManager::setCurrentScreen(SCREEN_SETTINGS);
  // IntegratedFontReflowGUI::updateFontDisplay();
}

void ButtonSetup::nextFont()
{
  IntegratedFontReflowGUI::currentFontIndex =
      (IntegratedFontReflowGUI::currentFontIndex + 1) % IntegratedFontReflowGUI::fontCount;
  IntegratedFontReflowGUI::updateFontDisplay();
}

void ButtonSetup::prevFont()
{
  IntegratedFontReflowGUI::currentFontIndex =
      (IntegratedFontReflowGUI::currentFontIndex - 1 + IntegratedFontReflowGUI::fontCount) % IntegratedFontReflowGUI::fontCount;
  IntegratedFontReflowGUI::updateFontDisplay();
}

void ButtonSetup::increaseSoakTempCoarse()
{
  IntegratedFontReflowGUI::soakTemp += 10;
  UIManager::updateTextElementContent("soak_temp_value", String(IntegratedFontReflowGUI::soakTemp) + " C");
}

void ButtonSetup::decreaseSoakTempCoarse()
{
  IntegratedFontReflowGUI::soakTemp -= 10;
  UIManager::updateTextElementContent("soak_temp_value", String(IntegratedFontReflowGUI::soakTemp) + " C");
}

void ButtonSetup::increaseSoakTempFine()
{
  IntegratedFontReflowGUI::soakTemp += 1;
  UIManager::updateTextElementContent("soak_temp_value", String(IntegratedFontReflowGUI::soakTemp) + " C");
}

void ButtonSetup::decreaseSoakTempFine()
{
  IntegratedFontReflowGUI::soakTemp -= 1;
  UIManager::updateTextElementContent("soak_temp_value", String(IntegratedFontReflowGUI::soakTemp) + " C");
}

void ButtonSetup::increaseReflowTempCoarse()
{
  IntegratedFontReflowGUI::reflowTemp += 10;
  UIManager::updateTextElementContent("reflow_temp_value", String(IntegratedFontReflowGUI::reflowTemp) + " C");
}

void ButtonSetup::decreaseReflowTempCoarse()
{
  IntegratedFontReflowGUI::reflowTemp -= 10;
  UIManager::updateTextElementContent("reflow_temp_value", String(IntegratedFontReflowGUI::reflowTemp) + " C");
}

void ButtonSetup::increaseReflowTempFine()
{
  IntegratedFontReflowGUI::reflowTemp += 1;
  UIManager::updateTextElementContent("reflow_temp_value", String(IntegratedFontReflowGUI::reflowTemp) + " C");
}

void ButtonSetup::decreaseReflowTempFine()
{
  IntegratedFontReflowGUI::reflowTemp -= 1;
  UIManager::updateTextElementContent("reflow_temp_value", String(IntegratedFontReflowGUI::reflowTemp) + " C");
}

void ButtonSetup::toggleAffectButtons()
{
  IntegratedFontReflowGUI::toggleAffectButtons();
}

void ButtonSetup::beginReflow()
{
  Serial.println("Reflow button pressed");
  if (!reflowConfirmationNeeded) {
    // First press - request confirmation
    reflowConfirmationNeeded = true;
    // Store the current time for timeout tracking
    reflowConfirmationTimestamp = millis();
    // Update button text to ask for confirmation
    UIManager::updateButtonText("reflow_btn", "CONFIRM?");
  } else {
    // Confirmation received - proceed with reflow
    reflowConfirmationNeeded = false;
    // Reset the button text
    UIManager::updateButtonText("reflow_btn", "Reflow");
    // Set thermal lag offset to 50°C
    ReflowController::setThermalLagOffset(50.0);
    // Set target temperature to the reflow temperature from settings
    ReflowController::setTargetTemperature(IntegratedFontReflowGUI::soakTemp);
    // Update duty cycle coefficient to get appropriate values
    // For a target of 150°C, we want 15% duty cycle: 0.15/150 = 0.001
    ReflowController::setDutyCycleCoefficient(0.001f);
    // Start the reflow process
    ReflowController::startReflow();
    // Update the target temperature display
    UIManager::updateTextElementContent("target_temp_display",
        String(IntegratedFontReflowGUI::reflowTemp) + "C");
  }
}

void ButtonSetup::checkReflowConfirmationTimeout() {
  // If confirmation is needed and 10 seconds have passed
  if (reflowConfirmationNeeded && (millis() - reflowConfirmationTimestamp > 10000)) {
    // Reset confirmation state
    reflowConfirmationNeeded = false;
    // Reset button text
    UIManager::updateButtonText("reflow_btn", "Reflow");
  }
}
