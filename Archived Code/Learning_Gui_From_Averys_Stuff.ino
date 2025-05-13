//This is not even for my capstone project but a fellow steam student wanted to take my screen computer board so I made some code for them and sold them the board.

#include <TFT_eSPI.h>
#include <Wire.h>
#include <bb_captouch.h>
#include <TimeLib.h>

// Define pins
#define TOUCH_SDA 33
#define TOUCH_SCL 32
#define TOUCH_INT 21
#define TOUCH_RST 25
#define LCD DISPLAY_CYD

// Create objects
TFT_eSPI tft = TFT_eSPI();
BBCapTouch touch;

// Screen state management
enum ScreenState {
  MAIN_SCREEN,
  SETTINGS_SCREEN,
  TIME_SETTING_SCREEN
};
ScreenState currentScreen = MAIN_SCREEN;

// Colors
#define BG_COLOR TFT_BLACK
#define TEXT_COLOR TFT_WHITE
#define ACCENT_COLOR TFT_DARKGREY  // Changed from BLUE to DARKGREY
#define HIGHLIGHT_COLOR TFT_RED
#define BUTTON_TEXT_COLOR TFT_WHITE

// Time variables
time_t currentTime;
bool use12HourFormat = true;
int feedHour = 8;  // 24-hour format
int feedMinute = 0;
bool feedAM = true;  // This will be derived from feedHour
int foodAmount = 1; // 1-7%

// Current time setting variables (24-hour format)
int currentHour = 8;  // 24-hour format
int currentMinute = 0;
bool currentAM = true;  // This will be derived from currentHour

// Button defsetupions
#define SETTINGS_BTN_X 10
#define SETTINGS_BTN_Y (tft.height() - 60)
#define SETTINGS_BTN_W (tft.width()/2 - 20)  // Half screen width minus margins
#define SETTINGS_BTN_H 50

// New Set Time button dimensions
#define SET_TIME_BTN_X (tft.width()/2 + 10)
#define SET_TIME_BTN_Y (tft.height() - 60)
#define SET_TIME_BTN_W (tft.width()/2 - 20)  // Half screen width minus margins
#define SET_TIME_BTN_H 50

// Settings screen elements
#define ARROW_SIZE 40
#define BACK_BUTTON_W 100
#define BACK_BUTTON_H 40
#define ARROW_BTN_W 50
#define ARROW_BTN_H 40
#define VALUE_W 80
#define VALUE_H 40

// Tracking for partial screen updates
uint16_t oldHour = 99;
uint16_t oldMinute = 99;
uint16_t oldSecond = 99;
uint16_t oldCountdownHours = 99;
uint16_t oldCountdownMins = 99;
int oldFoodAmount = -1;

// Function prototypes
void setupMainScreen();
void updateMainScreen();
void updateClock(bool forceRedraw = false);
void updateCountdown(bool forceRedraw = false);
void setupSettingsScreen();
void setupTimeSettingScreen();
void drawSettingsButton();
void drawSetTimeButton();
void drawButton(int x, int y, int w, int h, const char* label, uint16_t bgColor, uint16_t textColor);
void drawArrowButton(int x, int y, bool isUp, uint16_t color);
void drawValueBox(int x, int y, int w, int h, const char* value, uint16_t bgColor, uint16_t textColor);
void handleTouch();
void handleMainScreenTouch(int x, int y);
void handleSettingsScreenTouch(int x, int y);
void handleTimeSettingScreenTouch(int x, int y);
void updateFeedingTime(int hour, int minute, bool isAM);
void setCurrentTime(int hour, int minute, int second, bool isAM);
void activateServo();
const char* formatHour(int h, bool use12Hour);
bool isAM(int hour);
int convertTo24Hour(int hour, bool isAM);
int convertTo12Hour(int hour);
void getTimeComponents(int &outHour, int &outMinute, int &outSecond, bool &isAMFlag);
void calculateTimeUntilFeeding(int &hours, int &minutes);
bool isFeedingTime();
void updateDisplayRegion(int x, int y, int w, int h, uint16_t color);
void drawTextWithBackground(int x, int y, const char* text, uint16_t textColor, uint16_t bgColor, int textSize);
void animateButton(int x, int y, int w, int h, const char* label, uint16_t bgColor, uint16_t textColor);
void drawAMPMButton(int x, int y, const char* label, bool isSelected);

