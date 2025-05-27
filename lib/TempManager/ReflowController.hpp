#pragma once

#include "TempManager.hpp"

class ReflowController {
public:
    // Initialize with default values
    static void begin() {
        thermalLagOffset = 50.0; // Default thermal lag offset in Celsius
        targetTemp = 0.0;
        isActive = false;
        dutyCycleCoefficient = 0.001f;
        Serial.println("ReflowController initialized");
    }

    // Set the target temperature
    static void setTargetTemperature(float temp) {
        targetTemp = temp;
        Serial.printf("ReflowController target temperature set to: %.2f°C\n", targetTemp);
    }

    // Start the reflow process
    static void startReflow() {
        if (targetTemp <= 0.0) {
            Serial.println("Error: Target temperature not set properly");
            return;
        }
        
        isActive = true;
        TempManager::activateHeater(1.0); // Start with 100% power
        Serial.println("Reflow process started with 100% power");
    }

    // Stop the reflow process
    static void stopReflow() {
        isActive = false;
        TempManager::deactivateHeater();
        Serial.println("Reflow process stopped");
    }

    // Update function to be called regularly from the main loop
    static void update() {
        if (!isActive) return;
        
        float currentTemp = TempManager::getTemperature();
        
        // If we're below the threshold (target - thermalLagOffset), use 100% power
        if (currentTemp < (targetTemp - thermalLagOffset)) {
            TempManager::setDutyCycle(1.0);
            Serial.printf("Heating up: Temp=%.2f°C, Target=%.2f°C, Power=100%%\n",
                         currentTemp, targetTemp);
        } else {
            // We've passed the threshold, now use the fixed duty cycle based on target temperature
            float dutyCycle = targetTemp * dutyCycleCoefficient;

            // Clamp duty cycle between 0.05 and 0.5 for safety
            if (dutyCycle < 0.05f) dutyCycle = 0.05f;
            if (dutyCycle > 0.5f) dutyCycle = 0.5f;
            TempManager::setDutyCycle(dutyCycle);
            Serial.printf("Maintaining temp: Temp=%.2f°C, Target=%.2f°C, Duty=%.2f%%\n",
                         currentTemp, targetTemp, dutyCycle * 100);
        }
    }

    // Set thermal lag offset
    static void setThermalLagOffset(float offset) {
        thermalLagOffset = offset;
        Serial.printf("Thermal lag offset set to: %.2f°C\n", thermalLagOffset);
    }

    // Set duty cycle coefficient
    static void setDutyCycleCoefficient(float coeff) {
        dutyCycleCoefficient = coeff;
        Serial.printf("Duty cycle coefficient set to: %.6f\n", dutyCycleCoefficient);
    }

    // Get status
    static bool isReflowActive() {
        return isActive;
    }

private:
    static float thermalLagOffset;      // °C below target to start reducing power
    static float targetTemp;            // Target temperature in °C
    static bool isActive;               // Is reflow process active
    static float dutyCycleCoefficient;  // Coefficient for duty cycle calculation
};


