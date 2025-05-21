#pragma once

#include <Arduino.h>
#include <vector>
#include <map>
#include "LGFX_Config.h"

// Forward declaration to avoid circular dependencies
class LGFX;

enum LineArtType {
    LINE,
    RECTANGLE,
    FILLED_RECTANGLE
};

struct LineArt {
    int id;
    int screen;  // The screen this line art belongs to
    bool active;
    LineArtType type;
    
    // Coordinates
    int x1, y1, x2, y2;
    
    // For rectangles: x1,y1 is top-left corner, x2,y2 is width,height
    
    // Color
    uint32_t color;
    
    // Draw the line art element
    void draw(LGFX& display) const {
        if (!active) return;
        switch (type) {
            case LINE:
                display.drawLine(x1, y1, x2, y2, color);
                break;
            case RECTANGLE:
                display.drawRect(x1, y1, x2, y2, color); // x2,y2 are width,height
                break;
            case FILLED_RECTANGLE:
                display.fillRect(x1, y1, x2, y2, color); // x2,y2 are width,height
                break;
        }
    }
};

class LineArtManager {
private:
    static int nextId;
    static std::map<int, LineArt> lineArts;
    static LGFX* display;
    
public:
    // Initialize with display reference
    static void setup(LGFX& tft);
    
    // Check if line art is visible on the current screen
    static bool isVisibleOnScreen(int screenId);
    
    // Add a line
    static int addLine(int screen, int x1, int y1, int x2, int y2, uint32_t color = TFT_BLACK);
    
    // Add a rectangle outline
    static int addRect(int screen, int x, int y, int width, int height, uint32_t color = TFT_BLACK);
    
    // Add a filled rectangle
    static int addFilledRect(int screen, int x, int y, int width, int height, uint32_t color = TFT_BLACK);
    
    // Add a graph with grid lines
    static void addGraph(int screen, int x, int y, int width, int height, 
                         int horizontalLines, int verticalLines,
                         uint32_t frameColor = TFT_BLACK, 
                         uint32_t gridColor = TFT_BLACK,
                         uint32_t bgColor = TFT_LIGHTGRAY);
    
    // Get a line art by ID
    static LineArt* getLineArt(int id);
    
    // Update a line art's colors
    static void updateColor(int id, uint32_t color);
    
    // Update a line art's coordinates
    static void updateCoordinates(int id, int x1, int y1, int x2, int y2);
    
    // Remove a line art
    static void remove(int id);
    
    // Clear all line arts
    static void clear();
    
    // Update active state based on current screen
    static void updateActiveState(int currentScreen);
    
    // Draw all active line arts
    static void draw();
    
    // Alternative draw method that allows passing a display
    static void draw(LGFX& tft);
};