#include <TFT_eSPI.h>
#include <bb_captouch.h>

TFT_eSPI tft = TFT_eSPI();
bb_captouch touch;

#define BUTTON_X 60
#define BUTTON_Y 100
#define BUTTON_W 120
#define BUTTON_H 50

bool touchedState = false;

void drawButton(const char *label, uint16_t bgColor) {
  tft.fillRect(BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H, bgColor);
  tft.drawRect(BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H, TFT_WHITE);
  tft.setTextColor(TFT_WHITE, bgColor);
  tft.setTextSize(2);
  int16_t x = BUTTON_X + 10;
  int16_t y = BUTTON_Y + (BUTTON_H / 2) - 8;
  tft.setCursor(x, y);
  tft.print(label);
}

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(0); // Portrait
  tft.fillScreen(TFT_BLACK);

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  // Initialize touch
  if (!touch.begin()) {
    Serial.println("Touch init failed!");
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_RED);
    tft.print("Touch init failed!");
  } else {
    Serial.println("Touch OK");
  }

  drawButton("Touch me", TFT_BLUE);
}

void loop() {
  if (touch.touched()) {
    int x, y;
    touch.getPoint(&x, &y);


    Serial.printf("Touch: %d, %d\n", x, y);

    if (x > BUTTON_X && x < BUTTON_X + BUTTON_W &&
        y > BUTTON_Y && y < BUTTON_Y + BUTTON_H) {
      if (!touchedState) {
        touchedState = true;
        drawButton("Touched!", TFT_RED);
      }
    } else if (touchedState) {
      touchedState = false;
      drawButton("Touch me", TFT_BLUE);
    }

    delay(200); // debounce touch
  }
}
