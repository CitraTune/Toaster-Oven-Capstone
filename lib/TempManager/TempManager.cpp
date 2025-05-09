#include "TempManager.h"

// Constructor
TempManager::TempManager(LGFX& tft) : _tft(tft) {
  soakTemp = 150;  // Default soak temperature
  reflowTemp = 230;  // Default reflow temperature
}

// Temperature adjustment methods
void TempManager::increaseSoakTempCoarse() { 
  soakTemp += 10; 
}

void TempManager::decreaseSoakTempCoarse() { 
  soakTemp -= 10; 
}

void TempManager::increaseSoakTempFine() { 
  soakTemp += 1; 
}

void TempManager::decreaseSoakTempFine() { 
  soakTemp -= 1; 
}

void TempManager::increaseReflowTempCoarse() { 
  reflowTemp += 10; 
}

void TempManager::decreaseReflowTempCoarse() { 
  reflowTemp -= 10; 
}

void TempManager::increaseReflowTempFine() { 
  reflowTemp += 1; 
}

void TempManager::decreaseReflowTempFine() { 
  reflowTemp -= 1; 
}

// Display temperature values on settings screen
void TempManager::displayTemperatures(bool lightMode) {
  _tft.setFont(&lgfx::fonts::FreeSans9pt7b);
  _tft.setTextColor(lightMode ? TFT_BLACK : TFT_WHITE);
  
  // Display soak temperature
  _tft.setCursor(10, 50);
  _tft.printf("Soak Temp\n     %d C", soakTemp);
  
  // Display reflow temperature
  _tft.setCursor(10, 140);
  _tft.printf("Reflow Temp\n     %d C", reflowTemp);
}

// Update only the reflow temperature display
void TempManager::updateReflowTempDisplay(bool lightMode) {
  int tempX = 10; // X position of the reflow temp text
  int tempY = 140; // Y position of the reflow temp text
  int tempWidth = 100; // Width of the area to clear
  int tempHeight = _tft.fontHeight() * 2; // Height of the area to clear

  // Clear the area around the reflow temp
  _tft.fillRect(tempX, tempY, tempWidth, tempHeight, lightMode ? TFT_WHITE : TFT_BLACK);

  // Redraw the reflow temp text
  _tft.setCursor(tempX, tempY);
  _tft.setTextColor(lightMode ? TFT_BLACK : TFT_WHITE);
  _tft.printf("Reflow Temp\n     %d C", reflowTemp);
}