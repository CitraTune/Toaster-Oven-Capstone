#include "ReflowController.hpp"

// Define the static class members
float ReflowController::thermalLagOffset = 0.0;
float ReflowController::soakTemp = 0.0;
float ReflowController::reflowTemp = 0.0;
bool ReflowController::isActive = false;
ReflowController::ReflowPhase ReflowController::currentPhase = ReflowController::IDLE;
unsigned long ReflowController::phaseStartTime = 0;

void ReflowController::begin() {
    thermalLagOffset = 65.0; // Default thermal lag offset in Celsius
    soakTemp = 150.0;       // Default soak temperature
    reflowTemp = 230.0;     // Default reflow temperature
    isActive = false;
    currentPhase = IDLE;
    Serial.println("ReflowController initialized");
}

void ReflowController::setSoakTemperature(float temp) {
    soakTemp = temp;
    Serial.printf("ReflowController soak temperature set to: %.2f°C\n", soakTemp);
}

void ReflowController::setReflowTemperature(float temp) {
    reflowTemp = temp;
    Serial.printf("ReflowController reflow temperature set to: %.2f°C\n", reflowTemp);
}

void ReflowController::startReflow() {
    if (soakTemp <= 0.0 || reflowTemp <= 0.0) {
        Serial.println("Error: Target temperatures not set properly");
        return;
    }
    
    isActive = true;
    currentPhase = PREHEAT;
    phaseStartTime = millis();
    TempManager::activateHeater(1.0); // Start with 100% power
    Serial.println("Reflow process started - Phase: PREHEAT with 100% power");
}

void ReflowController::stopReflow() {
    isActive = false;
    currentPhase = IDLE;
    TempManager::deactivateHeater();
    Serial.println("Reflow process stopped");
}

float ReflowController::calculateDutyCycle(float targetTemperature) {
    // Compute duty cycle using regression: PWM = (Temp - b) / m
    const float m = 5.27668f;
    const float b = 58.53368f;
    float dutyCycle = (targetTemperature - b) / m / 100.0f;  // convert % to 0.0–1.0

    // Clamp duty cycle between 0.05 and 0.5 for safety
    if (dutyCycle < 0.05f) dutyCycle = 0.05f;
    if (dutyCycle > 0.5f) dutyCycle = 0.5f;
    
    return dutyCycle;
}

void ReflowController::update() {
    if (!isActive) return;

    float currentTemp = TempManager::getTemperature();
    unsigned long currentTime = millis();
    unsigned long phaseDuration = currentTime - phaseStartTime;

    // Emergency shutoff if temperature exceeds 250°C
    if (currentTemp > 250.0) {
        Serial.println("EMERGENCY SHUTOFF: Temperature exceeded 250°C!");
                stopReflow();
        // You might want to add additional emergency procedures here
        // such as triggering an alarm or sending a notification
        return;
            }

    switch (currentPhase) {
        case PREHEAT:
            // Full power until we reach soak temp minus thermal lag offset
            if (currentTemp >= (soakTemp - thermalLagOffset)) {
                // Transition to SOAK phase
                currentPhase = SOAK;
                phaseStartTime = currentTime;
                
            } else {
                TempManager::setDutyCycle(1.0);
                
            }
            break;
            
        case SOAK:
            // Maintain soak temperature for SOAK_DURATION
            if (phaseDuration >= SOAK_DURATION) {
                // Transition to REFLOW phase
                currentPhase = REFLOW;
                phaseStartTime = currentTime;
                TempManager::setDutyCycle(1.0); // Full power to begin reflow
                
            } else {
                float dutyCycle = calculateDutyCycle(soakTemp);
                TempManager::setDutyCycle(dutyCycle);
                
            }
            break;
            
        case REFLOW:
            if (phaseDuration < REFLOW_DURATION) {
                // If we're below the threshold (reflow temp - thermalLagOffset), use 100% power
                if (currentTemp < (reflowTemp - thermalLagOffset) && phaseDuration < PREHEAT_REFLOW_DURATION) {
                    TempManager::setDutyCycle(1.0);
                    
                } else {
                    // Calculate duty cycle to maintain reflow temperature
                    
                    TempManager::setDutyCycle(0);
                    
                }
            } else {
                // Transition to COOLING phase
                currentPhase = COOLING;
                phaseStartTime = currentTime;
                TempManager::deactivateHeater();
                
            }
            break;
            
        case COOLING:
            // Cooling phase - no heating
            Serial.printf("COOLING: Temp=%.2f°C\n", currentTemp);
            
            // Optional: End the process when temperature drops below a certain level
            if (currentTemp < 100.0) {
                stopReflow();
                Serial.println("Reflow cycle completed and cooled down");
            }
            break;
            
        default:
            break;
    }
}

void ReflowController::setThermalLagOffset(float offset) {
    thermalLagOffset = offset;
    Serial.printf("Thermal lag offset set to: %.2f°C\n", thermalLagOffset);
}

bool ReflowController::isReflowActive() {
    return isActive;
}

ReflowController::ReflowPhase ReflowController::getCurrentPhase() {
    return currentPhase;
}
