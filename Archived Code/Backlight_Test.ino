#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();  // Create TFT object

#define TFT_BL     21  // Test backlight on GPIO 21

void setup() {
  Serial.begin(115200);

  // Setup backlight pin
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH); // Try to turn on backlight

  // Setup TFT
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(30, 100);
  tft.println("Hello from GPIO 21!");
}

void loop() {
  // Nothing needed here
}
// this just checks which pin was the backlight pin, 21 or 27. it was 27.
