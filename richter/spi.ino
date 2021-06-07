
/*Define the SPI settings for reading messages*/
SPISettings readSettings(SPISPEED, MSBFIRST, SPI_MODE0);

uint8_t readRegister(uint8_t addr)
{

  SPI.beginTransaction(readSettings);
  SPI.transfer(addr & 0x7F);
  uint8_t res = (uint8_t)SPI.transfer(0x00);
  SPI.endTransaction();
  return ((uint8_t)res);
}

/*define the settings for SPI writing*/
SPISettings writeSettings(SPISPEED, MSBFIRST, SPI_MODE0);

void writeRegister(uint8_t addr, uint8_t value)
{
  SPI.beginTransaction(writeSettings);
  SPI.transfer((addr | 0x80) & 0xFF);
  SPI.transfer(value & 0xFF);
  SPI.endTransaction();
}

