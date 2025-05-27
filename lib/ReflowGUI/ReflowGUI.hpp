#pragma once
#include <Arduino.h>
#include "../config/LGFX_Config.h"
#include <bb_captouch.h>
#include "../UIManager/UIManager.hpp"


class ReflowGUI {
public:
  // Constructor
  ReflowGUI();
  
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
  
  // Temperature methods
  void increaseSoakTemp(bool coarse);
  void decreaseSoakTemp(bool coarse);
  void increaseReflowTemp(bool coarse);
  void decreaseReflowTemp(bool coarse);
  void setupTemperatureElements();
  
  // Getters for components
  UIManager* getUIManager() { return &uiManager; }
 

private:
  int soakTemp;
  int reflowTemp;

  
private:
  // Components
  LGFX display;
  BBCapTouch touch;
  UIManager uiManager;

  
  // Debounce variables
  unsigned long lastTouchTime;
  const unsigned long debounceDelay = 200; // 200ms debounce delay
  
  // setup interface buttons
  void setupButtons();
};
