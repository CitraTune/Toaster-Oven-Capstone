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
        // Create a graph with 12x14 perfect squares
        // For internal grid lines only (not counting the outer frame):
        // 12 rows need 11 horizontal internal grid lines
        // 14 columns need 13 vertical internal grid lines

        // Define the graph position and size
        // Assuming SCREEN_MAIN is defined elsewhere
        const int graphX = 40;  // X position
        const int graphY = 55;  // Y position
        const int squareSize = 14;  // Size of each square (14 pixels)
        const int graphWidth = squareSize * 14;  // Width for 14 columns
        const int graphHeight = squareSize * 12;  // Height for 12 rows

        // Create the graph with black lines and gray background
        LineArtManager::addGraph(
            SCREEN_MAIN,        // Screen ID
            graphX, graphY,     // Position
            graphWidth, graphHeight, // Dimensions
            11, 13,             // 11 horizontal lines, 13 vertical lines (internal grid lines)
            TFT_BLACK,          // Frame color
            TFT_BLACK,          // Grid color
            TFT_LIGHTGRAY       // Background color
        );
    }

    static void activateCooldownScreen()
    {
        // Add light green background for cooldown screen
        LineArtManager::addFilledRect(SCREEN_COOLDOWN, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, TFT_GREENYELLOW);
    }

};

