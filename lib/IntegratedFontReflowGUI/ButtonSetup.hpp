#pragma once

#include "UIManager.hpp"
#include <functional>
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
    void setupAllButtons(const std::function<void()>& goToSettings,
                         const std::function<void()>& goToMain,
                         const std::function<void()>& goToFonts,
                         const std::function<void()>& toggleLightMode,
                         const std::function<void()>& toggleInvertAccent,
                         const std::function<void()>& nextFont,
                         const std::function<void()>& prevFont,
                         const std::function<void()>& increaseSoakTempCoarse,
                         const std::function<void()>& decreaseSoakTempCoarse,
                         const std::function<void()>& increaseSoakTempFine,
                         const std::function<void()>& decreaseSoakTempFine,
                         const std::function<void()>& increaseReflowTempCoarse,
                         const std::function<void()>& decreaseReflowTempCoarse,
                         const std::function<void()>& increaseReflowTempFine,
                         const std::function<void()>& decreaseReflowTempFine) {

        setupMainScreenButtons(goToSettings, goToFonts);
        setupSettingsScreenButtons(goToMain, toggleLightMode, toggleInvertAccent,
                                 increaseSoakTempCoarse, decreaseSoakTempCoarse,
                                 increaseSoakTempFine, decreaseSoakTempFine,
                                 increaseReflowTempCoarse, decreaseReflowTempCoarse,
                                 increaseReflowTempFine, decreaseReflowTempFine);
        setupFontScreenButtons(goToMain, nextFont, prevFont);
    }

private:
// Setup main screen buttons
    void setupMainScreenButtons(const std::function<void()>& goToSettings,
                              const std::function<void()>& goToFonts) {
  const int buttonMargin = BUTTON_MARGIN;
        const int buttonWidth = (SCREEN_WIDTH - (3 * buttonMargin)) / 2;
  const int leftButtonX = buttonMargin;
  const int rightButtonX = leftButtonX + buttonWidth + buttonMargin;

  // Font Test button on the left
  uiManager.createButton(
      "font_test_btn",
            leftButtonX,
      SCREEN_HEIGHT - buttonHeight - buttonMargin,
            buttonWidth,
            buttonHeight,
            10,
            "Font Test",
            SCREEN_MAIN,
            goToFonts
  );

        // Settings button on the right
  uiManager.createButton(
            "settings_btn",
            rightButtonX,
            SCREEN_HEIGHT - buttonHeight - buttonMargin,
            buttonWidth,
            buttonHeight,
            10,
            "Settings",
            SCREEN_MAIN,
            goToSettings
  );
    }
  
    // Setup settings screen buttons
    void setupSettingsScreenButtons(const std::function<void()>& goToMain,
                                  const std::function<void()>& toggleLightMode,
                                  const std::function<void()>& toggleInvertAccent,
                                  const std::function<void()>& increaseSoakTempCoarse,
                                  const std::function<void()>& decreaseSoakTempCoarse,
                                  const std::function<void()>& increaseSoakTempFine,
                                  const std::function<void()>& decreaseSoakTempFine,
                                  const std::function<void()>& increaseReflowTempCoarse,
                                  const std::function<void()>& decreaseReflowTempCoarse,
                                  const std::function<void()>& increaseReflowTempFine,
                                  const std::function<void()>& decreaseReflowTempFine) {
        const int buttonMargin = BUTTON_MARGIN;
        const int backButtonWidth = 160;
        const int backButtonX = (SCREEN_WIDTH - backButtonWidth) / 2;

        // Back button
  uiManager.createButton(
            "back_from_settings_btn",
            backButtonX,
            SCREEN_HEIGHT - buttonHeight - buttonMargin,
            backButtonWidth,
            buttonHeight,
            10,
            "Back",
            SCREEN_SETTINGS,
            goToMain();
  );
  
        // Temperature controls in settings screen
        int tempBoxX = 130;
        int tempBoxY = 10;

        // Soak Temp Controls
        uiManager.createButton("soak_temp_plus10", tempBoxX, tempBoxY, 40, 40, 5, "+10",
                             SCREEN_SETTINGS, increaseSoakTempCoarse);
        uiManager.createButton("soak_temp_minus10", tempBoxX, tempBoxY + 50, 40, 40, 5, "-10",
                             SCREEN_SETTINGS, decreaseSoakTempCoarse);
        uiManager.createButton("soak_temp_plus1", tempBoxX + 60, tempBoxY, 40, 40, 5, "+1",
                             SCREEN_SETTINGS, increaseSoakTempFine);
        uiManager.createButton("soak_temp_minus1", tempBoxX + 60, tempBoxY + 50, 40, 40, 5, "-1",
                             SCREEN_SETTINGS, decreaseSoakTempFine);

        // Reflow Temp Controls
        tempBoxY += 100;
        uiManager.createButton("reflow_temp_plus10", tempBoxX, tempBoxY, 40, 40, 5, "+10",
                             SCREEN_SETTINGS, increaseReflowTempCoarse);
        uiManager.createButton("reflow_temp_minus10", tempBoxX, tempBoxY + 50, 40, 40, 5, "-10",
                             SCREEN_SETTINGS, decreaseReflowTempCoarse);
        uiManager.createButton("reflow_temp_plus1", tempBoxX + 60, tempBoxY, 40, 40, 5, "+1",
                             SCREEN_SETTINGS, increaseReflowTempFine);
        uiManager.createButton("reflow_temp_minus1", tempBoxX + 60, tempBoxY + 50, 40, 40, 5, "-1",
                             SCREEN_SETTINGS, decreaseReflowTempFine);

        // Accent Invert Button
        uiManager.createButton("invert_accent_btn", 3, SCREEN_HEIGHT - 100, 112, 40, 10,
                             "Invert Accent", SCREEN_SETTINGS, toggleInvertAccent);

        // Light Mode Toggle Button
        uiManager.createButton("light_mode_btn", (SCREEN_WIDTH - 112) - 3, SCREEN_HEIGHT - 100, 112, 40, 10,
                             "Light Mode", SCREEN_SETTINGS, toggleLightMode);
    }

    // Setup font screen buttons
    void setupFontScreenButtons(const std::function<void()>& goToMain,
                              const std::function<void()>& nextFont,
                              const std::function<void()>& prevFont) {
        const int buttonMargin = BUTTON_MARGIN;
        const int backButtonWidth = 160;
        const int backButtonX = (SCREEN_WIDTH - backButtonWidth) / 2;

        // Font test screen back button
  uiManager.createButton(
            "back_from_fonts_btn",
            backButtonX,
            SCREEN_HEIGHT - buttonHeight - buttonMargin,
            backButtonWidth,
            buttonHeight,
            10,
            "Back",
            SCREEN_FONTS,
            goToMain
  );

        // Font navigation buttons (left/right)
        uiManager.createButton(
            "prev_font_btn",
            10,
            SCREEN_HEIGHT - buttonHeight - 60,
            60,
            buttonHeight,
            10,
            "<",
            SCREEN_FONTS,
            prevFont
        );

        uiManager.createButton(
            "next_font_btn",
            SCREEN_WIDTH - 70,
            SCREEN_HEIGHT - buttonHeight - 60,
            60,
            buttonHeight,
            10,
            ">",
            SCREEN_FONTS,
            nextFont
        );
}
};
