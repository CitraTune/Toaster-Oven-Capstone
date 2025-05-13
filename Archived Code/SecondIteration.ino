#include <MAX6675.h>
#include <PID_v1.h>


// Pin defsetupions
#define SSR_PIN 9      // PWM output to SSR
#define MAX6675_CLK 13
#define MAX6675_CS 10
#define MAX6675_DO 12


// Thermocouple setup
MAX6675 thermocouple(MAX6675_CLK, MAX6675_CS, MAX6675_DO);


// PID variables
double Setpoint = 100.0;  // Target temperature (change as needed)
double Input, Output;
double Kp = 2.0, Ki = 5.0, Kd = 1.0;  // PID tuning parameters


// PID setup
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);


void setup() {
  Serial.begin(9600);
  pinMode(SSR_PIN, OUTPUT);


  // Start PID control
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(0, 255); // Limit to PWM range
}


void loop() {
  Input = thermocouple.readCelsius(); // Read temperature
  if (Input < Setpoint - 20) {  // Full power when way below target
    analogWrite(SSR_PIN, 255);
  } 
  else {
    myPID.Compute();  // Enable PID near target
  }






  analogWrite(SSR_PIN, (int)Output); // Control SSR via PWM


  // Debugging output
  Serial.print("Temperature: "); Serial.print(Input);
  Serial.print(" °C, Output: "); Serial.println(Output);


  delay(1000); // Adjust as needed
}
