
/*Configure all pins.*/
void configurePins(){
  pinMode(motor1Pin1Left, OUTPUT);
  pinMode(motor1Pin2Left, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  pinMode(motor2Pin1Right, OUTPUT);
  pinMode(motor2Pin2Right, OUTPUT);
  pinMode(enable2Pin, OUTPUT);
}

/*Do all initial setups*/
void initRichter(){

  /*Init the watchdog interface.*/
  esp_task_wdt_init(8, true);
  esp_task_wdt_add(NULL);

  /*Init serial.*/
  init_serial();

  /*Init non vollatile memory*/
  NVS.begin(); 

  /*Get values from NVS*/  
  getValuesFromNVS();

  /*Configure all pins as input, output...*/
  configurePins();

  /*Configure SPI*/
  SPI.begin(18, 19, 23, SS); 

  /*Configure I2C*/
  Wire.begin(); 
  i2cdetect();       
  
  /*Configure LED PWM functionalitites*/
  ledcSetup(pwmChannel, freq, resolution);
  ledcSetup(pwmChannel2, freq, resolution);
  
  /*Attach the channel to the GPIO to be controlled*/
  ledcAttachPin(enable1Pin, pwmChannel);
  ledcAttachPin(enable2Pin, pwmChannel2);

  /*Configure the sleep mode in case we need.*/
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  
  /*Create tasks to control the serial communication and LEDS.*/
  xTaskCreatePinnedToCore(vLowSerial,"vLowSerial",8192,NULL,PRIORITY_2, &task_low_serial, CORE_0);
  xTaskCreatePinnedToCore(vLowLED,"vLowLED",1024,NULL,PRIORITY_1,&task_low_led, CORE_1);

  /*Configure PID functionalities*/
  myPID.SetMode(AUTOMATIC);

  /*Init wifi*/
  init_wifi();

  /*Starts socket connection*/
  if (!client.connect("yourServer", 1883)) {
    Serial.println("Socket connection has failed.");
    return;
  }

  /*Init MQTT*/
  init_mqtt();
  
  /*Init ESP-NOW*/
  if (esp_now_init() != ESP_OK) {
    socketPrint("Error initializing ESP-NOW");
    return;
  }

  /*Make all configs to use ESP-NOW*/
  configESPNOW();

  /*Interrupts*/
  attachInterrupt(pinRecharge, rechargebleISR, RISING); 

  /*Publish a topic to tell he's alive.*/
  MQTT.publish(TOPICO_PUBLISH, "Richter has been initialized successfully.");
  socketPrint("Richter has been initialized successfully.");

}


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
    vTaskDelay(pdMS_TO_TICKS(100));
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
            vTaskDelay(pdMS_TO_TICKS(2000));
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
    vTaskDelay(pdMS_TO_TICKS(1000));
    WiFi.mode(WIFI_STA);
    vTaskDelay(pdMS_TO_TICKS(1000));
  
  
    WiFiManager wifiManager;
    WiFi.setAutoConnect(true);
    wifiManager.setTimeout(80);
    wifiManager.setBreakAfterConfig(true);
    wifiManager.setConfigPortalTimeout(180);
  
    if (!wifiManager.autoConnect("Richter", "12345678")) {
      Serial.println("Failed to connect. Restarting...");
      vTaskDelay(pdMS_TO_TICKS(100));
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
int checkConnections(void)
{
    reconnect_wifi(); 
    if (!MQTT.connected()) 
        reconnect_mqtt(); 

    return SUCCESFULL;
} 
 



/*Values from NVS*/
void getValuesFromNVS(){
  id = NVS.getInt("id");  /*Unique identifies of the robot*/
}

/*Socket - Serial*/
void socketPrint(String cmd){
  if(debugSocket)
    client.print(cmd);
  else 
    Serial.println(cmd);
}

/*Tasks - LED*/
void vLowLED(void *pvParameters) {
  const int freq = 5000;
  const int ledChannel = 0;
  const int resolution = 8;
  const int ledChannel2 = 1;

  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(ledPin, ledChannel);
  ledcSetup(ledChannel2, freq, resolution);
  ledcAttachPin(ledPin2, ledChannel2);

  
  while (true) {

    /*Oscilates the led.*/
    if (estado == WORKING){
      for(int i = 0; i< 255; i++){
        ledcWrite(ledChannel, i);
        vTaskDelay(pdMS_TO_TICKS(5));
      }

      for(int i = 255; i> 0; i--){
        ledcWrite(ledChannel, i);
        vTaskDelay(pdMS_TO_TICKS(5));
      }
    } else {
      vTaskDelay(pdMS_TO_TICKS(100));
    }
  }
}

/*Tasks*/
void vLowSerial(void *pvParameters) {
  while (true) {
    if (Serial.available()) {
      input = Serial.readStringUntil('\n');
      if (input.equals("help")) {
        Serial.println(
            "~~~~~~~~~~~~~~~Richter ~~~~~~~~~~~~~~~");
        Serial.println("List of available commands:");
        Serial.println(
            "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

      }
      
    }vTaskDelay(pdMS_TO_TICKS(100));}
    
}

/*Check for new updates*/
void checkUpdates(){
  secureEsp32FOTA._host="raw.githubusercontent.com"; //e.g. example.com
  secureEsp32FOTA._descriptionOfFirmwareURL="/User/Repository/main/firmware.json"; 
  secureEsp32FOTA._certificate=test_root_ca;
  secureEsp32FOTA.clientForOta=client2;

  bool shouldExecuteFirmwareUpdate=secureEsp32FOTA.execHTTPSCheck();
  if(shouldExecuteFirmwareUpdate)
  {
    Serial.println("Firmware update available!");
    secureEsp32FOTA.executeOTA();
  }
}

/*Get actual time.*/
String getCurrentTime(){
  timeoutHorario = 60;
  Serial.println("Getting actual date");
  timeClient.begin();
  timeClient.setTimeOffset(-10800); 
  while (!timeClient.update()) {
    timeClient.forceUpdate();
    vTaskDelay(pdMS_TO_TICKS(100));
    timeoutHorario--;
    if (timeoutHorario == 0) {
      break;
    }
  }

  formattedDate = timeClient.getFormattedDate(); 
  if (timeoutHorario == 0) {
    Serial.println("Wasn't possible to get actual date.");
    return "";
  } else {
    Serial.println("Date acquired successfully.");
    return formattedDate;
  }
}