void setup() {
  Serial.begin(115200);
  Wire.begin(TOUCH_SDA, TOUCH_SCL);
  delay(300);  // Let power stabilize

  // setup display
  tft.setup();
  tft.setRotation(0); // Portrait
  tft.fillScreen(BG_COLOR);
  
  // Enable backlight
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  
  // setup touch
  touch.setup(TOUCH_SDA, TOUCH_SCL, TOUCH_RST, TOUCH_INT);
  Serial.printf("Touch sensor type: %d\n", touch.sensorType());
  
  // Set setupial time (would use RTC or NTP in real implementation)
  setTime(8, 0, 0, 1, 1, 2023);
  
  // Draw setupial screen
  setupMainScreen();
}

void loop() {
  // Update current time
  currentTime = now();
  
  // Update current time variables if needed
  if (currentHour == 0) {
    currentHour = convertTo12Hour(hour(currentTime));
    currentMinute = minute(currentTime);
    currentAM = isAM(hour(currentTime));
  }
  
  // Check for AM/PM change at 11:59
  if (currentMinute == 59 && minute(currentTime) == 0) {
    if (currentHour == 11) {
      currentAM = !currentAM; // Toggle AM/PM at 11:59
    }
    currentHour = convertTo12Hour(hour(currentTime));
    currentMinute = minute(currentTime);
  }
  
  // Check for feeding time only on main screen
  if (currentScreen == MAIN_SCREEN) {
    if (hour(currentTime) == feedHour && minute(currentTime) == feedMinute && second(currentTime) == 0) {
      activateServo();
    }
  }
  
  // Check for settings screen timeout
  static unsigned long lastTouchTime = 0;
  if (currentScreen != MAIN_SCREEN) {
    if (millis() - lastTouchTime > 30000) { // 30 seconds timeout
      currentScreen = MAIN_SCREEN;
      setupMainScreen();
    }
  } else {
    lastTouchTime = millis();
  }
  
  // Update display based on current screen
  if (currentScreen == MAIN_SCREEN) {
    updateMainScreen();
  }
  
  // Handle touch events
  handleTouch();
  
  // Small delay
  delay(50);
}

void setupMainScreen() {
  tft.fillScreen(BG_COLOR);
  
  // Draw settings button
  drawSettingsButton();
  
  // Draw set time button
  drawSetTimeButton();
  
  // Force full redraw of clock and countdown
  updateClock(true);
  updateCountdown(true);
}

void updateMainScreen() {
  updateClock();
  updateCountdown();
}

void updateClock(bool forceRedraw) {
  int h = hour(currentTime);
  int m = minute(currentTime);
  int s = second(currentTime);
  
  // Only update if time has changed or force redraw
  if (forceRedraw || h != oldHour || m != oldMinute) {
    char timeStr[12];
    char ampm[3] = "AM";
    
    // Format based on 12/24 hour preference
    if (use12HourFormat) {
      if (h >= 12) {
        strcpy(ampm, "PM");
        if (h > 12) h -= 12;
      }
      if (h == 0) h = 12;
      sprintf(timeStr, "%2d:%02d", h, m);
    } else {
      sprintf(timeStr, "%02d:%02d", h, m);
    }
    
    // Calculate areas to clear and text positions
    int clockY = 40;
    
    // Clear entire time area
    tft.fillRect(0, clockY, tft.width(), 90, BG_COLOR);
    
    // Draw time
    tft.setTextSize(5);
    tft.setTextColor(TEXT_COLOR);
    
    // Properly center the time text
    int textWidth = strlen(timeStr) * 30; // Approximate for text size 5
    int clockX = (tft.width() - textWidth) / 2;
    tft.setCursor(clockX, clockY);
    tft.print(timeStr);
    
    // Draw AM/PM if using 12-hour format
    if (use12HourFormat) {
      tft.setTextSize(2);  // Reduced size
      // Center the AM/PM text above the seconds dot
      int ampmWidth = strlen(ampm) * 12; // Approximate for text size 2
      int ampmX = (tft.width() - ampmWidth) / 2;
      tft.setCursor(ampmX, clockY + 50);  // Moved up
      tft.print(ampm);
    }
    
    // Update old values
    oldHour = h;
    oldMinute = m;
  }
  
  // Update seconds indicator (small dot that blinks)
  if (forceRedraw || s != oldSecond) {
    // Clear previous dot if needed
    if (s % 2 == 0) {
      tft.fillCircle(tft.width()/2, 120, 2, TEXT_COLOR);  // Reduced size
    } else {
      tft.fillCircle(tft.width()/2, 120, 2, BG_COLOR);    // Reduced size
    }
    oldSecond = s;
  }
}

