#include "GraphManager.hpp"
#include "UIManager.hpp"

// Constructor
GraphManager::GraphManager(LGFX& tft) : _tft(tft) {
}

// Check if graph should be visible
bool GraphManager::isVisibleOnScreen(int screenId) {
    return screenId == SCREEN_MAIN;
}

// Draw internal graph elements (grid, axes, etc.)
void GraphManager::drawGraphElements(int graphX, int graphY, int graphWidth, int graphHeight) {
    // Fill background
    _tft.fillRect(graphX, graphY, graphWidth, graphHeight, TFT_LIGHTGRAY);
    
    // Draw border
    _tft.drawRect(graphX, graphY, graphWidth, graphHeight, TFT_BLACK);

    // Draw grid lines
    int horizontalSpacing = graphWidth / 10;
    int verticalSpacing = graphHeight / 6;

    for (int i = 1; i < 10; i++) {
        _tft.drawLine(
            graphX + i * horizontalSpacing, graphY,
            graphX + i * horizontalSpacing, graphY + graphHeight,
            TFT_BLACK
        );
    }

    for (int i = 1; i < 6; i++) {
        _tft.drawLine(
            graphX, graphY + i * verticalSpacing,
            graphX + graphWidth, graphY + i * verticalSpacing,
            TFT_BLACK
        );
    }
}

// Draw the graph
void GraphManager::draw() {
    // Fixed position and size for graph
    int graphX = 20;
    int graphY = 50;
    int graphWidth = SCREEN_WIDTH - 40;  // 40 pixels total margin (20 each side)
    int graphHeight = 120;  // Fixed height instead of calculated
    
    // Draw the graph elements
    drawGraphElements(graphX, graphY, graphWidth, graphHeight);
}
