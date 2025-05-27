#include "ReflowController.hpp"

// Define the static class members
float ReflowController::thermalLagOffset = 0.0;
float ReflowController::targetTemp = 0.0;
bool ReflowController::isActive = false;
float ReflowController::dutyCycleCoefficient = 0.0;

void ReflowController::begin() {
    thermalLagOffset = 50.0; // Default thermal lag offset in Celsius
    targetTemp = 0.0;
    isActive = false;
    dutyCycleCoefficient = 0.001f;
    Serial.println("ReflowController initialized");
}

void ReflowController::setTargetTemperature(float temp) {
    targetTemp = temp;
    Serial.printf("ReflowController target temperature set to: %.2f°C\n", targetTemp);
}

void ReflowController::startReflow() {
    if (targetTemp <= 0.0) {
        Serial.println("Error: Target temperature not set properly");
        return;
    }
    
    isActive = true;
    TempManager::activateHeater(1.0); // Start with 100% power
    Serial.println("Reflow process started with 100% power");
}

void ReflowController::stopReflow() {
    isActive = false;
    TempManager::deactivateHeater();
    Serial.println("Reflow process stopped");
}

void ReflowController::update() {
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

void ReflowController::setThermalLagOffset(float offset) {
    thermalLagOffset = offset;
    Serial.printf("Thermal lag offset set to: %.2f°C\n", thermalLagOffset);
}

void ReflowController::setDutyCycleCoefficient(float coeff) {
    dutyCycleCoefficient = coeff;
    Serial.printf("Duty cycle coefficient set to: %.6f\n", dutyCycleCoefficient);
}

bool ReflowController::isReflowActive() {
    return isActive;
}