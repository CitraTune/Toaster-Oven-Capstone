#pragma once

#include "LineArtManager.hpp"

class LineArtSetup
{
public:
    static void setupAllLineArt()
    {
        setupMainScreenGraphs();
        addGraphTickMarks();
        addDividerLines();
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
        const int yOffset = 8;   // Shift everything down by 8 pixels
        const int xOffset = -4;  // Shift graph left by 4 pixels
        const int graphX = 40 + xOffset;  // X position with offset
        const int graphY = 55 + yOffset;  // Y position with offset
        const int squareSize = 15;  // Size of each square
        const int graphWidth = squareSize * 12 + 1;  // Width for 12 columns
        const int graphHeight = squareSize * 10 + 1;  // Height for 10 rows

        // Create the graph with black lines and gray background
        LineArtManager::addGraph(
            SCREEN_MAIN,        // Screen ID
            graphX, graphY,     // Position
            graphWidth, graphHeight, // Dimensions
            9, 11,             // 11 horizontal lines, 13 vertical lines (internal grid lines)
            TFT_BLACK,          // Frame color
            TFT_BLACK,          // Grid color
            TFT_LIGHTGRAY       // Background color
        );

        // Dark gray outline options (pick one):
        // 0x4208 - Very dark gray (close to black)
        // 0x5A69 - Dark slate gray
        // 0x6B4D - Medium-dark gray
        // 0x7BEF - Medium gray
        // 0x8C71 - Standard dark gray
        // 0x9CD3 - Slightly darker than TFT_DARKGRAY

        // Using the very dark gray option (0x4208)
        LineArtManager::addRect(
                SCREEN_MAIN,
            graphX, graphY,
            graphWidth, graphHeight,
            0x20E4  // Very dark gray, almost black
            );
        }

    static void addGraphTickMarks()
    {
        // Define the graph position and dimensions (must match what's in setupMainScreenGraphs)
        const int yOffset = 8;   // Shift everything down by 8 pixels
        const int xOffset = -4;  // Shift graph left by 4 pixels
        const int graphX = 40 + xOffset;  // X position with offset
        const int graphY = 55 + yOffset;  // Y position with offset
        const int squareSize = 15;  // Size of each square
        const int graphHeight = squareSize * 10;  // Height for 10 rows
        const int graphWidth = squareSize * 12;   // Width for 12 columns

        // Y-axis tick mark properties
        const int yTickWidth = 4;
        const int yTickHeight = 1;
        const int tickOffset = 6; // How much the tick extends left of the graph

        // We have 6 labels: 0, 50, 100, 150, 200, 250
        const int labelCount = 6;

        // Add tick marks for each temperature label
        for (int i = 0; i < labelCount; i++) {
            // Calculate Y position: map i from [0,5] to [graphBottom,graphTop]
            int y = (graphY + graphHeight) - (i * (graphHeight) / (labelCount - 1));

            // Add a red tick mark at this position
            // The tick will be positioned just to the left of the graph
            LineArtManager::addFilledRect(
            SCREEN_MAIN,
                graphX-2,  // X position (left of graph)
                y, // Y position (centered on the grid line)
                yTickWidth,           // Width
                yTickHeight,          // Height
                TFT_RED               // Color
        );
        }

        // X-axis tick mark properties
        const int xTickWidth = 1;
        const int xTickHeight = 4;

        // We'll have 6 time labels: 1, 2, 3, 4, 5, 6
        const int timeLabelsCount = 6;

        // Add tick marks for each time label, including both ends of the graph
        for (int i = 0; i <= timeLabelsCount; i++) {
            // Calculate X position: evenly distribute across the graph width
            // We want i=0 to be at the left edge and i=6 at the right edge
            int x = graphX + (i * graphWidth / timeLabelsCount);

            // Add a tick mark at this position
            LineArtManager::addFilledRect(
            SCREEN_MAIN,
                x,                   // X position (centered on grid line)
                graphY + graphHeight, // Y position (below graph)
                xTickWidth,           // Width
                xTickHeight,          // Height
                TFT_RED              // Color
        );
        }
    }

    static void addDividerLines()
    {
        // Add a divider line below the "Main Menu" text on main screen
        // The Main Menu text is at y=24, so we'll place this a few pixels below
        LineArtManager::addLine(
            SCREEN_MAIN,
            0, 45,             // Start point (left edge of screen, y=40)
            SCREEN_WIDTH, 45,  // End point (right edge of screen, same y)
            TFT_WHITE          // Color
        );

        // LineArtManager::addLine(
        //     SCREEN_MAIN,
        //     0, 20,             // Start point (left edge of screen, y=40)
        //     SCREEN_WIDTH, 20,  // End point (right edge of screen, same y)
        //     TFT_WHITE          // Color
        // );

        // Add a divider line above the buttons on main screen
        // Assuming buttons are at the bottom of the screen, we'll place this line appropriately
        // Typically buttons might be around y=200-220 on a 240px height screen
        LineArtManager::addLine(
            SCREEN_MAIN,
            0, 250,             // Start point (left edge of screen, y=200)
            SCREEN_WIDTH, 250,  // End point (right edge of screen, same y)
            TFT_WHITE           // Color
        );
    }

    static void activateCooldownScreen()
    {
        // Add light green background for cooldown screen
        LineArtManager::addFilledRect(SCREEN_COOLDOWN, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, TFT_GREENYELLOW);
    }
};

