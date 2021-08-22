
/*Configure all pins.*/
void configurePins(){
  Serial.println("[GPIO] Starting to configure GPIOS.");
  pinMode(motor1Pin1Left, OUTPUT);
  pinMode(motor1Pin2Left, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  pinMode(motor2Pin1Right, OUTPUT);
  pinMode(motor2Pin2Right, OUTPUT);
  pinMode(enable2Pin, OUTPUT);

  /*Configure LED PWM functionalitites*/
  ledcSetup(pwmChannel, freq, resolution);
  ledcSetup(pwmChannel2, freq, resolution);
  
  /*Attach the channel to the GPIO to be controlled*/
  ledcAttachPin(enable1Pin, pwmChannel);
  ledcAttachPin(enable2Pin, pwmChannel2);

  Serial.println("[GPIO] GPIO's configured.");
}

/*Configure all tasks*/
void configureTasks(){
  Serial.println("[TASK] Starting to configure tasks.");
  xTaskCreatePinnedToCore(vLowSerial,"vLowSerial",2048,NULL,PRIORITY_2, &task_low_serial, CORE_0);
  xTaskCreatePinnedToCore(vLowLED,"vLowLED",1024,NULL,PRIORITY_1,&task_low_led, CORE_1);
  xTaskCreatePinnedToCore(vLowTimer,"vLowTimer",1024,NULL,PRIORITY_4,&task_timer, CORE_1);
  xTaskCreatePinnedToCore(vLowTelemetry,"vLowTelemetry",8192,NULL,PRIORITY_3, &task_low_telemetry, CORE_0);
  Serial.println("[TASK] Tasks configured.");
}

/*Do all initial setups*/
void initRichter(){

  /*Init serial.*/
  init_serial();

  Serial.println("[BOOT] Starting to load configurations.");

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

  /*Configure the sleep mode in case we need.*/
  Serial.println("[BOOT] Configuring sleep mode.");
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("[BOOT] Sleep mode configured.");
  
  /*Configure PID functionalities*/
  Serial.println("[PID] Configuring PID.");
  myPID.SetMode(AUTOMATIC);
  Serial.println("[PID] PID configured.");

  /*Init wifi*/
  init_wifi();

  /*Interrupts*/
  attachInterrupt(pinRecharge, rechargebleISR, RISING); 

  /*Tasks*/
  configureTasks();

  /*Publish a topic to tell he's alive.*/
  Serial.println("[BOOT] Richter has been initialized successfully.");

  /*Init the watchdog interface.*/
  esp_task_wdt_init(8, true);
  esp_task_wdt_add(NULL);

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
    Serial.println("[WIFI] Trying to connect to WiFi network.");
    r = reconnect_wifi();
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
    Serial.println("[MQTT] Received message: ");
    Serial.println(msg);     
}

/*
Check if mqtt connection is ok. If not, try to reconnect.
*/  
void reconnect_mqtt(void) 
{
    while (!MQTT.connected()) 
    {
        Serial.println("[MQTT] Trying to connect with broker ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) 
        {
            Serial.println("[MQTT] Connected successfully with broker!");
            MQTT.subscribe(TOPICO_SUBSCRIBE); 
        } 
        else
        {
            Serial.println("[MQTT] Failed to connect on broker.");
            Serial.println("[MQTT] There will be a new attempt in 2 seconds.");
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
    }
}

/*
Check if wifi connection is ok. If not, try to reconnect.
*/  
int reconnect_wifi() 
{
    if (WiFi.status() == WL_CONNECTED)
        return SUCCESFULL;

    WiFi.begin(ssid, password);
 
    while (WiFi.status() != WL_CONNECTED && timeoutWifi) {
      timeoutWifi--;
      delay(500);
    }

    if (WiFi.status() != WL_CONNECTED){
      Serial.println("[WIFI] Failed to connect to wifi network");
      return ERROR;
    }

    Serial.println("[WIFI] Connected to the WiFi network");
    Serial.println("[WIFI] IP address: ");
    Serial.println(WiFi.localIP());

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
  Serial.println("[NVS] Getting data from NVS.");
  id = NVS.getInt("id");  /*Unique identifies of the robot*/
  Serial.println("[NVS] Data collected.");
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

  while (true) {

    switch (estado){
      case WORKING:

      break;
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

/*Timer*/
void vLowTimer(void *pvParameters) {
  while (true) {
    Serial.println("[TIMER_1S] Finished execution.");
    vTaskDelay(pdMS_TO_TICKS(1000));}
}


/*Telemetry*/
void vLowTelemetry(void *pvParameters) {

  while (true) {

    reconnect_wifi();

    accReading = getAccData();
    dhtReading = getHumidTemp();

    pkt = "{\"service\":\"richter\",";
    pkt += "\"tmstp\":\"" + String(millis()) + "\",";
    pkt += "\"sensors\":{\"humidty\":" + String(dhtReading.humidity) + ",";
    pkt += "\"temperature\":" + String(dhtReading.temperature) + ",";
    pkt += "\"xAxisAcc\":" + String(accReading.xAxis) + ",";
    pkt += "\"yAxisAcc\":" + String(accReading.yAxis) + ",";
    pkt += "\"zAxisAcc\":" + String(accReading.zAxis) + "}}";
    Serial.println("[PACKET] " + pkt);
    
    vTaskDelay(pdMS_TO_TICKS(15000));}
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
