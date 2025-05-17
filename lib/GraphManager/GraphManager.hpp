#pragma once

#include <Arduino.h>
#include "LGFX_Config.h"

class GraphManager {
public:
    // Constructor
    GraphManager(LGFX& tft);
    
    // Draw the graph
    void draw();
    
    // Check if graph should be visible on current screen
    bool isVisibleOnScreen(int screenId);

private:
    LGFX& _tft;
    
    // Internal method to draw graph elements
    void drawGraphElements(int graphX, int graphY, int graphWidth, int graphHeight);
};

