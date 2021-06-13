
/*Init the spiffs interface*/
void initSpiffs(int size){
  if(!ObjFS.init())
  {
    Serial.println("[SPIFFS] File system error");
    delay(1000);
    ESP.restart();
  }

  Serial.println("[SPIFFS] File system ok");

  if(!ObjFS.fileExists())
  {
     Serial.println("[SPIFFS] Creating new file");
     ObjFS.newFile(); 
  }  
}

int writeSpiffs(String cmd){
  ObjFS.writeFile(cmd, &errorMsg);
}

