#pragma once

#include <Arduino.h>
#include <max6675.h>

// Forward declaration
class LineArtSetup;

// Define pins for MAX6675 thermocouple module
// These are dummy values - replace with actual pins
#define THERMO_DO_PIN 35  // Data Out (SO) pin
#define THERMO_CS_PIN 4   // Chip Select (CS) pin
#define THERMO_CLK_PIN 16 // Clock (SCK) pin
#define SSR_PIN 17        // SSR control pin

class TempManager
{
private:
    static const int MAX_TEMP_READINGS = 24; // Store up to 6 minutes of readings (24 x 15 seconds)
    static float tempReadings[MAX_TEMP_READINGS];
    static unsigned long readingTimestamps[MAX_TEMP_READINGS];
    static int readingsCount;

    static MAX6675 thermocouple;
    static float currentTemp;
    static unsigned long lastReadTime;
    static const unsigned long READ_INTERVAL = 1000; // Read temperature every 1 second

    // Custom PWM variables
    static bool ssrState;                     // Current state of the SSR
    static unsigned long ssrLastToggleTime;   // Last time the SSR state was changed
    static const unsigned long TIME_ON = 500; // Fixed on time (0.5 seconds in ms)
    static float dutyCycle;                   // Current duty cycle (0.0 to 1.0)
    static unsigned long timeOff;             // Calculated off time based on duty cycle

    // Safety feature - heater active flag
    static bool heaterActive;
    // Timer for auto-shutoff
    static unsigned long heaterStartTime;
    static unsigned long heaterDuration;

    // Add a method to record temperature readings
    static void recordTemperature();
    static void resetTempReadings();
    static void updateSSR(unsigned long currentTime);
    static void addTempPointToGraph();

public:
    // Initialize the temperature manager
    static void begin();

    // Update temperature reading if interval has passed
    static void update();

    // Activate the heater with specified duty cycle and duration
    static void activateHeater(float duty, unsigned long duration_ms = 0);

    // Deactivate the heater
    static void deactivateHeater();

    // Check if heater is currently active
    static bool isHeaterActive();

    // Set duty cycle (0.0 to 1.0)
    static void setDutyCycle(float duty);

    // Get current duty cycle
    static float getDutyCycle();

    // Get current temperature in Celsius
    static float getTemperature();

    // Get temperature as formatted string with C suffix
    static String getTemperatureString();

    // Get SSR state
    static bool getSSRState();
};
