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

