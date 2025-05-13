#include <MAX6675.h>


// Pin defsetupions
#define SSR_PIN 9  // SSR control pin
#define SO_PIN 4   // MAX6675 SO pin
#define CS_PIN 5   // MAX6675 CS pin
#define SCK_PIN 6  // MAX6675 SCK pin


// Thermocouple setup
MAX6675 thermocouple(SCK_PIN, CS_PIN, SO_PIN);


void setup() {
  Serial.begin(9600);
  pinMode(SSR_PIN, OUTPUT);
 
  Serial.println("Starting heating test...");
 
  // Read setupial room temperature
  double roomTemp = thermocouple.readCelsius();
  double targetTemp = roomTemp + 10;  // Target is 10°C above room temp


  Serial.print("Room Temperature: ");
  Serial.print(roomTemp);
  Serial.println(" °C");
  Serial.print("Target Temperature: ");
  Serial.print(targetTemp);
  Serial.println(" °C");


  // Start heating
  Serial.println("Heating for 100 seconds...");
  digitalWrite(SSR_PIN, HIGH);
  delay(100000);  // 100 seconds


  // Stop heating
  digitalWrite(SSR_PIN, LOW);
  Serial.println("Heating stopped. Waiting for temperature increase...");


  // Start timing how long it takes to reach target temperature
  unsigned long startTime = millis();
  double temp = thermocouple.readCelsius();
 
  while (temp < targetTemp) {
    temp = thermocouple.readCelsius();
    Serial.print("Current Temp: ");
    Serial.print(temp);
    Serial.println(" °C");
    delay(500);  // Check every 0.5 seconds
  }


  // Compute elapsed time
  unsigned long elapsedTime = millis() - startTime;
  Serial.print("Time to reach ");
  Serial.print(targetTemp);
  Serial.print(" °C: ");
  Serial.print(elapsedTime / 1000);
  Serial.println(" seconds");
}


void loop() {
  // Do nothing after test completes
}


