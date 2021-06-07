
/*Configure all pins.*/
void configurePins(){
  pinMode(motor1Pin1Left, OUTPUT);
  pinMode(motor1Pin2Left, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  pinMode(motor2Pin1Right, OUTPUT);
  pinMode(motor2Pin2Right, OUTPUT);
  pinMode(enable2Pin, OUTPUT);
}

/*ESP-NOW*/
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status:");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

int configESPNOW(){
  esp_now_register_send_cb(OnDataSent);
  
  /*Register peer*/
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  /*Add peer*/
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return 0;
  }

  return 1;
}

/*Values from NVS*/
void getValuesFromNVS(){

  id = NVS.getInt("id");  /*Unique identifies of the robot*/
  
}

/*Tasks - LED*/
void vLowLED(void *pvParameters) {
  const int freq = 5000;
  const int ledChannel = 0;
  const int resolution = 8;
  const int ledChannel2 = 1;
  esp_task_wdt_init(8, true);
  esp_task_wdt_add(NULL);
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(ledPin, ledChannel);
  ledcSetup(ledChannel2, freq, resolution);
  ledcAttachPin(ledPin2, ledChannel2);

  
  while (true) {
    esp_task_wdt_reset();
    if (estado == WORKING){
      for(int i = 0; i< 255; i++){
        ledcWrite(ledChannel, i);
        esp_task_wdt_reset();
        delay(5);
      }

      for(int i = 255; i> 0; i--){
        ledcWrite(ledChannel, i);
        esp_task_wdt_reset();
        delay(5);
      }
    } else {
      delay(5);
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

/*Get actual time.*/
String getCurrentTime(){
  timeoutHorario = 60;
  Serial.println("Getting actual date");
  timeClient.begin();
  timeClient.setTimeOffset(-10800); 
  while (!timeClient.update()) {
    timeClient.forceUpdate();
    delay(100);
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
