#include <stdio.h>
#include <stdint.h>
#include "LoRaMacCrypto.c"
#include "aes.c"
#include "cmac.c"
#include "utilities.c"
#define LORAMAC_PHY_MAXPAYLOAD 255
#define UPLINK 0
#define DOWNLINK 1

/*Faz a descriptografia do payload recebido via LoRaWAN*/
String decryptPayload(uint8_t payload[], uint16_t size)
{
  size = size - 10;
  uint8_t appPayloadStartIndex = 9;
  uint8_t key[] = {0x4A, 0xD5, 0x3A, 0x3F, 0xF4, 0xC0, 0x07, 0xDC, 0xA9, 0x07, 0x76, 0xF9, 0xCB, 0x59, 0x10, 0x31}; //App Session Key
  address = (uint32_t)payload[1] + ((uint32_t)payload[2] << 8) + ((uint32_t)payload[3] << 16) + ((uint32_t)payload[4] << 24);
  printf("Device Address: 0x%x\n", address);
  uint8_t dir = UPLINK;
  uint32_t sequenceCounter = (uint32_t)payload[6] + ((uint32_t)payload[7] << 8); //FCnt from the LoRa spec
  LoRaMacPayloadDecrypt(payload + appPayloadStartIndex, size, key, address, dir, sequenceCounter, LoRaMacRxPayload);
  String resultString = String((char *)LoRaMacRxPayload);
  return resultString;

}
