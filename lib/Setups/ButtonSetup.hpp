#pragma once

#include "../UIManager/UIManager.hpp"
#include "../IntegratedFontReflowGUI/IntegratedFontReflowGUI.hpp"
#include <bb_captouch.h>
#include "../TempManager/TempManager.hpp"
#include "../TempManager/ReflowController.hpp"

// Constants for button dimensions
#define BUTTON_HEIGHT 40
#define BUTTON_MARGIN 10
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

class ButtonSetup
{
public:
  // Setup all buttons for the application
  static void setupAllButtons();
  static void checkReflowConfirmationTimeout();     // Add this line

private:
  static constexpr int buttonMargin = BUTTON_MARGIN;
  static constexpr int buttonHeight = BUTTON_HEIGHT;
  // Static variable for reflow confirmation
  static bool reflowConfirmationNeeded;
  static unsigned long reflowConfirmationTimestamp; // Add this line
 

  static void setupMainScreenButtons();
  static void setupSettingsScreenButtons();
  static void setupFontScreenButtons();

  static void goToMain();
  static void goToFonts();
  static void goToSettings();
  static void nextFont();
  static void prevFont();

  static void increaseSoakTempCoarse();
  static void decreaseSoakTempCoarse();
  static void increaseSoakTempFine();
  static void decreaseSoakTempFine();

  static void increaseReflowTempCoarse();
  static void decreaseReflowTempCoarse();
  static void increaseReflowTempFine();
  static void decreaseReflowTempFine();

  static void toggleAffectButtons();
  static void beginReflow();
};

