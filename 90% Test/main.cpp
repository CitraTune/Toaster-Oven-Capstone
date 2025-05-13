#include <Arduino.h>
#include "LGFX_Config.h"
#include <bb_captouch.h>

// Define pins for capacitive touch
#define TOUCH_SDA 33
#define TOUCH_SCL 32
#define TOUCH_INT 21
#define TOUCH_RST 25

// Screen dimensions
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

// Define screens
#define SCREEN_MAIN 0
#define SCREEN_SETTINGS 1

LGFX myTFT;
BBCapTouch touch;

// Current active screen
int currentScreen = SCREEN_MAIN;

// Light mode setting (default: off)
bool lightMode = false;

//Graph full screen setting (default: off)
bool graphFullScreen = false; // Track if the graph is in fullscreen mode

// Add new variables for temperature and accent inversion
int soakTemp = 150;  // Default soak temperature
int reflowTemp = 230;  // Default reflow temperature
bool invertAccent = false;  // Default: no accent inversion

// Define colors
uint16_t outlineColor = TFT_DARKGRAY; // Outline color for buttons

// Button structure to make management easier
struct Button {
  int x;
  int y;
  int width;
  int height;
  int radius;          // Corner radius
  uint16_t color;      // Button color
  uint16_t textColor;  // Text color
  String label;        // Button text
  int screen;          // Which screen this button belongs to
  bool active;         // Is this button currently active/visible
  void (*action)();    // Function pointer for button action
};

// Array to store our buttons
#define MAX_BUTTONS 14
Button buttons[MAX_BUTTONS];
int buttonCount = 0;

// Debounce variables
unsigned long lastTouchTime = 0;
const unsigned long debounceDelay = 200; // 200ms debounce delay

// Function declarations
void navigateToScreen(int screen);
void goToSettings();
void goToMain();
void toggleLightMode();
int createButton(int x, int y, int width, int height, int radius, uint16_t color, uint16_t textColor, String label, int screen, void (*action)());
void drawButton(Button &button);
void drawActiveScreen();
void checkButtonPress(int touchX, int touchY);

// Function declarations for temperature adjustments
void increaseSoakTempCoarse();
void decreaseSoakTempCoarse();
void increaseSoakTempFine();
void decreaseSoakTempFine();
void increaseReflowTempCoarse();
void decreaseReflowTempCoarse();
void increaseReflowTempFine();
void decreaseReflowTempFine();
void toggleInvertAccent();
void updateReflowTempDisplay();
void drawGraph(bool fullScreen);
void toggleGraphSize();

