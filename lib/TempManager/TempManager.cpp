#include "TempManager.hpp"
#include "../Setups/LineArtSetup.hpp"

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
float TempManager::tempReadings[MAX_TEMP_READINGS] = {0};
unsigned long TempManager::readingTimestamps[MAX_TEMP_READINGS] = {0};
int TempManager::readingsCount = 0;

void TempManager::begin() {
    // Initialize SSR pin
    pinMode(SSR_PIN, OUTPUT);
    digitalWrite(SSR_PIN, LOW);

    // Initialize with a first reading
    currentTemp = thermocouple.readCelsius();

    // Ensure heater is inactive on startup
    heaterActive = false;
    Serial.println("TempManager initialized - Heater inactive");
}

void TempManager::update() {
    unsigned long currentTime = millis();

    // Update temperature reading
    if (currentTime - lastReadTime >= READ_INTERVAL) {
        currentTemp = thermocouple.readCelsius();
        lastReadTime = currentTime;

        // Record temperature if heater is active
    if (heaterActive) {
            recordTemperature();
    }

        // Debug temperature readings
        Serial.printf("Temperature: %.2f°C\n", currentTemp);
    }

    // Check if heater duration has elapsed
    if (heaterActive && heaterDuration > 0) {
        if (currentTime - heaterStartTime >= heaterDuration) {
            deactivateHeater();
            Serial.println("Auto-shutoff: Heater duration elapsed");
        }
    }

    // Only update SSR if heater is active
    if (heaterActive) {
        updateSSR(currentTime);
    }
    else if (ssrState) {
        // If heater is inactive but SSR is still on, turn it off
        ssrState = false;
        digitalWrite(SSR_PIN, LOW);
        Serial.println("Safety check: Disabled SSR because heater is inactive");
    }
}

void TempManager::activateHeater(float duty, unsigned long duration_ms) {
    setDutyCycle(duty);
    heaterActive = true;
    heaterStartTime = millis();
    heaterDuration = duration_ms;

    Serial.printf("Heater activated - Duty cycle: %.2f%%, Duration: %lu ms\n",
                duty * 100, duration_ms);
}

void TempManager::deactivateHeater() {
    heaterActive = false;
    // Ensure SSR is off
    ssrState = false;
    digitalWrite(SSR_PIN, LOW);

    Serial.println("Heater deactivated");
}

bool TempManager::isHeaterActive() {
    return heaterActive;
}

void TempManager::setDutyCycle(float duty) {
    // Constrain duty cycle between 0.0 and 1.0
    dutyCycle = constrain(duty, 0.0, 1.0);

    // For 100% duty cycle, set timeOff to 0 to keep SSR always on
    if (dutyCycle >= 0.99) {
        timeOff = 0; // Always on
    }
    else if (dutyCycle <= 0.01) {
        timeOff = 10000; // 10 seconds off when essentially zero
    }
    else {
        // TIME_ON / dutyCycle = total cycle time
        // timeOff = total cycle time - TIME_ON
        timeOff = (TIME_ON / dutyCycle) - TIME_ON;
    }

    Serial.printf("Duty cycle set to: %.2f%%, Time off: %lu ms\n",
                dutyCycle * 100, timeOff);
}

float TempManager::getDutyCycle() {
    return dutyCycle;
}

float TempManager::getTemperature() {
    return currentTemp;
}

String TempManager::getTemperatureString() {
    // Format with 2 decimal places
    return String(currentTemp, 2) + "C";
}

bool TempManager::getSSRState() {
    return ssrState;
}

void TempManager::recordTemperature() {
    // Only record if we have space and heater is active
    if (readingsCount < MAX_TEMP_READINGS && heaterActive) {
        unsigned long currentTime = millis();

        // Only record every 15 seconds
        if (readingsCount == 0 ||
            (currentTime - readingTimestamps[readingsCount-1] >= 15000)) {

            tempReadings[readingsCount] = currentTemp;
            readingTimestamps[readingsCount] = currentTime;
            readingsCount++;

            // Draw this point on the graph
            addTempPointToGraph();

            Serial.printf("Temperature recorded: %.2f°C at time %lu ms\n",
                        currentTemp, currentTime - heaterStartTime);
        }
    }
}

void TempManager::resetTempReadings() {
    readingsCount = 0;

    Serial.println("Temperature readings reset");
}

void TempManager::addTempPointToGraph() {
    if (readingsCount == 0) return;

    // Calculate position on graph using LineArtSetup's public constants
    unsigned long timeElapsed = readingTimestamps[readingsCount-1] - heaterStartTime;

    int x = LineArtSetup::graphX + (timeElapsed * LineArtSetup::graphWidth / (360 * 1000));

    // Y position: temperature (0-250°C full scale)
    // Note: Y is inverted in screen coordinates (0 at top)
    const int maxTemp = 250; // Maximum temperature on graph
    int y = (LineArtSetup::graphY + LineArtSetup::graphHeight) -
           (tempReadings[readingsCount-1] * LineArtSetup::graphHeight / maxTemp);

    // Constrain to graph boundaries
    x = constrain(x, LineArtSetup::graphX, LineArtSetup::graphX + LineArtSetup::graphWidth);
    y = constrain(y, LineArtSetup::graphY, LineArtSetup::graphY + LineArtSetup::graphHeight);

    // Draw a small red square
    const int dotSize = 3;
    LineArtManager::addFilledRect(
        SCREEN_MAIN,
        x - dotSize/2,
        y - dotSize/2,
        dotSize,
        dotSize,
        TFT_RED
    );

    Serial.printf("Added temperature point at x=%d, y=%d\n", x, y);
}

void TempManager::updateSSR(unsigned long currentTime) {
    unsigned long elapsedTime = currentTime - ssrLastToggleTime;

    // For 100% duty cycle, keep SSR on continuously
    if (dutyCycle >= 0.99) {
        if (!ssrState) {
            ssrState = true;
            digitalWrite(SSR_PIN, HIGH);
            ssrLastToggleTime = currentTime;

            Serial.println("SSR turned ON (100% duty)");
        }

        return;
    }

    if (ssrState) {
        // If SSR is ON and the on-time has elapsed, turn it OFF
        if (elapsedTime >= TIME_ON) {
            ssrState = false;
            digitalWrite(SSR_PIN, LOW);
            ssrLastToggleTime = currentTime;

            Serial.println("SSR turned OFF");
        }
    }
    else {
        // If SSR is OFF and the off-time has elapsed, turn it ON
        if (elapsedTime >= timeOff) {
            ssrState = true;
            digitalWrite(SSR_PIN, HIGH);
            ssrLastToggleTime = currentTime;
            Serial.println("SSR turned ON");
        }
    }
}

