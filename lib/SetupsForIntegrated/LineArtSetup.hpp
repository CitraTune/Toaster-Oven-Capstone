#pragma once

#include "LineArtManager.hpp"

class LineArtSetup
{
public:
    static void setupAllLineArt()
    {
        setupMainScreenGraphs();
    }

private:
    static void setupMainScreenGraphs()
    {
        // Add main screen graph with no right margin and 5 horizontal lines (5 intervals)
        // X-axis reduced to 9 divisions, larger size
        LineArtManager::addGraph(SCREEN_MAIN, 20, 50, SCREEN_WIDTH - 20, 150, 5, 9, TFT_BLACK, TFT_BLACK, TFT_RED);
    }

    static void activateCooldownScreen()
    {
        // Add light green background for cooldown screen
        LineArtManager::addFilledRect(SCREEN_COOLDOWN, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, TFT_GREENYELLOW);
    }
};
