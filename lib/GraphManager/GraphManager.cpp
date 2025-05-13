#include "GraphManager.hpp"
#include "UIManager.hpp" // For screen dimensions

// Constructor
GraphManager::GraphManager(LGFX& tft) : _tft(tft) {
  fullScreen = false;
}

// Toggle between full screen and normal graph
void GraphManager::toggleFullScreen() {
  fullScreen = !fullScreen;
}

// Draw the graph
void GraphManager::draw(bool lightMode) {
  int graphX = fullScreen ? 0 : 20;
  int graphY = fullScreen ? 40 : 50; // Offset by 40 pixels when fullscreen
  int graphWidth = fullScreen ? SCREEN_WIDTH : SCREEN_WIDTH - 40;
  int graphHeight = (graphWidth * 6) / 10; // Maintain 10:6 ratio

  uint16_t graphColor = !lightMode ? TFT_WHITE : TFT_BLACK;

  // Clear the previous graph area
  _tft.fillRect(0, 40, SCREEN_WIDTH, SCREEN_HEIGHT - 40, lightMode ? TFT_WHITE : TFT_BLACK);

  // Draw the graph rectangle
  _tft.fillRect(graphX, graphY, graphWidth, graphHeight, graphColor);
  _tft.drawRect(graphX, graphY, graphWidth, graphHeight, graphColor);

  // Draw grid lines to make perfect squares
  int horizontalSpacing = graphWidth / 10;
  int verticalSpacing = graphHeight / 6;

  for (int i = 1; i < 10; i++) {
    _tft.drawLine(graphX + i * horizontalSpacing, graphY, graphX + i * horizontalSpacing, 
                 graphY + graphHeight, TFT_DARKGRAY);
  }

  for (int i = 1; i < 6; i++) {
    _tft.drawLine(graphX, graphY + i * verticalSpacing, graphX + graphWidth, 
                 graphY + i * verticalSpacing, TFT_DARKGRAY);
  }
}