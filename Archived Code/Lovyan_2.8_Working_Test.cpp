#include <Arduino.h>
#include "LGFX_Config.h"

LGFX myTFT;

void setup() {
  Serial.begin(115200);
  myTFT.begin();
  myTFT.setRotation(7);
           // ← This one flips both X and Y axes
  myTFT.invertDisplay(true);
  myTFT.setBrightness(128);
  myTFT.setColorDepth(16);

  myTFT.fillScreen(TFT_BLACK);
  myTFT.setTextColor(TFT_WHITE);
  myTFT.setTextSize(2);
  myTFT.drawString("Display OK", 40, 100);
  delay(3000);
  myTFT.fillScreen(TFT_RED);
  delay(1000);
  myTFT.fillScreen(TFT_GREEN);
  delay(1000);
  myTFT.fillScreen(TFT_BLUE);
  delay(1000);
  myTFT.fillScreen(TFT_BLACK);
  delay(3000);
  myTFT.drawString("Display OK", 40, 100);
  
}

void loop() {
  // Nothing here yet
}
