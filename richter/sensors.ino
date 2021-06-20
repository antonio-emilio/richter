/********************************************************************************************ADXL345*******************************************************************************************************************************/

/*Initialize the accelerometer ADXL345*/
int initAccelerometer(){
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("There was a problem trying to connect to the accelerometer.");
    return ERROR;
  }

  /* Set the range to whatever is appropriate for your project */
  accel.setRange(ADXL345_RANGE_2_G);
  return SUCCESFULL;
}

AccMessage getAccData(){
  /*Get the new values*/
  sensors_event_t event; 
  accel.getEvent(&event);

  AccMessage myReadings;

  myReadings.xAxis = event.acceleration.x;
  myReadings.yAxis = event.acceleration.y;
  myReadings.zAxis = event.acceleration.z;

  return myReadings;
  
}
/********************************************************************************************************DHT22********************************************************************************************************************/

HumidTemp getHumidTemp(){

  HumidTemp myReadings;
  
  dht.begin();
  myReadings.temperature = dht.readTemperature();
  myReadings.humidity = dht.readHumidity(); 

  return myReadings;

}
