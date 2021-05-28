/* Headers */ 
#include <WiFi.h> 
#include <PubSubClient.h>
#include <WiFiManager.h>         
#include <WiFi.h>
#include <esp_now.h>
#include <SPI.h>
#include <Wire.h>
#include <i2cdetect.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Arduino.h>
#include "ArduinoNvs.h"
#include <ArduinoOTA.h>
#include <HTTPClient.h>
#include <ESPmDNS.h>
#include <esp32fota.h>
#include <esp_task_wdt.h> 
#include "globalvar.h"
#include "globaldef.h"
#include <Ultrasonic.h>
#include "esp_wpa2.h"

/*Declaration*/
WiFiClientSecure client2; //Inicializar cliente wifi
WiFiUDP ntpUDP; //NTP-UDP
NTPClient timeClient(ntpUDP);//Cliente NTP
secureEsp32FOTA secureEsp32FOTA("esp32-fota-https", V_FIRMWARE);
WiFiClient espClient;
PubSubClient MQTT(espClient);
TaskHandle_t task_low_serial;
TaskHandle_t task_low_led;
TaskHandle_t task_low;
void vLow(void *pvParameters);
void vLowSerial(void *pvParameters);
void vLowLED(void *pvParameters);
void init_serial(void);
void init_wifi(void);
void init_mqtt(void);
void reconnect_wifi(void); 
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void verifica_conexoes_wifi_mqtt(void);
void RightMotorPWM(String action, int dutyCycle, int delayTime);
void RightMotorBinary(String action);
void LeftMotorPWM(String action, int dutyCycle, int delayTime);
void LefttMotorBinary(String action);
int getDistanceBack();
int getDistanceFront();
int getDistanceLeft();
int getDistanceRight();
void checkPerimeter();



/*
Init the serial communication.
*/ 
void init_serial() 
{
    Serial.begin(115200);
}

/*
Init the wifi.
*/ 
void init_wifi(void) 
{
    delay(10);
    Serial.print("Trying to connect with: ");
    reconnect_wifi();
}

/*
Init the mqtt connection.
*/  
void init_mqtt(void) 
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT); 
    MQTT.setCallback(mqtt_callback);            
}


/*
If receives any message on mqtt, executes this function.
*/  
void mqtt_callback(char* topic, byte* payload, unsigned int length) 
{
    String msg;
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
    }
    Serial.print("[MQTT] Received message: ");
    Serial.println(msg);     
}

/*
Check if mqtt connection is ok. If not, try to reconnect.
*/  
void reconnect_mqtt(void) 
{
    while (!MQTT.connected()) 
    {
        Serial.print("Trying to connect with broker ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) 
        {
            Serial.println("Connected successfully with broker!");
            MQTT.subscribe(TOPICO_SUBSCRIBE); 
        } 
        else
        {
            Serial.println("Failed to connect on broker.");
            Serial.println("There will be a new attempt in 2 seconds.");
            delay(2000);
        }
    }
}

/*
Check if wifi connection is ok. If not, try to reconnect.
*/  
void reconnect_wifi() 
{
    if (WiFi.status() == WL_CONNECTED)
        return;
    WiFi.disconnect(true);
    delay(1000);
    WiFi.mode(WIFI_STA);
    delay(1000);
  
  
    WiFiManager wifiManager;
    WiFi.setAutoConnect(true);
    wifiManager.setTimeout(80);
    wifiManager.setBreakAfterConfig(true);
    wifiManager.setConfigPortalTimeout(180);
  
    if (!wifiManager.autoConnect("Richter", "12345678")) {
      Serial.println("Failed to connect. Restarting...");
      delay(100);
      if (isWaitingForOta == 0) {
        ESP.restart();
      }
    }
    Serial.println();
    Serial.print("Connected.");

}

/*
Check all connections.
*/
void verifica_conexoes_wifi_mqtt(void)
{
    reconnect_wifi(); 
    if (!MQTT.connected()) 
        reconnect_mqtt(); 
} 
 
