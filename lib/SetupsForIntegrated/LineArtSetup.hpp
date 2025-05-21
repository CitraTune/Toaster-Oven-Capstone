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
        // Add main screen graph
        LineArtManager::addGraph(SCREEN_MAIN, 20, 50, SCREEN_WIDTH - 40, 120, 5, 9, TFT_BLACK, TFT_BLACK, TFT_DARKGRAY);
    }

    static void setupCooldownScreen()
    {
        // Add light green background for cooldown screen
        LineArtManager::addFilledRect(SCREEN_COOLDOWN, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, TFT_GREENYELLOW);
    }
};
