#include "prototypes.h"

/*Interrupt of recharging*/
void IRAM_ATTR rechargebleISR()
{
  isCharging = 1;
}

/*Run once.*/
void setup() 
{
  /*Do all initial setups.*/
  initRichter(); 
}

/*While loop.*/
void loop() 
{
  /*Resets the watchdog interface.*/
  esp_task_wdt_reset(); 

  /*Run the state machine.*/
  SM_s1();

}

/*State machine workflow.*/
void SM_s1() {

  /*State machine selection*/
  switch (state_s1) {
    case STATE_0: 

      /*Check the wifi and mqtt connection, and keeps the mqtt connection alive.*/ 
      r = checkConnections();
      MQTT.loop();

      if (r == SUCCESFULL) {state_s1 = 1;} 
    break;

    case STATE_1: 

      /*Evaluate the perimeter and check for too close objects.*/
      r = checkPerimeter();

      /*Calculate the PID.*/
      myPID.Compute();

      if (r == SUCCESFULL) {state_s1 = 2;} 
    break;

    case STATE_2: 

      /*Check if richter is charging. If true, check for updates and goes to sleep.*/
      if(isCharging){
        checkUpdates(); 
        sendMessageESPNOW(RECHARGING, true);  
        esp_deep_sleep_start();
      }

      if (r == SUCCESFULL) {state_s1 = 0;} 
    break;

    default:
      /*Something went wrong?*/
      break;
  }
  
}
