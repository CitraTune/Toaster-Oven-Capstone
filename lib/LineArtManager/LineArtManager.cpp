#include "LineArtManager.hpp"

// Initialize static members
int LineArtManager::nextId = 0;
std::map<int, LineArt> LineArtManager::lineArts;
LGFX* LineArtManager::display = nullptr;

// Initialize with display reference
void LineArtManager::setup(LGFX& tft) {
    display = &tft;
}

// Check if line art is visible on the current screen
bool LineArtManager::isVisibleOnScreen(int screenId) {
    for (const auto& pair : lineArts) {
        const LineArt& art = pair.second;
        if (art.screen == screenId) {
            return true;
        }
    }
    return false;
}

// Add a line
int LineArtManager::addLine(int screen, int x1, int y1, int x2, int y2, uint32_t color) {
    LineArt line;
    line.id = nextId++;
    line.screen = screen;
    line.active = false;
    line.type = LINE;
    line.x1 = x1;
    line.y1 = y1;
    line.x2 = x2;
    line.y2 = y2;
    line.color = color;
    
    lineArts[line.id] = line;
    return line.id;
}

// Add a rectangle outline
int LineArtManager::addRect(int screen, int x, int y, int width, int height, uint32_t color) {
    LineArt rect;
    rect.id = nextId++;
    rect.screen = screen;
    rect.active = false;
    rect.type = RECTANGLE;
    rect.x1 = x;
    rect.y1 = y;
    rect.x2 = width;
    rect.y2 = height;
    rect.color = color;
    
    lineArts[rect.id] = rect;
    return rect.id;
}

// Add a filled rectangle
int LineArtManager::addFilledRect(int screen, int x, int y, int width, int height, uint32_t color) {
    LineArt rect;
    rect.id = nextId++;
    rect.screen = screen;
    rect.active = false;
    rect.type = FILLED_RECTANGLE;
    rect.x1 = x;
    rect.y1 = y;
    rect.x2 = width;
    rect.y2 = height;
    rect.color = color;
    
    lineArts[rect.id] = rect;
    return rect.id;
}

// Add a graph with grid lines
void LineArtManager::addGraph(int screen, int x, int y, int width, int height, 
                     int horizontalLines, int verticalLines,
                     uint32_t frameColor, 
                     uint32_t gridColor,
                     uint32_t bgColor) {
    
    // Add background
    addFilledRect(screen, x, y, width, height, bgColor);
    
    // Add frame
    addRect(screen, x, y, width, height, frameColor);
    
    // Add horizontal grid lines
    int verticalSpacing = height / (horizontalLines + 1);
    for (int i = 1; i <= horizontalLines; ++i) {
        int lineY = y + i * verticalSpacing;
        addLine(screen, x, lineY, x + width, lineY, gridColor);
    }
    
    // Add vertical grid lines
    int horizontalSpacing = width / (verticalLines + 1);
    for (int i = 1; i <= verticalLines; ++i) {
        int lineX = x + i * horizontalSpacing;
        addLine(screen, lineX, y, lineX, y + height, gridColor);
    }
}

// Get a line art by ID
LineArt* LineArtManager::getLineArt(int id) {
    auto it = lineArts.find(id);
    if (it != lineArts.end()) {
        return &it->second;
    }
    return nullptr;
}

// Update a line art's colors
void LineArtManager::updateColor(int id, uint32_t color) {
    auto it = lineArts.find(id);
    if (it != lineArts.end()) {
        it->second.color = color;
    }
}

// Update a line art's coordinates
void LineArtManager::updateCoordinates(int id, int x1, int y1, int x2, int y2) {
    auto it = lineArts.find(id);
    if (it != lineArts.end()) {
        it->second.x1 = x1;
        it->second.y1 = y1;
        it->second.x2 = x2;
        it->second.y2 = y2;
    }
}

// Remove a line art
void LineArtManager::remove(int id) {
    lineArts.erase(id);
}

// Clear all line arts
void LineArtManager::clear() {
    lineArts.clear();
    nextId = 0;
}

// Update active state based on current screen
void LineArtManager::updateActiveState(int currentScreen) {
    for (auto& pair : lineArts) {
        LineArt& art = pair.second;
        art.active = (art.screen == currentScreen);
    }
}

// Draw all active line arts
void LineArtManager::draw() {
    if (!display) return;  // Safety check
    
    for (const auto& pair : lineArts) {
        const LineArt& art = pair.second;
        art.draw(*display);
    }
}

// Alternative draw method that allows passing a display
void LineArtManager::draw(LGFX& tft) {
    for (const auto& pair : lineArts) {
        const LineArt& art = pair.second;
        art.draw(tft);
    }
}