void updateCountdown(bool forceRedraw) {
  // Calculate time until next feeding
  int currentHour = hour(currentTime);
  int currentMinute = minute(currentTime);
  
  int currentMinutes = currentHour * 60 + currentMinute;
  int feedMinutes = feedHour * 60 + feedMinute;
  
  int minutesToFeed;
  if (feedMinutes > currentMinutes) {
    minutesToFeed = feedMinutes - currentMinutes;
  } else {
    minutesToFeed = (24 * 60) - currentMinutes + feedMinutes;
  }
  
  int hoursToFeed = minutesToFeed / 60;
  int minsToFeed = minutesToFeed % 60;
  
  // Only update if countdown has changed or force redraw
  if (forceRedraw || hoursToFeed != oldCountdownHours || minsToFeed != oldCountdownMins) {
    // Format countdown
    char countdownStr[12];
    sprintf(countdownStr, "%d:%02d", hoursToFeed, minsToFeed);
    
    // Clear previous text
    int countdownY = 170;
    int labelY = countdownY - 20;
    tft.fillRect(0, labelY, tft.width(), 80, BG_COLOR);
    
    // Draw label
    tft.setTextColor(TEXT_COLOR);
    tft.setTextSize(1);
    
    // Center label text
    const char* labelText = "TIME UNTIL FOOD";
    int labelWidth = strlen(labelText) * 6; // Approximation for text size 1
    tft.setCursor((tft.width() - labelWidth) / 2, labelY);
    tft.print(labelText);
    
    // Draw countdown time
    tft.setTextSize(4);
    
    // Approximate center
    int textWidth = strlen(countdownStr) * 24; // Approximate for text size 4
    int countdownX = (tft.width() - textWidth) / 2;
    tft.setCursor(countdownX, countdownY);
    tft.print(countdownStr);
    
    // Update old values
    oldCountdownHours = hoursToFeed;
    oldCountdownMins = minsToFeed;
  }
}

void drawSettingsButton() {
  drawButton(SETTINGS_BTN_X, SETTINGS_BTN_Y, SETTINGS_BTN_W, SETTINGS_BTN_H, "Settings", ACCENT_COLOR, BUTTON_TEXT_COLOR,2);
}

void drawSetTimeButton() {
  drawButton(SET_TIME_BTN_X, SET_TIME_BTN_Y, SET_TIME_BTN_W, SET_TIME_BTN_H, "Set Time", ACCENT_COLOR, BUTTON_TEXT_COLOR,2);
}

void drawButton(int x, int y, int w, int h, const char* label, uint16_t bgColor, uint16_t textColor, int textSize) {
  tft.fillRoundRect(x, y, w, h, 8, bgColor);
  tft.drawRoundRect(x, y, w, h, 8, textColor);
  
  tft.setTextColor(textColor);
  tft.setTextSize(textSize);
  
  // Center text in button
  int textWidth = strlen(label) * 12; // Approximate for text size 2
  int textX = x + (w - textWidth) / 2;
  int textY = y + (h - 16) / 2;
  
  tft.setCursor(textX, textY);
  tft.print(label);
}

void drawArrowButton(int x, int y, bool isUp, uint16_t color) {
  tft.fillRoundRect(x, y, ARROW_BTN_W, ARROW_BTN_H, 5, color);
  
  // Draw arrow
  int centerX = x + ARROW_BTN_W / 2;
  int centerY = y + ARROW_BTN_H / 2;
  int arrowSize = 10;
  
  tft.setTextColor(TEXT_COLOR);
  tft.setTextSize(3);
  tft.setCursor(centerX - 9, centerY - 12);
  
  if (isUp) {
    tft.print("^");
  } else {
    tft.print("v");
  }
}

