

#define CFG_eu868 1
#define CFG_sx1276_radio 1
#define LMIC_DEBUG_LEVEL 2 //VERIFICAR DEF JA DEFINIDA ANTES EM CONFIG .H
#define LMIC_FAILURE_TO Serial
#define DISABLE_PING
#define DISABLE_BEACONS

// LoRaWAN NwkSKey, network session key
// This is the default Semtech key, which is used by the early prototype TTN
// network.
static const PROGMEM u1_t NWKSKEY[16] = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C };

// LoRaWAN AppSKey, application session key
// This is the default Semtech key, which is used by the early prototype TTN
// network.
static const u1_t PROGMEM APPSKEY[16] = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C };

// LoRaWAN end-device address (DevAddr)
static const u4_t DEVADDR = 0x03FF0001 ; // <-- Change this address for every node!

// These callbacks are only used in over-the-air activation, so they are
// left empty here (we cannot leave them out completely unless
// DISABLE_JOIN is set in config.h, otherwise the linker will complain).
void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }

static uint8_t mydata[] = "Hello, world!";
static osjob_t sendjob;

// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
const unsigned TX_INTERVAL = 60;

// Pin mapping
const lmic_pinmap lmic_pins = {
    .nss = 6,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 5,
    .dio = {2, 3, 4},
};

void onEvent (ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            Serial.println(F("EV_SCAN_TIMEOUT"));
            break;
        case EV_BEACON_FOUND:
            Serial.println(F("EV_BEACON_FOUND"));
            break;
        case EV_BEACON_MISSED:
            Serial.println(F("EV_BEACON_MISSED"));
            break;
        case EV_BEACON_TRACKED:
            Serial.println(F("EV_BEACON_TRACKED"));
            break;
        case EV_JOINING:
            Serial.println(F("EV_JOINING"));
            break;
        case EV_JOINED:
            Serial.println(F("EV_JOINED"));
            break;
        case EV_RFU1:
            Serial.println(F("EV_RFU1"));
            break;
        case EV_JOIN_FAILED:
            Serial.println(F("EV_JOIN_FAILED"));
            break;
        case EV_REJOIN_FAILED:
            Serial.println(F("EV_REJOIN_FAILED"));
            break;
        case EV_TXCOMPLETE:
            Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
            if (LMIC.txrxFlags & TXRX_ACK)
              Serial.println(F("Received ack"));
            if (LMIC.dataLen) {
              Serial.println(F("Received "));
              Serial.println(LMIC.dataLen);
              Serial.println(F(" bytes of payload"));
            }
            // Schedule next transmission
            os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
            break;
        case EV_LOST_TSYNC:
            Serial.println(F("EV_LOST_TSYNC"));
            break;
        case EV_RESET:
            Serial.println(F("EV_RESET"));
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            Serial.println(F("EV_RXCOMPLETE"));
            break;
        case EV_LINK_DEAD:
            Serial.println(F("EV_LINK_DEAD"));
            break;
        case EV_LINK_ALIVE:
            Serial.println(F("EV_LINK_ALIVE"));
            break;
         default:
            Serial.println(F("Unknown event"));
            break;
    }
}

void do_send(osjob_t* j){
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    } else {
        // Prepare upstream data transmission at the next possible time.
        LMIC_setTxData2(1, mydata, sizeof(mydata)-1, 0);
        Serial.println(F("Packet queued"));
    }
    // Next TX is scheduled after TX_COMPLETE event.
}

