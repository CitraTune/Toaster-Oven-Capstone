#pragma once

#include <Arduino.h>
#include <max6675.h>

// Define pins for MAX6675 thermocouple module
// These are dummy values - replace with actual pins
#define THERMO_DO_PIN 35  // Data Out (SO) pin
#define THERMO_CS_PIN 4   // Chip Select (CS) pin
#define THERMO_CLK_PIN 16 // Clock (SCK) pin
#define SSR_PIN 17        // SSR control pin

class TempManager
{
private:
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

public:
    // Initialize the temperature manager
    static void begin()
    {
        // Initialize SSR pin
        pinMode(SSR_PIN, OUTPUT);
        digitalWrite(SSR_PIN, LOW);

        // Initialize with a first reading
        currentTemp = thermocouple.readCelsius();

        // Ensure heater is inactive on startup
        heaterActive = false;
        Serial.println("TempManager initialized - Heater inactive");
    }

    // Update temperature reading if interval has passed
    static void update()
    {
        unsigned long currentTime = millis();

        // Update temperature reading
        if (currentTime - lastReadTime >= READ_INTERVAL)
        {
            currentTemp = thermocouple.readCelsius();
            lastReadTime = currentTime;

            // Debug temperature readings
            Serial.printf("Temperature: %.2f°C\n", currentTemp);
        }

        // Check if heater duration has elapsed
        if (heaterActive && heaterDuration > 0)
        {
            if (currentTime - heaterStartTime >= heaterDuration)
            {
                deactivateHeater();
                Serial.println("Auto-shutoff: Heater duration elapsed");
            }
        }

        // Only update SSR if heater is active
        if (heaterActive)
        {
            updateSSR(currentTime);
        }
        else if (ssrState)
        {
            // If heater is inactive but SSR is still on, turn it off
            ssrState = false;
            digitalWrite(SSR_PIN, LOW);
            Serial.println("Safety check: Disabled SSR because heater is inactive");
        }
    }

    // Activate the heater with specified duty cycle and duration
    static void activateHeater(float duty, unsigned long duration_ms = 0)
    {
        setDutyCycle(duty);
        heaterActive = true;
        heaterStartTime = millis();
        heaterDuration = duration_ms;

        Serial.printf("Heater activated - Duty cycle: %.2f%%, Duration: %lu ms\n",
                      duty * 100, duration_ms);
    }

    // Deactivate the heater
    static void deactivateHeater()
    {
        heaterActive = false;
        // Ensure SSR is off
        ssrState = false;
        digitalWrite(SSR_PIN, LOW);

        Serial.println("Heater deactivated");
    }

    // Check if heater is currently active
    static bool isHeaterActive()
    {
        return heaterActive;
    }

    // Set duty cycle (0.0 to 1.0)
    static void setDutyCycle(float duty)
    {
        // Constrain duty cycle between 0.0 and 1.0
        dutyCycle = constrain(duty, 0.0, 1.0);

        // For 100% duty cycle, set timeOff to 0 to keep SSR always on
        if (dutyCycle >= 0.99)
        {
            timeOff = 0; // Always on
        }
        else if (dutyCycle <= 0.01)
        {
            timeOff = 10000; // 10 seconds off when essentially zero
        }
        else
        {
            // TIME_ON / dutyCycle = total cycle time
            // timeOff = total cycle time - TIME_ON
            timeOff = (TIME_ON / dutyCycle) - TIME_ON;
        }

        Serial.printf("Duty cycle set to: %.2f%%, Time off: %lu ms\n",
                      dutyCycle * 100, timeOff);
    }

    // Get current duty cycle
    static float getDutyCycle()
    {
        return dutyCycle;
    }

    // Get current temperature in Celsius
    static float getTemperature()
    {
        return currentTemp;
    }

    // Get temperature as formatted string with C suffix
    static String getTemperatureString()
    {
        // Format with 2 decimal places
        return String(currentTemp, 2) + "C";
    }

    // Get SSR state
    static bool getSSRState()
    {
        return ssrState;
    }

private:
    // Update SSR state based on custom PWM logic
    static void updateSSR(unsigned long currentTime)
    {
        unsigned long elapsedTime = currentTime - ssrLastToggleTime;

        // For 100% duty cycle, keep SSR on continuously
        if (dutyCycle >= 0.99)
        {
            if (!ssrState)
            {
                ssrState = true;
                digitalWrite(SSR_PIN, HIGH);
                ssrLastToggleTime = currentTime;

                Serial.println("SSR turned ON (100% duty)");
            }

            return;
        }

        if (ssrState)
        {
            // If SSR is ON and the on-time has elapsed, turn it OFF
            if (elapsedTime >= TIME_ON)
            {
                ssrState = false;
                digitalWrite(SSR_PIN, LOW);
                ssrLastToggleTime = currentTime;

                Serial.println("SSR turned OFF");
            }
        }
        else
        {
            // If SSR is OFF and the off-time has elapsed, turn it ON
            if (elapsedTime >= timeOff)
            {
                ssrState = true;
                digitalWrite(SSR_PIN, HIGH);
                ssrLastToggleTime = currentTime;
                Serial.println("SSR turned ON");
            }
        }
    }
};