void drawValueBox(int x, int y, int w, int h, const char* value, uint16_t bgColor, uint16_t textColor) {
  tft.fillRect(x, y, w, h, bgColor);
  tft.drawRect(x, y, w, h, textColor);
  
  tft.setTextColor(textColor);
  tft.setTextSize(2);
  
  // Center text in box
  int textWidth = strlen(value) * 12; // Approximate for text size 2
  int textX = x + (w - textWidth) / 2;
  int textY = y + (h - 16) / 2;
  
  tft.setCursor(textX, textY);
  tft.print(value);
}

void setupSettingsScreen() {
  tft.fillScreen(BG_COLOR);
  
  // Title
  tft.setTextColor(TEXT_COLOR);
  tft.setTextSize(2);
  tft.setCursor(10, 20);
  tft.print("Settings");
  
  // Option 1: Set Feeding Time
  tft.setTextSize(2);
  tft.setCursor(10, 60);
  tft.print("Feed Time:");
  
  // AM/PM buttons
  int ampmX = 170;
  int ampmY = 60;
  int ampmW = 60;
  int ampmH = 30;
  
  // Draw AM button
  drawAMPMButton(ampmX, ampmY, "AM", feedAM);
  
  // Draw PM button
  drawAMPMButton(ampmX, ampmY + ampmH + 10, "PM", !feedAM);
  
  // Feed hour (display in 12-hour format)
  char hourStr[3];
  sprintf(hourStr, "%2d", convertTo12Hour(feedHour));
  drawValueBox(10, 90, 60, 30, hourStr, BG_COLOR, TEXT_COLOR);
  drawArrowButton(10, 130, true, ACCENT_COLOR);   // Up arrow
  drawArrowButton(10, 180, false, ACCENT_COLOR); // Down arrow
  
  // Feed minute
  char minStr[3];
  sprintf(minStr, "%02d", feedMinute);
  drawValueBox(90, 90, 60, 30, minStr, BG_COLOR, TEXT_COLOR);
  drawArrowButton(90, 130, true, ACCENT_COLOR);   // Up arrow
  drawArrowButton(90, 180, false, ACCENT_COLOR); // Down arrow
  
  // Option 2: Food Amount
  tft.setTextSize(2);
  tft.setCursor(10, 230);
  tft.print("Food Amount:");
  
  // Food amount value (right of text)
  char amountStr[3];
  sprintf(amountStr, "%d", foodAmount);
  drawValueBox(170, 230, 40, 30, amountStr, BG_COLOR, TEXT_COLOR);
  
  // Food amount arrows (horizontal, beneath text)
  int arrowY = 260;
  int arrowSpacing = 60;
  
  // Left arrow (increase)
  drawArrowButton(10, arrowY, true, ACCENT_COLOR);
  
  // Right arrow (decrease)
  drawArrowButton(10 + arrowSpacing, arrowY, false, ACCENT_COLOR);
  
  // Back button
  drawButton(tft.width() - BACK_BUTTON_W - 10, tft.height() - BACK_BUTTON_H - 10, 
             BACK_BUTTON_W, BACK_BUTTON_H, "Back", ACCENT_COLOR, BUTTON_TEXT_COLOR, 2);
}

