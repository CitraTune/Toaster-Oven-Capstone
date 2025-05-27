#pragma once

#include "LineArtManager.hpp"
#include "TempManager.hpp"
#include <Arduino.h>

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

    // Graph update variables
    static unsigned long lastGraphUpdateTime;
    static constexpr unsigned long graphUpdateInterval = 5000; // 5 seconds in milliseconds
    static int graphPointCount;
    static constexpr int maxTempValue = 250; // Maximum temperature on graph (y-axis)
    static constexpr int maxTimeValue = 6;   // Maximum time on graph (x-axis)
    
    // Variables to track the previous point for line drawing
    static int lastPointX;
    static int lastPointY;
    static bool hasLastPoint;
    static float startTemperature;
    static void setupAllLineArt();
    static void updateGraph(unsigned long currentMillis, bool reflowActive);
    static void resetGraph(); // Just the declaration here
private:
    static void setupMainScreenGraphs();
    static void addGraphTickMarks();
    static void addDividerLines();
    static void setupCooldownScreen();
    static void setupTemperatureTable();
    static int calculateXPosition(int timePoint);
    static int calculateYPosition(float temperature);
};

