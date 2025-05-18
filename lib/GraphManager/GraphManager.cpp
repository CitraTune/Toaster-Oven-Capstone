#include "GraphManager.hpp"
#include "UIManager.hpp"

// Check if graph should be visible
bool GraphManager::isVisibleOnScreen(int screenId) {
    return screenId == SCREEN_MAIN;
}

// Draw internal graph elements (grid, axes, etc.)
void GraphManager::drawGraphElements(LGFX& tft, int graphX, int graphY, int graphWidth, int graphHeight) {
    // Fill background
    tft.fillRect(graphX, graphY, graphWidth, graphHeight, TFT_LIGHTGRAY);
    
    // Draw border
    tft.drawRect(graphX, graphY, graphWidth, graphHeight, TFT_BLACK);

    // Draw grid lines
    int horizontalSpacing = graphWidth / 10;
    int verticalSpacing = graphHeight / 6;

    for (int i = 1; i < 10; i++) {
        tft.drawLine(
            graphX + i * horizontalSpacing, graphY,
            graphX + i * horizontalSpacing, graphY + graphHeight,
            TFT_BLACK
        );
    }

    for (int i = 1; i < 6; i++) {
        tft.drawLine(
            graphX, graphY + i * verticalSpacing,
            graphX + graphWidth, graphY + i * verticalSpacing,
            TFT_BLACK
        );
    }
}

// Draw the graph
void GraphManager::draw(LGFX& tft) {
    // Fixed position and size for graph
    int graphX = 20;
    int graphY = 50;
    int graphWidth = SCREEN_WIDTH - 40;  // 40 pixels total margin (20 each side)
    int graphHeight = 120;  // Fixed height instead of calculated

    // Draw the graph elements
    drawGraphElements(tft, graphX, graphY, graphWidth, graphHeight);
}
