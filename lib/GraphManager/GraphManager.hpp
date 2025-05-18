#pragma once

#include <Arduino.h>
#include "LGFX_Config.h"
#include "UIManager.hpp"

class GraphManager {
public:
    static bool isVisibleOnScreen(int screenId) {
        return screenId == SCREEN_MAIN;
    }

    static void draw() {
        int graphX = 20;
        int graphY = 50;
        int graphWidth = SCREEN_WIDTH - 40;
        int graphHeight = 120;
        drawGraphElements(graphX, graphY, graphWidth, graphHeight);
    }

private:
    static void drawGraphElements(int graphX, int graphY, int graphWidth, int graphHeight) {
        UIManager::display().fillRect(graphX, graphY, graphWidth, graphHeight, TFT_LIGHTGRAY);
        UIManager::display().drawRect(graphX, graphY, graphWidth, graphHeight, TFT_BLACK);

        int horizontalSpacing = graphWidth / 10;
        int verticalSpacing = graphHeight / 6;

        for (int i = 1; i < 10; i++) {
            UIManager::display().drawLine(graphX + i * horizontalSpacing, graphY,
                         graphX + i * horizontalSpacing, graphY + graphHeight,
                         TFT_BLACK);
        }

        for (int i = 1; i < 6; i++) {
            UIManager::display().drawLine(graphX, graphY + i * verticalSpacing,
                         graphX + graphWidth, graphY + i * verticalSpacing,
                         TFT_BLACK);
        }
    }
};
