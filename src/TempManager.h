#ifndef TEMP_MANAGER_H
#define TEMP_MANAGER_H

#include <Arduino.h>
#include "LGFX_Config.h"

class TempManager {
public:
  // Constructor
  TempManager(LGFX& tft);
  
  // Properties
  int soakTemp;
  int reflowTemp;
  
  // Methods
  void increaseSoakTempCoarse();
  void decreaseSoakTempCoarse();
  void increaseSoakTempFine();
  void decreaseSoakTempFine();
  void increaseReflowTempCoarse();
  void decreaseReflowTempCoarse();
  void increaseReflowTempFine();
  void decreaseReflowTempFine();
  
  // Display methods
  void displayTemperatures(bool lightMode);
  void updateReflowTempDisplay(bool lightMode);
  
private:
  LGFX& _tft;
};

#endif // TEMP_MANAGER_H