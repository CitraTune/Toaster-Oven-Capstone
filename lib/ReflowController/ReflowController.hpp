#pragma once
#include <Arduino.h>
#include "LGFX_Config.h"
#include <bb_captouch.h>
#include "UIManager.hpp"
#include "GraphManager.hpp"
#include "TempManager.hpp"

class ReflowController {
public:
  // Constructor
  ReflowController();
  
  // setup all components
  void setup();
  
  // Main loop
  void loop();
  
  // Action methods
  void goToSettings();
  void goToMain();
  void toggleLightMode();
  void toggleInvertAccent();
  void toggleGraphSize();
  
  // Getters for components
  UIManager* getUIManager() { return &uiManager; }
  GraphManager* getGraphManager() { return &graphManager; }
  TempManager* getTempManager() { return &tempManager; }
  
private:
  // Components
  LGFX myTFT;
  BBCapTouch touch;
  UIManager uiManager;
  GraphManager graphManager;
  TempManager tempManager;
  
  // Debounce variables
  unsigned long lastTouchTime;
  const unsigned long debounceDelay = 200; // 200ms debounce delay
  
  // setup interface buttons
  void setupButtons();
};

