#include <Wire.h>
#define _PCA8574_SlaveAddress    0x20


int valueReading = 0;

int pca8574_get_data(unsigned char &data){
  Wire.begin(_PCA8574_SlaveAddress);
  //Wire.onReceive(receiveEvent); 
  receiveEvent(1);

  return 1;
}

void receiveEvent(int howMany) {
  valueReading = Wire.read();              
}

int pca8574_put_data(unsigned char data){
    Wire.beginTransmission(_PCA8574_SlaveAddress);
  Wire.write(data);
  Wire.endTransmission();  

  return valueReading;
}
