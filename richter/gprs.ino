
String sendAT(String command)
{
  String response = "";    
  mySerial.println(command); 

  delay(5);
  long int time = millis();   
  while((time+TIMEOUT_AT) > millis())
  {
    while(mySerial.available())
      response += char(mySerial.read());      
      
  }        
  return response;
}

void serialConfig()
{
  Serial.begin(BAUD_RATE);
  mySerial.begin(BAUD_RATE, SERIAL_8N1, RX_PIN, TX_PIN); 
}

bool smsConfig()
{    
  if(sendAT("AT+CMGF=1").indexOf("OK") < 0)
    return false;

  return true;
}

bool gpsConfig()
{
  if(sendAT("AT+CGNSPWR=1").indexOf("OK") >= 0) 
    if(sendAT("AT+CGPSINF=0").indexOf("OK") >= 0) 
      return true;
  
  return false;
}

bool baudConfig()
{
  if(sendAT("AT+IPR="+String(BAUD_RATE)).indexOf("OK") >= 0)
      return true;

  return false;
}

bool getGSMLocation(HardwareSerial mySerial, String *latitude, String *longitude)
{    
    int i, tempoEspera;
    String response; 
    *latitude = *longitude = "";

    mySerial.println("AT+CIPGSMLOC=1,1"); 
    delay(1000);  
    response = "";
   
    tempoEspera = 0;
    while(response.indexOf("CIPGSMLOC")<=0 && tempoEspera<5)
    {
      if(mySerial.available())
      response += char(mySerial.read());   
      tempoEspera++;
      delay(1000);
    }

    while(mySerial.available())
         response += char(mySerial.read()); 
         
    if(response.indexOf("OK")<=0 || response.indexOf("CIPGSMLOC")<=0)
        return false;

    for(i = 0; i<response.length() && response.charAt(i)!=','; i++);

    for(i++; i<response.length() && response.charAt(i)!=','; i++)  
        *longitude+=response.charAt(i);

    for(i++; i<response.length() && response.charAt(i)!=','; i++)  
        *latitude+=response.charAt(i);

    if(*latitude == "" || *longitude =="")
        return false;

    return true;
}

bool GSMLocationInit(HardwareSerial mySerial)
{  
    String response;    
    response = "";
    while(response == "" || mySerial.available())
        response += char(mySerial.read());   

    mySerial.println("AT + CGATT = 1"); 
    delay(1000);  
    response = "";
    while(response == "" || mySerial.available())
        response += char(mySerial.read());   

    if(response.indexOf("OK")<=0)
        return false;

    mySerial.println("AT + SAPBR = 3,1, \"CONTYPE\", \"GPRS\""); 
    delay(1000);  
    response = "";
    while(response == "" || mySerial.available())
        response += char(mySerial.read());   

    if(response.indexOf("OK")<=0)
        return false;

    return true;
}

void setupGPRS() 
{    
  pinMode(pinLed, OUTPUT);
  serialConfig();

  if(!baudConfig())
      ESP.restart();

  if(!smsConfig()){
      Serial.println("SMS config failed!");
      delay(3000);
      ESP.restart();
  }

  Serial.println("SMS config ok");

  
  if(!GSMLocationInit(mySerial))
  {        
      Serial.println("GPS config failed!");
      delay(3000);
      ESP.restart();
  }
  if(!gpsConfig())
  {        
      Serial.println("GPS config failed!");
      delay(3000);
      ESP.restart();
  }
  Serial.println("GPS config ok, setting modem...");
  
  if(!modemConfig(mySerial))
  {
      Serial.println("Modem init fail");
      delay(5000);
      ESP.restart();
  }
  Serial.println("Modem ok"); 
}

double getDistance(float lat, float lon)
{
  double dist = 60 * ((acos(sin(LATITUDE_REFERENCE*(PI/180)) * sin(lat*(PI/180)) + cos(LATITUDE_REFERENCE*(PI/180)) * cos(lat*(PI/180)) * cos(abs((lon-LONGITUDE_REFERENCE)) * (PI/180)))) * (180/PI));
  return dist*1852;
}

bool deviceIsTooFar(float lat, float lon, String *distance)
{
  double dist = getDistance(lat, lon);

  *distance = String(dist);

  if(dist > MAX_DISTANCE)
      return true;
        
  return false;
}

bool flag = false;
void loopGPRS() 
{
  String distance, la, lo;
  float lat, lon;  

  if(sim808.getGPS(&lat, &lon)) 
  {    
    lat = la.toFloat();
    lon = lo.toFloat();
    Serial.println("GPS signal ok. Values obtained successfully.");

    if(deviceIsTooFar(lat, lon, &distance))
    {
      if(!smsSent)
      {          
        if(sim808.sendSMS(CELLNUMBER, "Device is too far!"))
          Serial.println("Device is too far - SMS sent");        
        else
          Serial.println("Device is too far - fail to send SMS");    

        digitalWrite(pinLed, HIGH);
        smsSent = true;
        delay(5000);
      }      
    }

  }
  else  
  {
    Serial.println("GPS signal is down. Values not obtained.");
  }

  flag = !flag;
  delay(3000);

}
