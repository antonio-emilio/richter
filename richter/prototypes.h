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

/*Declaração*/
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


 
void init_serial() 
{
    Serial.begin(115200);
}
 
void init_wifi(void) 
{
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    reconnect_wifi();
}
  
void init_mqtt(void) 
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT); 
    MQTT.setCallback(mqtt_callback);            
}
  
void mqtt_callback(char* topic, byte* payload, unsigned int length) 
{
    String msg;
 
    //obtem a string do payload recebido
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
    }
    Serial.print("[MQTT] Mensagem recebida: ");
    Serial.println(msg);     
}
  
void reconnect_mqtt(void) 
{
    while (!MQTT.connected()) 
    {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) 
        {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE); 
        } 
        else
        {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Havera nova tentatica de conexao em 2s");
            delay(2000);
        }
    }
}
  
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
    wifiManager.setConfigPortalTimeout(80);
  
    if (!wifiManager.autoConnect("Richter", "12345678")) {
      Serial.println("Falhou para se conectar... Reiniciando.");
      delay(100);
      if (isWaitingForOta == 0) {
        ESP.restart();
      }
    }
    Serial.println();
    Serial.print("Conectado com sucesso na rede ");

}

void verifica_conexoes_wifi_mqtt(void)
{
    reconnect_wifi(); 
    if (!MQTT.connected()) 
        reconnect_mqtt(); 
} 
 
