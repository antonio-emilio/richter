#include "Ultrasonic.h"

#define TRIG_PIN 5
#define ECHO_PIN 6

#define BAUDRATE 115200

Ultrasonic us(TRIG_PIN, ECHO_PIN);
float dt;
int startTime;

void setup() {

  Serial.begin(115200);
  while(!Serial)
    ;

  startTime = millis();  
}

void loop() {


  us.measure();
  dt = millis() - startTime;
  dt /= 1000;
  
  Serial.println(us.get_cm(), 3);
  
  delay(50);
}
