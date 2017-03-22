#include "Ultrasonic.h"
Ultrasonic ultrasonic(12,13);
//const int temperature = A0;
//const int potenciometer = 8;

void setup() {
  Serial.begin(9600);
  Serial.print("testing...");
//  pinMode(temperature, INPUT);
//  pinMode(potenciometer, INPUT);
}

void loop()
{
  Serial.print("To an object of: ");
  Serial.println(ultrasonic.Ranging(CM));
//  Serial.print("Temperature: ");
//  Serial.println(analogRead(temperature));
//  Serial.print("Value by potenciometer: ");
//  Serial.println(digitalRead(potenciometer));
  delay(1000);
}
