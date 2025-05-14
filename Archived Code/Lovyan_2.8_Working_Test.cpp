#include <Arduino.h>
#include "LGFX_Config.h"

LGFX display;

void setup() {
  Serial.begin(115200);
  display.begin();
  display.setRotation(7);
           // ← This one flips both X and Y axes
  display.invertDisplay(true);
  display.setBrightness(128);
  display.setColorDepth(16);

  display.fillScreen(TFT_BLACK);
  display.setTextColor(TFT_WHITE);
  display.setTextSize(2);
  display.drawString("Display OK", 40, 100);
  delay(3000);
  display.fillScreen(TFT_RED);
  delay(1000);
  display.fillScreen(TFT_GREEN);
  delay(1000);
  display.fillScreen(TFT_BLUE);
  delay(1000);
  display.fillScreen(TFT_BLACK);
  delay(3000);
  display.drawString("Display OK", 40, 100);
  
}

void loop() {
  // Nothing here yet
}