void setupTimeSettingScreen() {
  tft.fillScreen(BG_COLOR);
  
  // Title
  tft.setTextColor(TEXT_COLOR);
  tft.setTextSize(2);
  tft.setCursor(10, 20);
  tft.print("Set Current Time");
  
  // Get current time in 24-hour format
  currentHour = hour(currentTime);
  currentMinute = minute(currentTime);
  currentAM = currentHour < 12;
  
  // AM/PM buttons
  int ampmX = 170;
  int ampmY = 100;
  int ampmW = 60;
  int ampmH = 30;
  
  // Draw AM button
  drawAMPMButton(ampmX, ampmY, "AM", currentAM);
  
  // Draw PM button
  drawAMPMButton(ampmX, ampmY + ampmH + 10, "PM", !currentAM);
  
  // Current hour (display in 12-hour format)
  char hourStr[3];
  int displayHour = convertTo12Hour(currentHour);
  sprintf(hourStr, "%2d", displayHour);
  drawValueBox(10, 100, 60, 30, hourStr, BG_COLOR, TEXT_COLOR);
  drawArrowButton(10, 140, true, ACCENT_COLOR);   // Up arrow
  drawArrowButton(10, 200, false, ACCENT_COLOR); // Down arrow (increased spacing)
  
  // Current minute
  char minStr[3];
  sprintf(minStr, "%02d", currentMinute);
  drawValueBox(90, 100, 60, 30, minStr, BG_COLOR, TEXT_COLOR);
  drawArrowButton(90, 140, true, ACCENT_COLOR);   // Up arrow
  drawArrowButton(90, 200, false, ACCENT_COLOR); // Down arrow (increased spacing)
  
  // Back button (centered at bottom)
  drawButton((tft.width() - BACK_BUTTON_W) / 2, tft.height() - BACK_BUTTON_H - 10, 
             BACK_BUTTON_W, BACK_BUTTON_H, "Back", ACCENT_COLOR, BUTTON_TEXT_COLOR, 2);
}

void handleTouch() {
  static unsigned long lastTouchTime = 0;
  TOUCHINFO ti;
  if (touch.getSamples(&ti)) {
    int x = ti.x[0];
    int y = ti.y[0];
    Serial.printf("Touch x=%d y=%d\n", x, y);
    
    // Reset timeout timer on any touch
    lastTouchTime = millis();
    
    switch (currentScreen) {
      case MAIN_SCREEN:
        handleMainScreenTouch(x, y);
        break;
      case SETTINGS_SCREEN:
        handleSettingsScreenTouch(x, y);
        break;
      case TIME_SETTING_SCREEN:
        handleTimeSettingScreenTouch(x, y);
        break;
    }
    
    delay(100);  // Debounce
  }
}

void handleMainScreenTouch(int x, int y) {
  // Check if settings button was touched
  if (x > SETTINGS_BTN_X && x < SETTINGS_BTN_X + SETTINGS_BTN_W &&
      y > SETTINGS_BTN_Y && y < SETTINGS_BTN_Y + SETTINGS_BTN_H) {
    // Briefly show button press
    drawButton(SETTINGS_BTN_X, SETTINGS_BTN_Y, SETTINGS_BTN_W, SETTINGS_BTN_H, 
               "Settings", HIGHLIGHT_COLOR, BUTTON_TEXT_COLOR,2);
    delay(100);
    
    // Switch to settings screen
    currentScreen = SETTINGS_SCREEN;
    setupSettingsScreen();
    return;
  }
  
  // Check if set time button was touched
  if (x > SET_TIME_BTN_X && x < SET_TIME_BTN_X + SET_TIME_BTN_W &&
      y > SET_TIME_BTN_Y && y < SET_TIME_BTN_Y + SET_TIME_BTN_H) {
    // Briefly show button press
    drawButton(SET_TIME_BTN_X, SET_TIME_BTN_Y, SET_TIME_BTN_W, SET_TIME_BTN_H, 
               "Set Time", HIGHLIGHT_COLOR, BUTTON_TEXT_COLOR,2);
    delay(100);
    
    // Switch directly to time setting screen
    currentScreen = TIME_SETTING_SCREEN;
    setupTimeSettingScreen();
    return;
  }
}

