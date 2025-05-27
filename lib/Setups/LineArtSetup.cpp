#include "LineArtSetup.hpp"

// Initialize static member variables
unsigned long LineArtSetup::lastGraphUpdateTime = 0;
int LineArtSetup::graphPointCount = 0;
int LineArtSetup::lastPointX = 0;
int LineArtSetup::lastPointY = 0;
bool LineArtSetup::hasLastPoint = false;
float LineArtSetup::startTemperature = 0.0f;

void LineArtSetup::setupAllLineArt()
{
    setupMainScreenGraphs();
    addGraphTickMarks();
    addDividerLines();
    setupTemperatureTable();

    // Initialize graph update variables
    lastGraphUpdateTime = 0;
    graphPointCount = 0;
    hasLastPoint = false;
    startTemperature = 0.0f;
}

void LineArtSetup::updateGraph(unsigned long currentMillis, bool reflowActive)
{
    // Only update if reflow is active and enough time has passed since last update
    if (reflowActive && (currentMillis - lastGraphUpdateTime >= graphUpdateInterval))
    {
        // Get current temperature from TempManager
        float currentTemp = TempManager::getTemperature();

        // Calculate current point position
        int currentPointX = calculateXPosition(graphPointCount);
        int currentPointY = calculateYPosition(currentTemp);

        // Define square point size
        const int pointSize = 3;

        // Special handling for the first point
        if (graphPointCount == 0)
        {
            // Store the starting temperature
            startTemperature = currentTemp;

            // For the first point, draw a line from the origin (0,0 of the graph)
            // to the current temperature position
            int originX = graphX;
            int originY = graphY + graphHeight; // Bottom-left corner of the graph

            // Draw the initial line from origin to first reading
            LineArtManager::addLine(
                SCREEN_MAIN,
                originX, currentPointY,
                currentPointX, currentPointY,
                TFT_RED);

            hasLastPoint = true;
        }
        // For all subsequent points, connect to the previous point
        else if (hasLastPoint)
        {
            LineArtManager::addLine(
                SCREEN_MAIN,
                lastPointX, lastPointY,
                currentPointX, currentPointY,
                TFT_RED);
        }
        LineArtManager::updateActiveState(SCREEN_MAIN);
        LineArtManager::draw();

        // Draw a small square at the current temperature point
        LineArtManager::addFilledRect(
            SCREEN_MAIN,
            currentPointX - (pointSize / 2),
            currentPointY - (pointSize / 2),
            pointSize,
            pointSize,
            TFT_RED);

        // Store the current point as the last point for the next update
        lastPointX = currentPointX;
        lastPointY = currentPointY;

        // Update tracking variables
        lastGraphUpdateTime = currentMillis;
        graphPointCount++;
    }
}

void LineArtSetup::resetGraph()
{
    graphPointCount = 0;
    hasLastPoint = false;
    startTemperature = 0.0f;

    // Redraw the empty graph
    setupMainScreenGraphs();
    addGraphTickMarks();
}

int LineArtSetup::calculateXPosition(int timePoint)
{
    // For 5-second intervals, each point represents 5/60 = 1/12 of a minute
    float minutesElapsed = (timePoint + 1) * (1.0f / 12.0f); // +1 to start at 5sec instead of 0sec

    // Scale to fit within graph width
    float timeRatio = min(minutesElapsed / maxTimeValue, 1.0f);

    // Map ratio to graph coordinates
    return graphX + (timeRatio * graphWidth);
}

int LineArtSetup::calculateYPosition(float temperature)
{
    // Convert temperature to y-coordinate on graph
    // Scale to fit within graph height, invert because y-axis grows downward
    float tempRatio = min(temperature / maxTempValue, 1.0f);
    return (graphY + graphHeight) - (tempRatio * graphHeight);
}

void LineArtSetup::setupMainScreenGraphs()
{
    LineArtManager::addGraph(
        SCREEN_MAIN,
        graphX, graphY,
        graphWidth, graphHeight,
        9, 11,
        TFT_BLACK,
        TFT_BLACK,
        TFT_LIGHTGRAY);

    LineArtManager::addRect(
        SCREEN_MAIN,
        graphX, graphY,
        graphWidth, graphHeight,
        0x20E4);
}

void LineArtSetup::addGraphTickMarks()
{
    // Y-axis tick marks
    const int yTickWidth = 4;
    const int yTickHeight = 1;
    const int labelCount = 6; // 0, 50, 100, 150, 200, 250
    for (int i = 0; i < labelCount; i++)
    {
        int y = (graphY + graphHeight) - (i * (graphHeight) / (labelCount - 1));
        LineArtManager::addFilledRect(
            SCREEN_MAIN,
            graphX - 2,
            y,
            yTickWidth,
            yTickHeight,
            TFT_RED);
    }

    // X-axis tick marks
    const int xTickWidth = 1;
    const int xTickHeight = 4;
    const int timeLabelsCount = 6; // 1, 2, 3, 4, 5, 6

    for (int i = 0; i <= timeLabelsCount; i++)
    {
        int x = graphX + (i * graphWidth / timeLabelsCount);
        LineArtManager::addFilledRect(
            SCREEN_MAIN,
            x,
            graphY + graphHeight,
            xTickWidth,
            xTickHeight,
            TFT_RED);
    }
}

void LineArtSetup::addDividerLines()
{
    // Divider line below the "Main Menu" text
    LineArtManager::addLine(
        SCREEN_MAIN,
        0, 45,
        SCREEN_WIDTH, 45,
        TFT_WHITE);

    // Divider line above the buttons
    LineArtManager::addLine(
        SCREEN_MAIN,
        0, 263,
        SCREEN_WIDTH, 263,
        TFT_WHITE);
}

void LineArtSetup::setupTemperatureTable()
{
    // Table positioning (matching the button layout in ButtonSetup)
    const int tableX = 120; // Moved right from 90
    const int tableY = 55;  // Moved down from 40
    const int cellWidth = 40;
    const int cellHeight = 40;
    const int tableWidth = cellWidth * 2;
    const int tableHeight = cellHeight * 4;

    // Draw outer rectangle - no gap between button and border
    LineArtManager::addRect(
        SCREEN_SETTINGS,
        tableX, tableY,
        tableWidth, tableHeight,
        TFT_WHITE);

    // Draw horizontal dividers
    for (int i = 1; i < 4; i++)
    {
        LineArtManager::addLine(
            SCREEN_SETTINGS,
            tableX, tableY + (cellHeight * i),
            tableX + tableWidth, tableY + (cellHeight * i),
            TFT_WHITE);
    }

    // Draw vertical divider
    LineArtManager::addLine(
        SCREEN_SETTINGS,
        tableX + cellWidth, tableY,
        tableX + cellWidth, tableY + tableHeight,
        TFT_WHITE);
}
