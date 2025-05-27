#pragma once

#include "TempManager.hpp"

class ReflowController {
public:
    // Initialize with default values
    static void begin();
    
    // Set the target temperature
    static void setTargetTemperature(float temp);
    
    // Start the reflow process
    static void startReflow();
    
    // Stop the reflow process
    static void stopReflow();
    
    // Update function to be called regularly from the main loop
    static void update();
    
    // Set thermal lag offset
    static void setThermalLagOffset(float offset);
    
    // Set duty cycle coefficient
    static void setDutyCycleCoefficient(float coeff);
    
    // Get status
    static bool isReflowActive();

private:
    static float thermalLagOffset;      // °C below target to start reducing power
    static float targetTemp;            // Target temperature in °C
    static bool isActive;               // Is reflow process active
    static float dutyCycleCoefficient;  // Coefficient for duty cycle calculation
};