void setup() {
  Serial.begin(115200);
  
  // setup the TFT display
  myTFT.begin();
  myTFT.setRotation(7);     // Adjust based on your display orientation
  myTFT.invertDisplay(true); // Optionally invert colors
  myTFT.setBrightness(128);
  myTFT.setColorDepth(24);
  
  // setup touch controller
  touch.setup(TOUCH_SDA, TOUCH_SCL, TOUCH_RST, TOUCH_INT);
  
  // Set the default font
  myTFT.setFont(&lgfx::fonts::FreeSans9pt7b);
  
  // Get font height for dynamic button sizing
  int fontHeight = myTFT.fontHeight();
  int buttonTextMargin = 10; // Margin around text (top and bottom)
  int buttonHeight = fontHeight + 2 * buttonTextMargin; // Dynamic height based on font
  
  // Create our buttons
  
  // Settings button on main screen - centered at bottom
  createButton(
    (SCREEN_WIDTH - 160) / 2, // x - centered
    SCREEN_HEIGHT - buttonHeight - 10, // y - positioned from bottom with margin
    160,                      // width
    buttonHeight,             // height - dynamically sized
    10,                       // radius - rounded corners
    TFT_ORANGE,               // color - orange
    TFT_WHITE,                // text color
    "Settings",               // label
    SCREEN_MAIN,              // screen
    goToSettings              // action function
  );
  
  // Back button on settings screen - centered at bottom
  createButton(
    (SCREEN_WIDTH - 160) / 2, // x - centered
    SCREEN_HEIGHT - buttonHeight - 10, // y - positioned from bottom with margin
    160,                      // width
    buttonHeight,             // height - dynamically sized
    10,                       // radius - rounded corners
    TFT_ORANGE,               // color - orange
    TFT_WHITE,                // text color
    "Back",                   // label
    SCREEN_SETTINGS,          // screen
    goToMain                  // action function
  ); 
  // Create temperature controls in settings screen
  int tempBoxX = 130;
  int tempBoxY = 10;
  // Soak Temp Box
  //createButton(tempBoxX, tempBoxY, tempBoxWidth, tempBoxHeight, 5, TFT_LIGHTGRAY, TFT_BLACK, "Soak Temp", SCREEN_SETTINGS, NULL);
  createButton(tempBoxX, tempBoxY, 40, 40, 5, TFT_ORANGE, TFT_WHITE, "+10", SCREEN_SETTINGS, increaseSoakTempCoarse);
  createButton(tempBoxX, tempBoxY + 50, 40, 40, 5, TFT_ORANGE, TFT_WHITE, "-10", SCREEN_SETTINGS, decreaseSoakTempCoarse);
  createButton(tempBoxX + 60, tempBoxY, 40, 40, 5, TFT_ORANGE, TFT_WHITE, "+1", SCREEN_SETTINGS, increaseSoakTempFine);
  createButton(tempBoxX + 60, tempBoxY + 50, 40, 40, 5, TFT_ORANGE, TFT_WHITE, "-1", SCREEN_SETTINGS, decreaseSoakTempFine);
  // Reflow Temp Box
  tempBoxY += 100;
  //createButton(tempBoxX, tempBoxY, tempBoxWidth, tempBoxHeight, 5, TFT_LIGHTGRAY, TFT_BLACK, "Reflow Temp", SCREEN_SETTINGS, NULL);
  createButton(tempBoxX, tempBoxY, 40, 40, 5, TFT_ORANGE, TFT_WHITE, "+10", SCREEN_SETTINGS, increaseReflowTempCoarse);
  createButton(tempBoxX, tempBoxY + 50, 40, 40, 5, TFT_ORANGE, TFT_WHITE, "-10", SCREEN_SETTINGS, decreaseReflowTempCoarse);
  createButton(tempBoxX + 60, tempBoxY, 40, 40, 5, TFT_ORANGE, TFT_WHITE, "+1", SCREEN_SETTINGS, increaseReflowTempFine);
  createButton(tempBoxX + 60, tempBoxY + 50, 40, 40, 5, TFT_ORANGE, TFT_WHITE, "-1", SCREEN_SETTINGS, decreaseReflowTempFine);

  // Accent Invert Button
  createButton(3, SCREEN_HEIGHT - 100, 112, 40, 10, TFT_ORANGE, TFT_WHITE, "Invert Accent", SCREEN_SETTINGS, toggleInvertAccent);

  //Light Mode Toggle Button
  createButton((SCREEN_WIDTH - 112) - 3, SCREEN_HEIGHT - 100, 112, 40, 10, TFT_ORANGE, TFT_WHITE, "Light Mode", SCREEN_SETTINGS, toggleLightMode);

  //Add buttons for "Font Test" and "Swap Graph"
  createButton(
    3, SCREEN_HEIGHT - 100, 112, 40, 10, TFT_ORANGE, TFT_WHITE, "Swap Graph", SCREEN_MAIN, toggleGraphSize
  );

  createButton(
    (SCREEN_WIDTH - 112) - 3, SCREEN_HEIGHT - 100, 112, 40, 10, TFT_ORANGE, TFT_WHITE, "Font Test", SCREEN_MAIN, NULL
  );

  // setupial screen setup
  navigateToScreen(SCREEN_MAIN);
}

void loop() {
  TOUCHINFO ti;
  
  // Get touch samples
  if (touch.getSamples(&ti)) {
    unsigned long currentTime = millis();
    if (currentTime - lastTouchTime > debounceDelay) {
      lastTouchTime = currentTime;
      
      int x = ti.x[0];
      int y = ti.y[0];
      
      // Debugging output
      Serial.printf("Touch x=%d y=%d\n", x, y);
      
      // Check if any button was pressed
      checkButtonPress(x, y);
    }
  }
  
  delay(10);  // Reduced delay for smoother UI
}

// Create a new button and return its index
int createButton(int x, int y, int width, int height, int radius, uint16_t color, uint16_t textColor, String label, int screen, void (*action)()) {
  if (buttonCount >= MAX_BUTTONS) {
    Serial.println("Warning: Maximum button count reached!");
    return -1;
  }
  
  Button newButton;
  newButton.x = x;
  newButton.y = y;
  newButton.width = width;
  newButton.height = height;
  newButton.radius = radius;
  newButton.color = color;
  newButton.textColor = textColor;
  newButton.label = label;
  newButton.screen = screen;
  newButton.active = (screen == currentScreen);
  newButton.action = action;
  
  buttons[buttonCount] = newButton;
  return buttonCount++;
}

