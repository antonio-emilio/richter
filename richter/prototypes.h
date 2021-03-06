/* Headers */ 
#include <WiFi.h> 
#include <PubSubClient.h>
#include <WiFiManager.h>         
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
#include <DHT.h>
#include <BH1750.h>
#include <ESPmDNS.h>
#include <esp32fota.h>
#include <esp_task_wdt.h> 
#include "globalvar.h"
#include "globaldef.h"
#include <Ultrasonic.h>
#include <PID_v1.h>
#include "esp_wpa2.h"
#include <esp_wifi.h>
#include "FS_File_Record.h"  // Nossa lib personalizada do SPIFFS
#include <lmic.h>
#include <hal/hal.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <HardwareSerial.h> 
#include "GPRS_Functions.h"

/*Declaration*/
WiFiClientSecure client2; 
BH1750 lightMeter (0x23); 
WiFiUDP udp;
FS_File_Record ObjFS("/log.bin", 20);
WiFiUDP ntpUDP; 
NTPClient timeClient(ntpUDP);
WiFiClient espClient;
PubSubClient MQTT(espClient);
TaskHandle_t task_low_serial;
TaskHandle_t task_low_led;
TaskHandle_t task_low;
TaskHandle_t task_timer;
TaskHandle_t task_low_telemetry;
DHT dht(dhtPin, dhtType);
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
HardwareSerial mySerial(1); 


/*utils.ino*/
void vLow(void *pvParameters);
void vLowSerial(void *pvParameters);
void vLowLED(void *pvParameters); 
void vLowTimer(void *pvParameters);
void vLowTelemetry(void *pvParameters);
void configureTasks();
secureEsp32FOTA secureEsp32FOTA("esp32-fota-https", V_FIRMWARE);

/*sensors.ino*/
int initAccelerometer();
AccMessage getAccData();
int initDHT();
HumidTemp getHumidTemp();
int initBH1750();
float getLuminosity();

/*udp.ino*/
void initUDP();
int sendUDPmsg(String replyPacket);
void checkUDPmsg();

/*richter.ino*/
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

/*WiFi*/
WiFiClient client; 

/*prototypes.ino*/
void init_serial(void);
void init_wifi(void);
void init_mqtt(void);
int reconnect_wifi(void); 
void mqtt_callback(char* topic, byte* payload, unsigned int length);
int checkConnections(void);

/*motor.ino*/
void RightMotorPWM(String action, int dutyCycle, int delayTime);
void RightMotorBinary(String action);
void LeftMotorPWM(String action, int dutyCycle, int delayTime);
void LefttMotorBinary(String action);

/*ultrassonic.ino*/
int getDistanceBack();
int getDistanceFront();
int getDistanceLeft();
int getDistanceRight();
int checkPerimeter();
