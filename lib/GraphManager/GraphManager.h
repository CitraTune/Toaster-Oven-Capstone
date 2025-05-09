#ifndef GRAPH_MANAGER_H
#define GRAPH_MANAGER_H

#include <Arduino.h>
#include "LGFX_Config.h"

class GraphManager {
public:
  // Constructor
  GraphManager(LGFX& tft);
  
  // Properties
  bool fullScreen;
  
  // Methods
  void toggleFullScreen();
  void draw(bool lightMode);
  
private:
  LGFX& _tft;
};

#endif // GRAPH_MANAGER_H