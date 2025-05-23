#include "LineArtManager.hpp"
#include "LGFX_Config.h"

class LineArtSetup
{
public:
    static void setupAllLineArt()
    {
        setupMainScreenGraphs();
        setupMainScreenDivider();
    }

private:
    static void setupMainScreenGraphs()
    {
        // Updated position - moved up by 8 pixels and right by 12 pixels
        // Make sure the graph dimensions align with the calculations in TextSetup
        const int yOffset = -8;
        const int xOffset = 12;
        const int graphX = 40 + xOffset;
        const int graphY = 55 + yOffset;
        LineArtManager::addGraph(SCREEN_MAIN, graphX, graphY, SCREEN_WIDTH - 30, 160, 11, 14, TFT_BLACK, TFT_BLACK, TFT_LIGHTGRAY);
    }

    static void setupMainScreenDivider()
    {
        // Add white line beneath the "Main Menu" label
        LineArtManager::addLine(SCREEN_MAIN, 0, 47, SCREEN_WIDTH, 47, TFT_WHITE);
    }

    static void activateCooldownScreen()
    {
        // Add light green background for cooldown screen
        LineArtManager::addFilledRect(SCREEN_COOLDOWN, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, TFT_GREENYELLOW);
    }
};
