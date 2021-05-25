#include "prototypes.h"

void setup() 
{

  /*Configure all pins as input, output...*/
  configurePins();
  
  /*Configure LED PWM functionalitites*/
  ledcSetup(pwmChannel, freq, resolution);
  ledcSetup(pwmChannel2, freq, resolution);
  
  /*Attach the channel to the GPIO to be controlled*/
  ledcAttachPin(enable1Pin, pwmChannel);
  ledcAttachPin(enable2Pin, pwmChannel2);

  /*Configure the sleep mode in case we need.*/
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

  /*Init serial.*/
  init_serial();
  
  /*Create tasks to control the serial communication and LEDS.*/
  xTaskCreate(vLowSerial, "vLowSerial", 10000, NULL, 0, &task_low_serial);
  xTaskCreate(vLowLED, "vLowLED", 10000, NULL, 0, &task_low_led);

  /*Init wifi*/
  init_wifi();

  /*Init MQTT*/
  init_mqtt();

  /*Publish a topic to tell he's alive.*/
  MQTT.publish(TOPICO_PUBLISH, "Richter has been initialized successfully.");
  Serial.println("Richter has been initialized successfully.");
  
}

void loop() 
{  
  /*Check the wifi and mqtt connection, and keeps the mqtt connection alive.*/ 
  verifica_conexoes_wifi_mqtt();
  MQTT.loop();

  /*Check if richter is charging. If true, goes to sleep.*/
  if(isCharging){
    esp_deep_sleep_start();
  }

    
}
