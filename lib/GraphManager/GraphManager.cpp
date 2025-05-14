#include "GraphManager.hpp"
#include "UIManager.hpp" // For screen dimensions

// Constructor
GraphManager::GraphManager(LGFX& tft) : _tft(tft) {
  fullScreen = false;
  visibleScreens = nullptr;
  screenCount = 0;
}

// Toggle between full screen and normal graph
void GraphManager::toggleFullScreen() {
  fullScreen = !fullScreen;
}

// Set which screen(s) the graph should appear on
void GraphManager::setVisibleScreens(int* screens, int count) {
  // Free any previous allocation
  if (visibleScreens != nullptr) {
    delete[] visibleScreens;
  }
  
  // Allocate and copy new screens
  screenCount = count;
  visibleScreens = new int[count];
  for (int i = 0; i < count; i++) {
    visibleScreens[i] = screens[i];
  }
}

// Check if graph is visible on current screen
bool GraphManager::isVisibleOnScreen(int screenId) {
  if (visibleScreens == nullptr) return false;
  
  for (int i = 0; i < screenCount; i++) {
    if (visibleScreens[i] == screenId) {
      return true;
    }
  }
  return false;
}

// Draw internal graph elements (grid, axes, etc.)
void GraphManager::drawGraphElements(int graphX, int graphY, int graphWidth, int graphHeight, uint16_t graphColor) {
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

// Draw the graph with background clearing
void GraphManager::draw(bool lightMode) {
  int graphX = fullScreen ? 0 : 20;
  int graphY = fullScreen ? 40 : 50; // Offset by 40 pixels when fullscreen
  int graphWidth = fullScreen ? SCREEN_WIDTH : SCREEN_WIDTH - 40;
  int graphHeight = (graphWidth * 6) / 10; // Maintain 10:6 ratio

  uint16_t graphColor = !lightMode ? TFT_WHITE : TFT_BLACK;
  uint16_t bgColor = lightMode ? TFT_WHITE : TFT_BLACK;

  // Clear the previous graph area
  _tft.fillRect(0, 40, SCREEN_WIDTH, SCREEN_HEIGHT - 40, bgColor);

  // Draw graph elements
  drawGraphElements(graphX, graphY, graphWidth, graphHeight, graphColor);
}

// Redraw just the graph content without clearing background
void GraphManager::redrawGraphOnly(bool lightMode) {
  int graphX = fullScreen ? 0 : 20;
  int graphY = fullScreen ? 40 : 50; // Offset by 40 pixels when fullscreen
  int graphWidth = fullScreen ? SCREEN_WIDTH : SCREEN_WIDTH - 40;
  int graphHeight = (graphWidth * 6) / 10; // Maintain 10:6 ratio

  uint16_t graphColor = !lightMode ? TFT_WHITE : TFT_BLACK;
  
  // Just redraw the graph elements without clearing the background
  drawGraphElements(graphX, graphY, graphWidth, graphHeight, graphColor);
  
  // Here you would add code to draw the actual graph data
  // For example: plot points, lines, etc.
}
