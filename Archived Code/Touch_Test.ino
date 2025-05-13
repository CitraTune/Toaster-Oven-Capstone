#include <TFT_eSPI.h>
#include <Wire.h>
#include <bb_captouch.h>

// Touch config for CYD_543 (your setup)
// #define TOUCH_SDA 8
// #define TOUCH_SCL 4
// #define TOUCH_INT 3
// #define TOUCH_RST 38
// #ifdef CYD_28C
#define TOUCH_SDA 33
#define TOUCH_SCL 32
#define TOUCH_INT 21
#define TOUCH_RST 25
#define LCD DISPLAY_CYD
// #endif

TFT_eSPI tft = TFT_eSPI();
BBCapTouch touch;

bool touchedState = false;

// Button area
#define BUTTON_X 40
#define BUTTON_Y 120
#define BUTTON_W 160
#define BUTTON_H 50

void drawButton(const char *label, uint16_t bgColor) {
  tft.fillRect(BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H, bgColor);
  tft.drawRect(BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H, TFT_WHITE);
  tft.setTextColor(TFT_WHITE, bgColor);
  tft.setTextSize(2);
  tft.setCursor(BUTTON_X + 10, BUTTON_Y + 15);
  tft.print(label);
}

void setup() {
  Serial.begin(115200);
  Wire.begin(TOUCH_SDA, TOUCH_SCL);
  delay(300);  // Let power stabilize

  tft.setup();
  tft.setRotation(0); // Portrait
  tft.fillScreen(TFT_BLACK);
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  // setup touch
  touch.setup(TOUCH_SDA, TOUCH_SCL, TOUCH_RST, TOUCH_INT);
  Serial.printf("Touch sensor type: %d\n", touch.sensorType());

  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(2);
  tft.setCursor(10, 20);
  tft.println("Touch Test");

  drawButton("Touch me", TFT_BLUE);
}

void loop() {
  TOUCHINFO ti;
  if (touch.getSamples(&ti)) {
    int x = ti.x[0];
    int y = ti.y[0];
    Serial.printf("Touch x=%d y=%d\n", x, y);

    if (x > BUTTON_X && x < BUTTON_X + BUTTON_W &&
        y > BUTTON_Y && y < BUTTON_Y + BUTTON_H) {
      if (!touchedState) {
        drawButton("Touched!", TFT_RED);
        touchedState = true;
      }
    } else {
      if (touchedState) {
        drawButton("Touch me", TFT_BLUE);
        touchedState = false;
      }
    }

    delay(200);  // Debounce
  }
}
