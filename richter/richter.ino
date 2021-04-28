#include "prototypes.h"

void setup() 
{
  // sets the pins as outputs:
  pinMode(motor1Pin1Left, OUTPUT);
  pinMode(motor1Pin2Left, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  pinMode(motor2Pin1Right, OUTPUT);
  pinMode(motor2Pin2Right, OUTPUT);
  pinMode(enable2Pin, OUTPUT);
  
  // configure LED PWM functionalitites
  ledcSetup(pwmChannel, freq, resolution);
  ledcSetup(pwmChannel2, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(enable1Pin, pwmChannel);
  ledcAttachPin(enable2Pin, pwmChannel2);

  Serial.begin(115200);
  init_serial();
  init_wifi();
  init_mqtt();



  xTaskCreate(vLowSerial, "vLowSerial", 10000, NULL, 0, &task_low_serial);
  xTaskCreate(vLowLED, "vLowLED", 10000, NULL, 0, &task_low_led);

  MQTT.publish(TOPICO_PUBLISH, "Richter has been initialized successfully.");
  Serial.println("Richter has been initialized successfully.");
  
}

void loop() 
{   
  verifica_conexoes_wifi_mqtt();
  MQTT.loop();

    
}
