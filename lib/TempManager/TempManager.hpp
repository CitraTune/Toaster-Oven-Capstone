#pragma once

#include <Arduino.h>
#include <max6675.h>

// Define pins for MAX6675 thermocouple module
// These are dummy values - replace with actual pins
#define THERMO_DO_PIN  19  // Data Out (SO) pin
#define THERMO_CS_PIN  5   // Chip Select (CS) pin
#define THERMO_CLK_PIN 18  // Clock (SCK) pin

class TempManager {
private:
    MAX6675 thermocouple;
    float currentTemp;
    unsigned long lastReadTime;
    const unsigned long READ_INTERVAL = 1000; // Read temperature every 1 second

public:
    TempManager() : 
        thermocouple(THERMO_CLK_PIN, THERMO_CS_PIN, THERMO_DO_PIN),
        currentTemp(0.0),
        lastReadTime(0)
    {
        // Initialize with a first reading
        currentTemp = thermocouple.readCelsius();
    }

    // Update temperature reading if interval has passed
    void update() {
        unsigned long currentTime = millis();
        if (currentTime - lastReadTime >= READ_INTERVAL) {
            currentTemp = thermocouple.readCelsius();
            lastReadTime = currentTime;
        }
    }

    // Get current temperature in Celsius
    float getTemperature() const {
        return currentTemp;
    }

    // Get temperature as formatted string with C suffix
    String getTemperatureString() const {
        return String(round(currentTemp)) + "C";
    }
};