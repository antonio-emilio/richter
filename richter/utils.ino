
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
  /*Iniciando o watchdog*/

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

/*Função para obter horario atual*/
void getCurrentTime(){
  timeoutHorario = 60;
  Serial.println("Obtendo horário atual.");
  timeClient.begin(); //Inicia cliente para obter horário.
  timeClient.setTimeOffset(-10800); //Fuso horário.
  while (!timeClient.update()) {
    timeClient.forceUpdate(); //Atualiza o horário.
    delay(100);
    timeoutHorario--;
    if (timeoutHorario == 0) {
      break;
    }
  }

  formattedDate = "Horário de detecção: ";
  formattedDate += timeClient.getFormattedDate(); //Obtem o horário formatado.
  if (timeoutHorario == 0) {
    Serial.println("Não foi possível obter o horário.");
    formattedDate = "Não foi possível obter o horário.";
  } else {
    Serial.println("Horário obtido com sucesso.");
  }
}
