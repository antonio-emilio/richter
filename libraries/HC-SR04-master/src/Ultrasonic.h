#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "Arduino.h"

#define SOUND_SPEED 343.0 // m/s
#define US_TO_S 1e-6
#define M_TO_CM 1e2
#define CM_TO_IN 1.0/2.54

class Ultrasonic
{

  public:
    Ultrasonic();
    Ultrasonic(uint8_t trigPin, uint8_t echoPin);
    void setTrigPin(uint8_t trigPin_);
    void setEchoPin(uint8_t echoPin_);
    void measure();
    float get_cm();
    float get_inch();


  private:
    uint8_t trigPin;
    uint8_t echoPin;
    unsigned long echoDelay;

  
};


#endif