void handleSettingsScreenTouch(int x, int y) {
  // Check for back button
  if (x > tft.width() - BACK_BUTTON_W - 10 && x < tft.width() - 10 &&
      y > tft.height() - BACK_BUTTON_H - 10 && y < tft.height() - 10) {
    // Return to main screen
    currentScreen = MAIN_SCREEN;
    setupMainScreen();
    return;
  }
  
  // Hour arrows
  if (x > 10 && x < 10 + ARROW_BTN_W) {
    // Up arrow for hour
    if (y > 130 && y < 130 + ARROW_BTN_H) {
      int newHour = (feedHour + 1) % 24; // 24-hour format
      if (newHour != feedHour) { // Only update if changed
        feedHour = newHour;
        feedAM = feedHour < 12; // Update AM/PM based on 24-hour time
        char hourStr[3];
        sprintf(hourStr, "%2d", convertTo12Hour(feedHour));
        drawValueBox(10, 90, 60, 30, hourStr, BG_COLOR, TEXT_COLOR);
        // Update AM/PM buttons if needed
        drawAMPMButton(170, 60, "AM", feedAM);
        drawAMPMButton(170, 100, "PM", !feedAM);
      }
    }
    // Down arrow for hour
    else if (y > 180 && y < 180 + ARROW_BTN_H) {
      int newHour = (feedHour + 23) % 24; // +23 is same as -1 for 0-23 range
      if (newHour != feedHour) { // Only update if changed
        feedHour = newHour;
        feedAM = feedHour < 12; // Update AM/PM based on 24-hour time
        char hourStr[3];
        sprintf(hourStr, "%2d", convertTo12Hour(feedHour));
        drawValueBox(10, 90, 60, 30, hourStr, BG_COLOR, TEXT_COLOR);
        // Update AM/PM buttons if needed
        drawAMPMButton(170, 60, "AM", feedAM);
        drawAMPMButton(170, 100, "PM", !feedAM);
      }
    }
  }
  
  // Minute arrows
  if (x > 90 && x < 90 + ARROW_BTN_W) {
    // Up arrow for minute
    if (y > 130 && y < 130 + ARROW_BTN_H) {
      int newMinute = (feedMinute + 1) % 60;
      if (newMinute != feedMinute) { // Only update if changed
        feedMinute = newMinute;
        char minStr[3];
        sprintf(minStr, "%02d", feedMinute);
        drawValueBox(90, 90, 60, 30, minStr, BG_COLOR, TEXT_COLOR);
      }
    }
    // Down arrow for minute
    else if (y > 180 && y < 180 + ARROW_BTN_H) {
      int newMinute = (feedMinute + 59) % 60; // +59 is same as -1 for 0-59 range
      if (newMinute != feedMinute) { // Only update if changed
        feedMinute = newMinute;
        char minStr[3];
        sprintf(minStr, "%02d", feedMinute);
        drawValueBox(90, 90, 60, 30, minStr, BG_COLOR, TEXT_COLOR);
      }
    }
  }
  
  // Food amount controls
  int arrowY = 260;
  int arrowSpacing = 60;
  
  if (y > arrowY && y < arrowY + ARROW_BTN_H) {
    // Left arrow (increase)
    if (x > 10 && x < 10 + ARROW_BTN_W) {
      int newAmount = min(foodAmount + 1, 7);
      if (newAmount != foodAmount) { // Only update if changed
        foodAmount = newAmount;
        char amountStr[3];
        sprintf(amountStr, "%d", foodAmount);
        drawValueBox(170, 230, 40, 30, amountStr, BG_COLOR, TEXT_COLOR);
      }
    }
    // Right arrow (decrease)
    else if (x > 10 + arrowSpacing && x < 10 + arrowSpacing + ARROW_BTN_W) {
      int newAmount = max(foodAmount - 1, 1);
      if (newAmount != foodAmount) { // Only update if changed
        foodAmount = newAmount;
        char amountStr[3];
        sprintf(amountStr, "%d", foodAmount);
        drawValueBox(170, 230, 40, 30, amountStr, BG_COLOR, TEXT_COLOR);
      }
    }
  }
}

