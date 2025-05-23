#pragma once

#include "LineArtManager.hpp"

class LineArtSetup
{
public:
    // Constants for graph positioning
    static constexpr int yOffset = 18;
    static constexpr int xOffset = -4;
    static constexpr int baseGraphX = 40;
    static constexpr int baseGraphY = 55;
    static constexpr int graphX = baseGraphX + xOffset;
    static constexpr int graphY = baseGraphY + yOffset;
    static constexpr int squareSize = 15;
    static constexpr int graphWidth = squareSize * 12 + 1;
    static constexpr int graphHeight = squareSize * 10 + 1;

    static void setupAllLineArt()
    {
        setupMainScreenGraphs();
        addGraphTickMarks();
        addDividerLines();
    }

private:
    static void setupMainScreenGraphs()
    {
        LineArtManager::addGraph(
            SCREEN_MAIN,
            graphX, graphY,
            graphWidth, graphHeight,
            9, 11,
            TFT_BLACK,
            TFT_BLACK,
            TFT_LIGHTGRAY
        );

        LineArtManager::addRect(
                SCREEN_MAIN,
            graphX, graphY,
            graphWidth, graphHeight,
            0x20E4  
            );
        }

    static void addGraphTickMarks()
    {
        // Y-axis tick marks
        const int yTickWidth = 4;
        const int yTickHeight = 1;
        const int labelCount = 6;  // 0, 50, 100, 150, 200, 250
        for (int i = 0; i < labelCount; i++) {
            int y = (graphY + graphHeight) - (i * (graphHeight) / (labelCount - 1));
            LineArtManager::addFilledRect(
            SCREEN_MAIN,
                graphX-2,
                y,
                yTickWidth,
                yTickHeight,
                TFT_RED
        );
        }

        // X-axis tick marks
        const int xTickWidth = 1;
        const int xTickHeight = 4;
        const int timeLabelsCount = 6;  // 1, 2, 3, 4, 5, 6

        for (int i = 0; i <= timeLabelsCount; i++) {
            int x = graphX + (i * graphWidth / timeLabelsCount);
            LineArtManager::addFilledRect(
            SCREEN_MAIN,
                x,
                graphY + graphHeight,
                xTickWidth,
                xTickHeight,
                TFT_RED
        );
    }
    }

    static void addDividerLines()
    {
        // Divider line below the "Main Menu" text
        LineArtManager::addLine(
            SCREEN_MAIN,
            0, 45,
            SCREEN_WIDTH, 45,
            TFT_WHITE
        );

        // Divider line above the buttons
        LineArtManager::addLine(
            SCREEN_MAIN,
            0, 263,
            SCREEN_WIDTH, 263,
            TFT_WHITE
        );
    }

    static void setupCooldownScreen()
    {
        LineArtManager::addFilledRect(SCREEN_COOLDOWN, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, TFT_GREENYELLOW);
    }
};

