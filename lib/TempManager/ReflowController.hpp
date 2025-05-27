#pragma once

#include "TempManager.hpp"

class ReflowController {
public:
    // Reflow phases
    enum ReflowPhase {
        IDLE,
        PREHEAT,
        SOAK,
        REFLOW,
        COOLING
    };

    // Initialize with default values
    static void begin();
    
    // Set the target temperatures
    static void setSoakTemperature(float temp);
    static void setReflowTemperature(float temp);
    
    // Start the reflow process
    static void startReflow();
    
    // Stop the reflow process
    static void stopReflow();
    
    // Update function to be called regularly from the main loop
    static void update();
    
    // Set thermal lag offset
    static void setThermalLagOffset(float offset);
    
    // Get status
    static bool isReflowActive();
    
    // Get current phase
    static ReflowPhase getCurrentPhase();

private:
    static float thermalLagOffset;      // °C below target to start reducing power
    static float soakTemp;              // Soak temperature in °C
    static float reflowTemp;            // Reflow temperature in °C
    static bool isActive;               // Is reflow process active
    static ReflowPhase currentPhase;    // Current phase of reflow
    static unsigned long phaseStartTime; // When the current phase started
    static const unsigned long SOAK_DURATION = 90000;    // 60 seconds
    static const unsigned long PREHEAT_REFLOW_DURATION = 20000; // 60 seconds
    static const unsigned long REFLOW_DURATION = 30000;  // 30 seconds
    
    // Helper methods for calculating duty cycle
    static float calculateDutyCycle(float targetTemperature);
};