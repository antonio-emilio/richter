#include "MICS6814.h"

#define PIN_CO  A0
#define PIN_NO2 A1
#define PIN_NH3 A2

MICS6814 gas(PIN_CO, PIN_NO2, PIN_NH3);

void setup() {
  Serial.begin(9600);

  Serial.println("MICS-6814 Sensor Sample");
  Serial.print("Calibrating Sensor");

  gas.calibrate();

  Serial.println("OK!");
}

void loop() {
  Serial.print("NH3: ");
  Serial.print(gas.measure(NH3));
  Serial.println("ppm");
  Serial.print("CO: ");
  Serial.print(gas.measure(CO));
  Serial.println("ppm");
  Serial.print("NO2: ");
  Serial.print(gas.measure(NO2));
  Serial.println("ppm");
  delay(1000);
}