void handleTimeSettingScreenTouch(int x, int y) {
  // Check for back button
  if (x > (tft.width() - BACK_BUTTON_W) / 2 && x < (tft.width() + BACK_BUTTON_W) / 2 &&
      y > tft.height() - BACK_BUTTON_H - 10 && y < tft.height() - 10) {
    // Return to main screen
    currentScreen = MAIN_SCREEN;
    setupMainScreen();
    return;
  }
  
  // Hour arrows
  if (x > 10 && x < 10 + ARROW_BTN_W) {
    // Up arrow for hour
    if (y > 140 && y < 140 + ARROW_BTN_H) {
      int newHour = (currentHour + 1) % 24; // 24-hour format
      if (newHour != currentHour) { // Only update if changed
        currentHour = newHour;
        currentAM = currentHour < 12; // Update AM/PM based on 24-hour time
        char hourStr[3];
        sprintf(hourStr, "%2d", convertTo12Hour(currentHour));
        drawValueBox(10, 100, 60, 30, hourStr, BG_COLOR, TEXT_COLOR);
        // Update AM/PM buttons if needed
        drawAMPMButton(170, 100, "AM", currentAM);
        drawAMPMButton(170, 140, "PM", !currentAM);
        setCurrentTime(currentHour, currentMinute, 0, currentAM);
      }
    }
    // Down arrow for hour
    else if (y > 200 && y < 200 + ARROW_BTN_H) {
      int newHour = (currentHour + 23) % 24; // +23 is same as -1 for 0-23 range
      if (newHour != currentHour) { // Only update if changed
        currentHour = newHour;
        currentAM = currentHour < 12; // Update AM/PM based on 24-hour time
        char hourStr[3];
        sprintf(hourStr, "%2d", convertTo12Hour(currentHour));
        drawValueBox(10, 100, 60, 30, hourStr, BG_COLOR, TEXT_COLOR);
        // Update AM/PM buttons if needed
        drawAMPMButton(170, 100, "AM", currentAM);
        drawAMPMButton(170, 140, "PM", !currentAM);
        setCurrentTime(currentHour, currentMinute, 0, currentAM);
      }
    }
  }
  
  // Minute arrows
  if (x > 90 && x < 90 + ARROW_BTN_W) {
    // Up arrow for minute
    if (y > 140 && y < 140 + ARROW_BTN_H) {
      int newMinute = (currentMinute + 1) % 60;
      if (newMinute != currentMinute) { // Only update if changed
        currentMinute = newMinute;
        char minStr[3];
        sprintf(minStr, "%02d", currentMinute);
        drawValueBox(90, 100, 60, 30, minStr, BG_COLOR, TEXT_COLOR);
        setCurrentTime(currentHour, currentMinute, 0, currentAM);
      }
    }
    // Down arrow for minute
    else if (y > 200 && y < 200 + ARROW_BTN_H) {
      int newMinute = (currentMinute + 59) % 60; // +59 is same as -1 for 0-59 range
      if (newMinute != currentMinute) { // Only update if changed
        currentMinute = newMinute;
        char minStr[3];
        sprintf(minStr, "%02d", currentMinute);
        drawValueBox(90, 100, 60, 30, minStr, BG_COLOR, TEXT_COLOR);
        setCurrentTime(currentHour, currentMinute, 0, currentAM);
      }
    }
  }
}

void updateFeedingTime(int hour, int minute, bool isAM) {
  feedHour = convertTo24Hour(hour, isAM);
  feedMinute = minute;
  feedAM = isAM;
}

void setCurrentTime(int hour, int minute, int second, bool isAM) {
  // Convert from 12-hour to 24-hour if needed
  int hour24 = hour;
  if (use12HourFormat) {
    hour24 = convertTo24Hour(hour, isAM);
  }
  
  setTime(hour24, minute, second, day(), month(), year());
  Serial.printf("Time set to: %02d:%02d:%02d\n", hour24, minute, second);
}

void activateServo() {
  // This would control the servo in a real implementation
  Serial.println("FEEDING TIME! Activating servo...");
  Serial.printf("Dispensing food: %d%% of maximum amount\n", foodAmount);
  
  // TODO: Implement servo control code here
  
  // Visual feedback that feeding occurred
  int notificationY = 20;  // Moved to top
  tft.fillRect(0, notificationY, tft.width(), 30, ACCENT_COLOR);  // Changed to ACCENT_COLOR
  tft.setTextColor(TEXT_COLOR, ACCENT_COLOR);
  tft.setTextSize(2);
  
  const char* feedMsg = "FEEDING NOW";
  int textWidth = strlen(feedMsg) * 12; // Approximate for text size 2
  tft.setCursor((tft.width() - textWidth) / 2, notificationY + 5);
  tft.print(feedMsg);
  
  delay(1000);
  
  // Clear notification and force redraw of main screen
  currentScreen = MAIN_SCREEN;
  setupMainScreen();
}

