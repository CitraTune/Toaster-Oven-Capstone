#include <TFT_eSPI.h> // Include the graphics library
#include <SPI.h>      // SPI is required for TFT_eSPI


TFT_eSPI tft = TFT_eSPI();  // Create TFT object


void setup() {
  Serial.begin(115200);
  tft.init();               // Initialize the display
  tft.setRotation(1);       // Landscape orientation. Use 0–3 to rotate.
  tft.fillScreen(TFT_BLACK); // Clear the screen to black


  // Backlight pin setup (if using software control)
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, TFT_BACKLIGHT_ON); // Turn on backlight


  // Set text properties
  tft.setTextColor(TFT_WHITE, TFT_BLACK); // White text on black background
  tft.setTextSize(2);                     // Scale text 2x
  tft.setCursor(20, 100);                 // Set position
  tft.println("Hello, World!");
}


void loop() {
  // Nothing here
}


