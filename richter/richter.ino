#include "prototypes.h"


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