// Helper functions for time conversion
const char* formatHour(int h, bool use12Hour) {
  static char hourStr[3];
  
  if (use12Hour) {
    if (h == 0) {
      strcpy(hourStr, "12");
    } else if (h > 12) {
      sprintf(hourStr, "%2d", h - 12);
    } else {
      sprintf(hourStr, "%2d", h);
    }
  } else {
    sprintf(hourStr, "%02d", h);
  }
  
  return hourStr;
}

// Helper function to check if a given hour is AM
bool isAM(int hour) {
  return hour < 12;
}

// Convert 12-hour format to 24-hour format
int convertTo24Hour(int hour, bool isAM) {
  if (isAM) {
    return (hour == 12) ? 0 : hour;
  } else {
    return (hour == 12) ? 12 : hour + 12;
  }
}

// Convert 24-hour format to 12-hour format
int convertTo12Hour(int hour) {
  if (hour == 0) return 12;
  if (hour > 12) return hour - 12;
  return hour;
}

// Helper function to get current time components
void getTimeComponents(int &outHour, int &outMinute, int &outSecond, bool &isAMFlag) {
  outHour = hour(currentTime);
  outMinute = minute(currentTime);
  outSecond = second(currentTime);
  isAMFlag = (outHour < 12);
}

// Helper function to calculate time until next feeding
void calculateTimeUntilFeeding(int &hours, int &minutes) {
  int currentHour = hour(currentTime);
  int currentMinute = minute(currentTime);
  
  int currentMinutes = currentHour * 60 + currentMinute;
  int feedMinutes = feedHour * 60 + feedMinute;
  
  int minutesToFeed;
  if (feedMinutes > currentMinutes) {
    minutesToFeed = feedMinutes - currentMinutes;
  } else {
    minutesToFeed = (24 * 60) - currentMinutes + feedMinutes;
  }
  
  hours = minutesToFeed / 60;
  minutes = minutesToFeed % 60;
}

// Helper function to check if it's feeding time
bool isFeedingTime() {
  return (hour(currentTime) == feedHour && 
          minute(currentTime) == feedMinute && 
          second(currentTime) == 0);
}

// Helper function to update display with minimal redraw
void updateDisplayRegion(int x, int y, int w, int h, uint16_t color) {
  tft.setWindow(x, y, x + w - 1, y + h - 1);
  tft.startWrite();
  for (int i = 0; i < w * h; i++) {
    tft.pushColor(color);
  }
  tft.endWrite();
}

// Helper function to draw text with minimal redraw
void drawTextWithBackground(int x, int y, const char* text, uint16_t textColor, uint16_t bgColor, int textSize) {
  tft.setTextSize(textSize);
  tft.setTextColor(textColor, bgColor);
  tft.setCursor(x, y);

  // Calculate text dimensions using TFT_eSPI-compatible methods
  uint16_t w = tft.textWidth(text);
  uint16_t h = tft.fontHeight();

  // Clear background
  updateDisplayRegion(x, y, w, h, bgColor);

  // Draw text
  tft.print(text);
}

// Helper function to handle button press animation
void animateButton(int x, int y, int w, int h, const char* label, uint16_t bgColor, uint16_t textColor) {
  // Draw pressed state
  drawButton(x, y, w, h, label, HIGHLIGHT_COLOR, textColor,2);
  delay(100);
  
  // Draw normal state
  drawButton(x, y, w, h, label, bgColor, textColor,2);
}

// New function to draw AM/PM button with highlight
void drawAMPMButton(int x, int y, const char* label, bool isSelected) {
  // Draw button background
  tft.fillRect(x, y, 60, 30, BG_COLOR);
  
  // Draw border (always same thickness)
  tft.drawRect(x, y, 60, 30, TEXT_COLOR);
  
  // Draw text (red if selected)
  tft.setTextColor(isSelected ? HIGHLIGHT_COLOR : TEXT_COLOR);
  tft.setTextSize(2);
  
  // Center text in button
  int textWidth = strlen(label) * 12; // Approximate for text size 2
  int textX = x + (60 - textWidth) / 2;
  int textY = y + (30 - 16) / 2;
  
  tft.setCursor(textX, textY);
  tft.print(label);
}
