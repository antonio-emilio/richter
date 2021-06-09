/*Send a message throug ESPNOW.*/
void sendMessageESPNOW(String msg, bool batteryLevel){
  myData.cmd = "THIS IS A CHAR";
  myData.isBatteryLow = batteryLevel;

  /*Send message via ESP-NOW*/
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}


/*ESP-NOW*/
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status:");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("cmd: ");
  Serial.println(myData.cmd);
  Serial.print("batteryLow: ");
  Serial.println(myData.isBatteryLow);
  Serial.println();
}

int configESPNOW(){
  /*If richter is configured as sender*/
  if (sender){
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
  } else { 
    /*If richter is configured as receiver*/
     esp_now_register_recv_cb(OnDataRecv);
  }

  return 1;
}


