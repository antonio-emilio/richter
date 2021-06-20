/*MQTT*/
#define TOPICO_SUBSCRIBE "receiverCommands"   
#define TOPICO_PUBLISH   "senderCommands"  
#define ID_MQTT  "richter"     
#define V_FIRMWARE 7  /*Firmware version of OTA updates.*/

/*Status*/
#define WIFI 1
#define WORKING 2

/*Pins*/
#define ledPin 15
#define ledPin2 2
#define PORTA_TRIGGER1   13
#define PORTA_ECHO1      14
#define PORTA_TRIGGER2   4
#define PORTA_ECHO2      5
#define PORTA_TRIGGER3   13
#define PORTA_ECHO3      14
#define PORTA_TRIGGER4   4
#define PORTA_ECHO4      5
#define pinRecharge      4
#define RST 5

/*Sensors*/
#define dhtPin 4
#define dhtType DHT22

/*Cores*/
#define CORE_0 0
#define CORE_1 1

/*States*/
#define SUCCESFULL 1
#define ERROR 0
#define STATE_0 0
#define STATE_1 1
#define STATE_2 2

/*Priorities*/
#define PRIORITY_1 1
#define PRIORITY_2 2
#define PRIORITY_3 3
#define PRIORITY_4 4
#define PRIORITY_5 5
#define PRIORITY_6 6
#define PRIORITY_7 7
#define PRIORITY_8 8
#define PRIORITY_9 9

/*Intervals*/
#define checkInterval 600000

/*SPI*/
#define SPISPEED 8000000 

/*DeepSleep*/
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  18000        /* Time ESP32 will go to sleep (in seconds) */


/*SX1278 registers*/

/*Registradores do SX1278*/
#define REG_FIFO 0x00
#define REG_FIFO_ADDR_PTR 0x0D
#define REG_FIFO_TX_BASE_AD 0x0E
#define REG_FIFO_RX_BASE_AD 0x0F
#define REG_RX_NB_BYTES 0x13
#define REG_OPMODE 0x01
#define REG_FIFO_RX_CURRENT_ADDR 0x10
#define REG_IRQ_FLAGS 0x12
#define REG_DIO_MAPPING_1 0x40
#define REG_DIO_MAPPING_2 0x41
#define REG_MODEM_CONFIG 0x1D
#define REG_MODEM_CONFIG2 0x1E
#define REG_MODEM_CONFIG3 0x26
#define REG_SYMB_TIMEOUT_LSB 0x1F
#define REG_PKT_SNR_VALUE 0x19
#define REG_PAYLOAD_LENGTH 0x22
#define REG_IRQ_FLAGS_MASK 0x11
#define REG_MAX_PAYLOAD_LENGTH 0x23
#define REG_HOP_PERIOD 0x24
#define REG_SYNC_WORD 0x39
#define REG_VERSION 0x42
#define SX72_MODE_RX_CONTINUOS 0x85
#define SX72_MODE_TX 0x83
#define SX72_MODE_SLEEP 0x80
#define SX72_MODE_STANDBY 0x81
#define PAYLOAD_LENGTH 0x40
#define REG_LNA 0x0C
#define LNA_MAX_GAIN 0x23
#define LNA_OFF_GAIN 0x00
#define LNA_LOW_GAIN 0x20
#define REG1 0x0A
#define REG2 0x84
#define SX72_MC2_FSK 0x00
#define SX72_MC2_SF7 0x70
#define SX72_MC2_SF8 0x80
#define SX72_MC2_SF9 0x90
#define SX72_MC2_SF10 0xA0
#define SX72_MC2_SF11 0xB0
#define SX72_MC2_SF12 0xC0
#define SX72_MC1_LOW_DATA_RATE_OPTIMIZE 0x01 // mandated for SF11 and SF12
#define REG_FRF_MSB 0x06
#define REG_FRF_MID 0x07
#define REG_FRF_LSB 0x08
#define FRF_MSB 0xD9 // 868.1 Mhz
#define FRF_MID 0x06
#define FRF_LSB 0x66
#define BUFLEN 2048      //Max length of buffer
#define SPISPEED 8000000 // Set to 8 * 10E6
#define PROTOCOL_VERSION 1
#define PKT_PUSH_DATA 0
#define PKT_PUSH_ACK 1
#define PKT_PULL_DATA 2
#define PKT_PULL_RESP 3
#define PKT_PULL_ACK 4
#define TX_BUFF_SIZE 2048
#define STATUS_SIZE 1024