// Draw single button
void drawButton(Button &button) {
  // Calculate text dimensions for this particular button text
  myTFT.setFont(&lgfx::fonts::FreeSans9pt7b);
  int textWidth = myTFT.textWidth(button.label);
  int textHeight = myTFT.fontHeight();
  
  // Draw filled rounded rectangle for button
  myTFT.fillRoundRect(button.x, button.y, button.width, button.height, button.radius, button.color);
  
  // Draw thicker gray outline
  myTFT.drawRoundRect(button.x, button.y, button.width, button.height, button.radius, outlineColor);
  myTFT.drawRoundRect(button.x - 1, button.y - 1, button.width + 2, button.height + 2, button.radius + 1, outlineColor);
  
  // Calculate text position to center it in the button (horizontally and vertically)
  int textX = button.x + (button.width - textWidth) / 2;
  int textY = button.y + (button.height - textHeight) / 2;
  
  // Draw text
  myTFT.setTextColor(button.textColor);
  myTFT.setCursor(textX, textY);
  myTFT.print(button.label);
}

// Check if a button was pressed
void checkButtonPress(int touchX, int touchY) {
  for (int i = 0; i < buttonCount; i++) {
    // Only check active buttons
    if (!buttons[i].active) continue;
    
    // Check if touch is within button boundaries
    if (touchX >= buttons[i].x && touchX <= buttons[i].x + buttons[i].width &&
        touchY >= buttons[i].y && touchY <= buttons[i].y + buttons[i].height) {
      
      // Execute button action if assigned
      if (buttons[i].action != NULL) {
        buttons[i].action();
      }
      
      break;  // Exit after handling one button press
    }
  }
}

// Navigate to specified screen
void navigateToScreen(int screen) {
  currentScreen = screen;
  
  // Update button active states
  for (int i = 0; i < buttonCount; i++) {
    buttons[i].active = (buttons[i].screen == currentScreen);
  }
  
  // Draw the new screen
  drawActiveScreen();
}

// Draw current active screen with all its elements
void drawActiveScreen() {
  uint16_t bgColor = lightMode ? TFT_WHITE : TFT_BLACK;
  uint16_t textColor = lightMode ? TFT_BLACK : TFT_WHITE;
  
  myTFT.fillScreen(bgColor);  // Clear screen
  
  // Define small margin for titles
  const int MARGIN = 10;
  
  if (currentScreen == SCREEN_MAIN) {
    // Draw main screen title in top left with margin
    myTFT.setTextColor(textColor);
    myTFT.setFont(&lgfx::fonts::FreeSans12pt7b);  // Increased font size
    myTFT.setCursor(MARGIN, MARGIN);  // Adjusted position
    myTFT.print("Main Menu");
    drawGraph(graphFullScreen);  // Draw graph in main screen
  } 



  else if (currentScreen == SCREEN_SETTINGS) {
    // Draw settings screen title in top left with margin
    myTFT.setTextColor(textColor);
    myTFT.setFont(&lgfx::fonts::FreeSans12pt7b);  // Increased font size
    myTFT.setCursor(MARGIN, MARGIN);  // Adjusted position
    myTFT.print("Settings");

    // Draw temperature values
    myTFT.setFont(&lgfx::fonts::FreeSans9pt7b);
    myTFT.setCursor(10, 50);
    myTFT.printf("Soak Temp\n     %d C", soakTemp);
    myTFT.setCursor(10, 140);
    myTFT.printf("Reflow Temp\n     %d C", reflowTemp);
  }

  // Draw a white outline around the text for the screen label box
  int textWidth = myTFT.textWidth(currentScreen == SCREEN_MAIN ? "Main Menu" : "Settings");
  int textHeight = myTFT.fontHeight();
  int outlineOffset = 5; // Offset for the outline
  
  //method this later. create a universal method for contrasting background or matching background color?
  uint16_t boxColor = lightMode ? TFT_BLACK : TFT_WHITE;

  // Draw the right angle outline
  myTFT.drawLine(0, MARGIN + textHeight+ outlineOffset, MARGIN + textWidth + outlineOffset, MARGIN + textHeight + outlineOffset, boxColor); // Top line
  myTFT.drawLine(MARGIN + textWidth + outlineOffset, 0, MARGIN + textWidth + outlineOffset, MARGIN + textHeight + outlineOffset, boxColor); // Left line
  
  // Draw all active buttons for this screen
  for (int i = 0; i < buttonCount; i++) {
    if (buttons[i].active) {
      drawButton(buttons[i]);
    }
  }
}