void dosetup() {
    Serial.begin(115200);
    Serial.println(F("Starting"));

    #ifdef VCC_ENABLE
    // For Pinoccio Scout boards
    pinMode(VCC_ENABLE, OUTPUT);
    digitalWrite(VCC_ENABLE, HIGH);
    delay(1000);
    #endif

    // LMIC init
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();

    // Set static session parameters. Instead of dynamically establishing a session
    // by joining the network, precomputed session parameters are be provided.
    #ifdef PROGMEM
    // On AVR, these values are stored in flash and only copied to RAM
    // once. Copy them to a temporary buffer here, LMIC_setSession will
    // copy them into a buffer of its own again.
    uint8_t appskey[sizeof(APPSKEY)];
    uint8_t nwkskey[sizeof(NWKSKEY)];
    memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
    memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
    LMIC_setSession (0x1, DEVADDR, nwkskey, appskey);
    #else
    // If not running an AVR with PROGMEM, just use the arrays directly
    LMIC_setSession (0x1, DEVADDR, NWKSKEY, APPSKEY);
    #endif

    #if defined(CFG_eu868)
    // Set up the channels used by the Things Network, which corresponds
    // to the defaults of most gateways. Without this, only three base
    // channels from the LoRaWAN specification are used, which certainly
    // works, so it is good for debugging, but can overload those
    // frequencies, so be sure to configure the full frequency range of
    // your network here (unless your network autoconfigures them).
    // Setting up channels should happen after LMIC_setSession, as that
    // configures the minimal channel set.
    // NA-US channels 0-71 are configured automatically
    LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(1, 868300000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI);      // g-band
    LMIC_setupChannel(2, 868500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(3, 867100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(4, 867300000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(5, 867500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(6, 867700000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(7, 867900000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(8, 868800000, DR_RANGE_MAP(DR_FSK,  DR_FSK),  BAND_MILLI);      // g2-band
    // TTN defines an additional channel at 869.525Mhz using SF9 for class B
    // devices' ping slots. LMIC does not have an easy way to define set this
    // frequency and support for class B is spotty and untested, so this
    // frequency is not configured here.
    #elif defined(CFG_us915)
    // NA-US channels 0-71 are configured automatically
    // but only one group of 8 should (a subband) should be active
    // TTN recommends the second sub band, 1 in a zero based count.
    // https://github.com/TheThingsNetwork/gateway-conf/blob/master/US-global_conf.json
    LMIC_selectSubBand(1);
    #endif

    // Disable link check validation
    LMIC_setLinkCheckMode(0);

    // TTN uses SF9 for its RX2 window.
    LMIC.dn2Dr = DR_SF9;

    // Set data rate and transmit power for uplink (note: txpow seems to be ignored by the library)
    LMIC_setDrTxpow(SF12,14);

    // Start job
    do_send(&sendjob);
}

void doloop() {
    os_runloop_once();
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Base64.h>

boolean receivePkt(char *payload)
{

  // clear rxDone
  writeRegister(REG_IRQ_FLAGS, 0x40);
  int irqflags = readRegister(REG_IRQ_FLAGS);
  cp_nb_rx_rcv++;
  //  payload crc: 0x20
  if ((irqflags & 0x20) == 0x20)
  {
    printf("CRC error\n");
    writeRegister(REG_IRQ_FLAGS, 0x20);
    return false;
  }
  else
  {

    cp_nb_rx_ok++;

    byte currentAddr = readRegister(REG_FIFO_RX_CURRENT_ADDR);
    byte receivedCount = readRegister(REG_RX_NB_BYTES);
    receivedbytes = receivedCount;

    writeRegister(REG_FIFO_ADDR_PTR, currentAddr);

    for (int i = 0; i < receivedCount; i++)
    {
      payload[i] = (char)readRegister(REG_FIFO);
    }
  }
  return true;
}

void SetupLoRa(uint32_t frequencia)
{
  freq2 = frequencia;
  byte version = readRegister(REG_VERSION);
  if (version == 0x22)
  {
    // sx1272
    printf("SX1272 detected, starting.\n");
    sx1272 = true;
  }
  else
  {
    // sx1276?
    digitalWrite(RST, LOW);
    delay(100);
    digitalWrite(RST, HIGH);
    delay(100);
    version = readRegister(REG_VERSION);
    if (version == 0x12)
    {
      // sx1276
      sx1272 = false;
    }
    else
    {
      Serial.println("Module has not been found");
    }
  }

  writeRegister(REG_OPMODE, SX72_MODE_SLEEP);

  // set frequency
  uint64_t frf = ((uint64_t)freq2 << 19) / 32000000;
  writeRegister(REG_FRF_MSB, (uint8_t)(frf >> 16));
  writeRegister(REG_FRF_MID, (uint8_t)(frf >> 8));
  writeRegister(REG_FRF_LSB, (uint8_t)(frf >> 0));

  writeRegister(REG_SYNC_WORD, 0x34); // LoRaWAN public sync word

  if (sx1272)
  {
    if (sf == SF11 || sf == SF12)
    {
      writeRegister(REG_MODEM_CONFIG, 0x0B);
    }
    else
    {
      writeRegister(REG_MODEM_CONFIG, 0x0A);
    }
    writeRegister(REG_MODEM_CONFIG2, (sf << 4) | 0x04);
  }
  else
  {
    if (sf == SF11 || sf == SF12)
    {
      writeRegister(REG_MODEM_CONFIG3, 0x0C);
    }
    else
    {
      writeRegister(REG_MODEM_CONFIG3, 0x04);
    }
    writeRegister(REG_MODEM_CONFIG, 0x72);
    writeRegister(REG_MODEM_CONFIG2, (sf << 4) | 0x04);
  }

  if (sf == SF10 || sf == SF11 || sf == SF12)
  {
    writeRegister(REG_SYMB_TIMEOUT_LSB, 0x05);
  }
  else
  {
    writeRegister(REG_SYMB_TIMEOUT_LSB, 0x08);
  }
  writeRegister(REG_MAX_PAYLOAD_LENGTH, 0x80);
  writeRegister(REG_PAYLOAD_LENGTH, PAYLOAD_LENGTH);
  writeRegister(REG_HOP_PERIOD, 0xFF);
  writeRegister(REG_FIFO_ADDR_PTR, readRegister(REG_FIFO_RX_BASE_AD));

  // Set Continous Receive Mode
  writeRegister(REG_LNA, LNA_MAX_GAIN); // max lna gain
  writeRegister(REG_OPMODE, SX72_MODE_RX_CONTINUOS);
}

void receivepacket()
{

  long int SNR;
  int rssicorr;
  if (receivePkt(message))
  {
    byte value = readRegister(REG_PKT_SNR_VALUE);
    if (value & 0x80) // The SNR sign bit is 1
    {
      // Invert and divide by 4
      value = ((~value + 1) & 0xFF) >> 2;
      SNR = -value;
    }
    else
    {
      // Divide by 4
      SNR = (value & 0xFF) >> 2;
    }

    if (sx1272)
    {
      rssicorr = 139;
    }
    else
    {
      rssicorr = 157;
    }

    int j;
    j = bin_to_b64((uint8_t *)message, receivedbytes, (char *)(b64), 341);
    //fwrite(b64, sizeof(char), j, stdout);

    char buff_up[TX_BUFF_SIZE]; /* buffer to compose the upstream packet */
    char buff_up_msg[TX_BUFF_SIZE];
    int buff_index = 0;

    /* gateway <-> MAC protocol variables */
    //static uint32_t net_mac_h; /* Most Significant Nibble, network order */
    //static uint32_t net_mac_l; /* Least Significant Nibble, network order */

    /* pre-fill the data buffer with fixed fields */
    buff_up[0] = PROTOCOL_VERSION;
    buff_up[3] = PKT_PUSH_DATA;

    /* process some of the configuration variables */
    //net_mac_h = htonl((uint32_t)(0xFFFFFFFF & (lgwm>>32)));
    //net_mac_l = htonl((uint32_t)(0xFFFFFFFF &  lgwm  ));
    //*(uint32_t *)(buff_up + 4) = net_mac_h;
    //*(uint32_t *)(buff_up + 8) = net_mac_l;

    buff_up[4] = (unsigned char)0x00;
    buff_up[5] = (unsigned char)0x00;
    buff_up[6] = (unsigned char)0x00;
    buff_up[7] = 0xFF;
    buff_up[8] = 0xFF;
    buff_up[9] = (unsigned char)0x00;
    buff_up[10] = (unsigned char)0x00;
    buff_up[11] = (unsigned char)0x00;

    /* start composing datagram with the header */
    uint8_t token_h = (uint8_t)rand(); /* random token */
    uint8_t token_l = (uint8_t)rand(); /* random token */
    buff_up[1] = token_h;
    buff_up[2] = token_l;
    buff_index = 12; /* 12-byte header */

    // TODO: tmst can jump is time is (re)set, not good.
    struct timeval now;
    gettimeofday(&now, NULL);
    uint32_t tmst = (uint32_t)(now.tv_sec * 1000000 + now.tv_usec);

    /* start of JSON structure */
    memcpy((void *)(buff_up + buff_index), (void *)"{\"rxpk\":[", 9);
    buff_index += 9;
    buff_up[buff_index] = '{';
    ++buff_index;
    j = snprintf((char *)(buff_up + buff_index), TX_BUFF_SIZE - buff_index, "\"tmst\":%u", tmst);
    buff_index += j;
    j = snprintf((char *)(buff_up + buff_index), TX_BUFF_SIZE - buff_index, ",\"chan\":%1u,\"rfch\":%1u,\"freq2\":%.6lf", 0, 0, (double)freq2 / 1000000);
    buff_index += j;
    memcpy((void *)(buff_up + buff_index), (void *)",\"stat\":1", 9);
    buff_index += 9;
    memcpy((void *)(buff_up + buff_index), (void *)",\"modu\":\"LORA\"", 14);
    buff_index += 14;
    /* Lora datarate & bandwidth, 16-19 useful chars */
    switch (sf)
    {
    case SF7:
      memcpy((void *)(buff_up + buff_index), (void *)",\"datr\":\"SF7", 12);
      buff_index += 12;
      break;
    case SF8:
      memcpy((void *)(buff_up + buff_index), (void *)",\"datr\":\"SF8", 12);
      buff_index += 12;
      break;
    case SF9:
      memcpy((void *)(buff_up + buff_index), (void *)",\"datr\":\"SF9", 12);
      buff_index += 12;
      break;
    case SF10:
      memcpy((void *)(buff_up + buff_index), (void *)",\"datr\":\"SF10", 13);
      buff_index += 13;
      break;
    case SF11:
      memcpy((void *)(buff_up + buff_index), (void *)",\"datr\":\"SF11", 13);
      buff_index += 13;
      break;
    case SF12:
      memcpy((void *)(buff_up + buff_index), (void *)",\"datr\":\"SF12", 13);
      buff_index += 13;
      break;
    default:
      memcpy((void *)(buff_up + buff_index), (void *)",\"datr\":\"SF?", 12);
      buff_index += 12;
    }
    memcpy((void *)(buff_up + buff_index), (void *)"BW125\"", 6);
    buff_index += 6;
    memcpy((void *)(buff_up + buff_index), (void *)",\"codr\":\"4/5\"", 13);
    buff_index += 13;
    j = snprintf((char *)(buff_up + buff_index), TX_BUFF_SIZE - buff_index, ",\"lsnr\":%li", SNR);
    buff_index += j;
    j = snprintf((char *)(buff_up + buff_index), TX_BUFF_SIZE - buff_index, ",\"rssi\":%d,\"size\":%u", readRegister(0x1A) - rssicorr, receivedbytes);
    buff_index += j;
    memcpy((void *)(buff_up + buff_index), (void *)",\"data\":\"", 9);
    buff_index += 9;
    j = bin_to_b64((uint8_t *)message, receivedbytes, (char *)(buff_up + buff_index), 341);

    /*Descriptografando o pacote....*/
    j = bin_to_b64((uint8_t *)message, receivedbytes, (char *)(buff_up_msg + buff_index), 341); //BASE64
    int input2Len = sizeof((char *)(buff_up_msg + buff_index));
    int decodedLen = base64_dec_len((buff_up_msg + buff_index), receivedbytes);
    char decoded[decodedLen];
    base64_decode(decoded, (char *)(buff_up_msg + buff_index), receivedbytes);
    uint8_t payload[receivedbytes];
    String auxString = "";
    for (int i = 0; i < receivedbytes; i++)
    {
      payload[i] = decoded[i];
    }
    String payloadDecrypt = decryptPayload(payload, receivedbytes);
    /*Descriptografia finalizada.*/

    rssiAtual = readRegister(0x1B) - rssicorr;
    printf("Packet RSSI: %d, ", readRegister(0x1A) - rssicorr);
    printf("RSSI: %d, ", rssiAtual);
    printf("SNR: %li, ", SNR);
    printf("Length: %i", (int)receivedbytes);
    printf("\n");

    buff_index += j;
    buff_up[buff_index] = '"';
    ++buff_index;

    /* End of packet serialization */
    buff_up[buff_index] = '}';
    ++buff_index;
    buff_up[buff_index] = ']';
    ++buff_index;
    /* end of JSON datagram payload */
    buff_up[buff_index] = '}';
    ++buff_index;
    buff_up[buff_index] = 0; /* add string terminator, for safety */

    printf("rxpk update: %s\n", (char *)(buff_up + 12)); /* DEBUG: display JSON payload */

    pkt1 = payloadDecrypt;

    fflush(stdout);

  } // received a message
}
