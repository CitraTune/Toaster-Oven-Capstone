#include "TempManager.hpp"

// Initialize static variables
MAX6675 TempManager::thermocouple(THERMO_CLK_PIN, THERMO_CS_PIN, THERMO_DO_PIN);
float TempManager::currentTemp = 0.0;
unsigned long TempManager::lastReadTime = 0;
bool TempManager::ssrState = false;
unsigned long TempManager::ssrLastToggleTime = 0;
float TempManager::dutyCycle = 0.0;
unsigned long TempManager::timeOff = 10000;
bool TempManager::heaterActive = false;
unsigned long TempManager::heaterStartTime = 0;
unsigned long TempManager::heaterDuration = 0;