// Button action functions
void goToSettings() {
  navigateToScreen(SCREEN_SETTINGS);
}

void goToMain() {
  navigateToScreen(SCREEN_MAIN);
}

void toggleLightMode() {
  lightMode = !lightMode;
  buttons[11].label = !lightMode ? "Light Mode" : "Dark Mode";
  drawActiveScreen();
}

// Button action functions for temperature adjustments
void increaseSoakTempCoarse() { soakTemp += 10; drawActiveScreen(); }
void decreaseSoakTempCoarse() { soakTemp -= 10; drawActiveScreen(); }
void increaseSoakTempFine() { soakTemp += 1; drawActiveScreen(); }
void decreaseSoakTempFine() { soakTemp -= 1; drawActiveScreen(); }
void increaseReflowTempCoarse() { reflowTemp += 10; updateReflowTempDisplay(); }
void decreaseReflowTempCoarse() { reflowTemp -= 10; updateReflowTempDisplay(); }
void increaseReflowTempFine() { reflowTemp += 1; updateReflowTempDisplay(); }
void decreaseReflowTempFine() { reflowTemp -= 1; updateReflowTempDisplay(); }

// Toggle accent inversion
void toggleInvertAccent() {
  invertAccent = !invertAccent;
  outlineColor = invertAccent ? TFT_ORANGE : TFT_DARKGRAY;
  for (int i = 0; i < buttonCount; i++) {
    buttons[i].color = invertAccent ? TFT_LIGHTGRAY : TFT_ORANGE;
  }
  drawActiveScreen();
}

// Update only the area around the reflow temperature number
void updateReflowTempDisplay() {
  int tempX = 10; // X position of the reflow temp text
  int tempY = 140; // Y position of the reflow temp text
  int tempWidth = 100; // Width of the area to clear
  int tempHeight = myTFT.fontHeight() * 2; // Height of the area to clear

  // Clear the area around the reflow temp
  myTFT.fillRect(tempX, tempY, tempWidth, tempHeight, lightMode ? TFT_WHITE : TFT_BLACK);

  // Redraw the reflow temp text
  myTFT.setCursor(tempX, tempY);
  myTFT.setTextColor(lightMode ? TFT_BLACK : TFT_WHITE);
  myTFT.printf("Reflow Temp\n     %d C", reflowTemp);
}

// Adjust the graph to be offset by 40 pixels from the top when fullscreen
void drawGraph(bool fullScreen) {
  int graphX = fullScreen ? 0 : 20;
  int graphY = fullScreen ? 40 : 50; // Offset by 40 pixels when fullscreen
  int graphWidth = fullScreen ? SCREEN_WIDTH : SCREEN_WIDTH - 40;
  int graphHeight = (graphWidth * 6) / 10; // Maintain 10:6 ratio

  uint16_t graphColor = !lightMode ? TFT_WHITE : TFT_BLACK;

  // Clear the previous graph area
  myTFT.fillRect(0, 40, SCREEN_WIDTH, SCREEN_HEIGHT - 40, lightMode ? TFT_WHITE : TFT_BLACK);

  // Draw the graph rectangle
  myTFT.fillRect(graphX, graphY, graphWidth, graphHeight, graphColor);
  myTFT.drawRect(graphX, graphY, graphWidth, graphHeight, graphColor);

  // Draw grid lines to make perfect squares
  int horizontalSpacing = graphWidth / 10;
  int verticalSpacing = graphHeight / 6;

  for (int i = 1; i < 10; i++) {
    myTFT.drawLine(graphX + i * horizontalSpacing, graphY, graphX + i * horizontalSpacing, graphY + graphHeight, TFT_DARKGRAY);
  }

  for (int i = 1; i < 6; i++) {
    myTFT.drawLine(graphX, graphY + i * verticalSpacing, graphX + graphWidth, graphY + i * verticalSpacing, TFT_DARKGRAY);
  }
}

// Update the toggleGraphSize function to redraw the screen
void toggleGraphSize() {
  graphFullScreen = !graphFullScreen;
  drawActiveScreen(); // Redraw the entire screen to clear the old graph and draw the new one
}