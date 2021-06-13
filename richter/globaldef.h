#define TOPICO_SUBSCRIBE "receiverCommands"   
#define TOPICO_PUBLISH   "senderCommands"  
#define ID_MQTT  "richter"     
#define V_FIRMWARE 7  /*Firmware version of OTA updates.*/

/*Status*/
#define WIFI 1
#define WORKING 2

/*Pins*/
#define ledPin 15
#define ledPin2 2
#define PORTA_TRIGGER1   13
#define PORTA_ECHO1      14
#define PORTA_TRIGGER2   4
#define PORTA_ECHO2      5
#define PORTA_TRIGGER3   13
#define PORTA_ECHO3      14
#define PORTA_TRIGGER4   4
#define PORTA_ECHO4      5
#define pinRecharge      4

/*Cores*/
#define CORE_0 0
#define CORE_1 1

/*States*/
#define SUCCESFULL 1
#define ERROR 0
#define STATE_0 0
#define STATE_1 1
#define STATE_2 2

/*Priorities*/
#define PRIORITY_1 1
#define PRIORITY_2 2
#define PRIORITY_3 3
#define PRIORITY_4 4
#define PRIORITY_5 5
#define PRIORITY_6 6
#define PRIORITY_7 7
#define PRIORITY_8 8
#define PRIORITY_9 9

/*Intervals*/
#define checkInterval 600000

/*SPI*/
#define SPISPEED 8000000 

/*DeepSleep*/
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  18000        /* Time ESP32 will go to sleep (in seconds) */
