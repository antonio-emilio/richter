//#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_MODEM_SIM808
// #define TINY_GSM_MODEM_SIM900
// #define TINY_GSM_MODEM_UBLOX
// #define TINY_GSM_MODEM_BG96
// #define TINY_GSM_MODEM_A6
// #define TINY_GSM_MODEM_A7
// #define TINY_GSM_MODEM_M590
// #define TINY_GSM_MODEM_ESP8266
// #define TINY_GSM_MODEM_XBEE

#include <TinyGsmClient.h> //Biblioteca que configura o modem GSM

const char apn[]  = "APN"; //YourAPN
const char user[] = "";
const char pass[] = "";
HardwareSerial Serial_SIM_Module(1);

TinyGsm sim808(Serial_SIM_Module);

bool networkConnect()
{
  Serial.print("Waiting for network...");
  if (!sim808.waitForNetwork()) 
  {
    Serial.println(" fail");
    return false;
  }
  
  Serial.println(" OK");
  Serial.print("Connecting to ");
  Serial.print(apn);
  if (!sim808.gprsConnect(apn, user, pass)) 
  {
    Serial.println(" fail");
    return false;
  }
  Serial.println(" OK");
  return true;

}

//Configura o modem GPRS
bool modemConfig(HardwareSerial mySerial)
{
  Serial_SIM_Module = mySerial;
  //Inicia modem
  Serial.println("Setting modem...");  
  
  if(!sim808.restart()) 
    return false;

  //Conecta na rede
  return networkConnect();
}

