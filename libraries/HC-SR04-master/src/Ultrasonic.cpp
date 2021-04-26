#include "Ultrasonic.h"


Ultrasonic::Ultrasonic()
{
  
}

Ultrasonic::Ultrasonic(uint8_t trigPin_, uint8_t echoPin_)
{
  setTrigPin(trigPin_);
  setEchoPin(echoPin_);
}

void Ultrasonic::setTrigPin(uint8_t trigPin_)
{
  trigPin = trigPin_;
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
}

void Ultrasonic::setEchoPin(uint8_t echoPin_)
{
  echoPin = echoPin_;
  pinMode(echoPin, INPUT);
}

void Ultrasonic::measure()
{
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  echoDelay = pulseIn(echoPin, HIGH);
}


float Ultrasonic::get_cm()
{
    return (float)(0.5*echoDelay)*SOUND_SPEED*US_TO_S*M_TO_CM;   // distance is measured "twice" because of bouncing
}


float Ultrasonic::get_inch()
{
    return (float)(0.5*echoDelay)*SOUND_SPEED*US_TO_S*M_TO_CM*CM_TO_IN; 
}

