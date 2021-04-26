#include "prototypes.h"



void setup() 
{
    init_serial();
    init_wifi();
    init_mqtt();
}

void loop() 
{   
    verifica_conexoes_wifi_mqtt();
    MQTT.publish(TOPICO_PUBLISH, "ESP32 se comunicando com MQTT");
    MQTT.loop();
    delay(1000);   
}
