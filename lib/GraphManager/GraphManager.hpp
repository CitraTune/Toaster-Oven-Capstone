#pragma once

#include <Arduino.h>
#include "LGFX_Config.h"

class GraphManager {
public:
  // Constructor
  GraphManager(LGFX& tft);
  
  // Toggle between full screen and normal graph
  void toggleFullScreen();

  // Draw the graph (full draw including background clear)
  void draw(bool lightMode);
  
  // Just redraw the graph data without clearing background
  void redrawGraphOnly(bool lightMode);

  // Check if this graph should be shown on the current screen
  bool isVisibleOnScreen(int screenId);

  // Set which screen(s) the graph should appear on
  void setVisibleScreens(int* screens, int count);

  // Current state
  bool fullScreen;

private:
  LGFX& _tft;
  int* visibleScreens;
  int screenCount;

  // Internal method to draw graph elements
  void drawGraphElements(int graphX, int graphY, int graphWidth, int graphHeight, uint16_t graphColor);
};

