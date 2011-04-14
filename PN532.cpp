#include <WProgram.h>
#include "PN532.h"

//#define PN532DEBUG 1

byte pn532ack[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00};
byte pn532response_firmwarevers[] = {0x00, 0xFF, 0x06, 0xFA, 0xD5, 0x03};

#define PN532_PACKBUFFSIZ 64
byte pn532_packetbuffer[PN532_PACKBUFFSIZ];
 
PN532::PN532(uint8_t clk, uint8_t miso, uint8_t mosi, uint8_t ss) {
  _clk = clk;
  _miso = miso;
  _mosi = mosi;
  _ss = ss;

  pinMode(_ss, OUTPUT);
  pinMode(_clk, OUTPUT);
  pinMode(_mosi, OUTPUT);
  pinMode(_miso, INPUT);
}

void PN532::begin() {
  digitalWrite(_ss, LOW);
  
  delay(1000);

  // not exactly sure why but we have to send a dummy command to get synced up
  pn532_packetbuffer[0] = PN532_FIRMWAREVERSION;
  sendCommandCheckAck(pn532_packetbuffer, 1);

  // ignore response!
}

uint32_t PN532::getFirmwareVersion(void) {
  uint32_t response;

  pn532_packetbuffer[0] = PN532_FIRMWAREVERSION;
  
  if (! sendCommandCheckAck(pn532_packetbuffer, 1))
    return 0;
  
  // read data packet
  readspidata(pn532_packetbuffer, 12);
  // check some basic stuff
  if (0 != strncmp((char *)pn532_packetbuffer, (char *)pn532response_firmwarevers, 6)) {
    return 0;
  }
  
  response = pn532_packetbuffer[6];
  response <<= 8;
  response |= pn532_packetbuffer[7];
  response <<= 8;
  response |= pn532_packetbuffer[8];
  response <<= 8;
  response |= pn532_packetbuffer[9];

  return response;
}


// default timeout of one second
boolean PN532::sendCommandCheckAck(uint8_t *cmd, uint8_t cmdlen, uint16_t timeout) {
  uint16_t timer = 0;
  
  // write the command
  spiwritecommand(cmd, cmdlen);
  
  // Wait for chip to say its ready!
  while (readspistatus() != PN532_SPI_READY) {
    if (timeout != 0) {
      timer+=10;
      if (timer > timeout)  
        return false;
    }
    delay(10);
  }
  
  // read acknowledgement
  if (!spi_readack()) {
    return false;
  }
  
  timer = 0;
  // Wait for chip to say its ready!
  while (readspistatus() != PN532_SPI_READY) {
    if (timeout != 0) {
      timer+=10;
      if (timer > timeout)  
        return false;
    }
    delay(10);
  }
  
  return true; // ack'd command
}

boolean PN532::SAMConfig(void) {
  pn532_packetbuffer[0] = PN532_SAMCONFIGURATION;
  pn532_packetbuffer[1] = 0x01; // normal mode;
  pn532_packetbuffer[2] = 0x14; // timeout 50ms * 20 = 1 second
  pn532_packetbuffer[3] = 0x01; // use IRQ pin!
  
  if (! sendCommandCheckAck(pn532_packetbuffer, 4))
     return false;

  // read data packet
  readspidata(pn532_packetbuffer, 8);
  
  return  (pn532_packetbuffer[5] == 0x15);
}


uint32_t PN532::readPassiveTargetID(uint8_t cardbaudrate) {
  uint32_t cid;
  
  pn532_packetbuffer[0] = PN532_INLISTPASSIVETARGET;
  pn532_packetbuffer[1] = 1;  // max 1 cards at once (we can set this to 2 later)
  pn532_packetbuffer[2] = cardbaudrate;
  
  if (! sendCommandCheckAck(pn532_packetbuffer, 3))
    return 0x0;  // no cards read
  
  // read data packet
  readspidata(pn532_packetbuffer, 20);
  // check some basic stuff
  
  Serial.print("Found "); Serial.print(pn532_packetbuffer[7], DEC); Serial.println(" tags");
  if (pn532_packetbuffer[7] != 1) 
    return 0;
    
  uint16_t sens_res = pn532_packetbuffer[9];
  sens_res <<= 8;
  sens_res |= pn532_packetbuffer[10];
  Serial.print("Sens Response: 0x");  Serial.println(sens_res, HEX); 
  Serial.print("Sel Response: 0x");  Serial.println(pn532_packetbuffer[11], HEX); 
  cid = 0;
  for (uint8_t i=0; i< pn532_packetbuffer[12]; i++) {
    cid <<= 8;
    cid |= pn532_packetbuffer[13+i];
    Serial.print(" 0x"); Serial.print(pn532_packetbuffer[13+i], HEX); 
  }
  Serial.println();

  return cid;
}


/************** high level SPI */


boolean PN532::spi_readack() {
  uint8_t ackbuff[6];
  
  readspidata(ackbuff, 6);
  
  return (0 == strncmp((char *)ackbuff, (char *)pn532ack, 6));
}

/************** mid level SPI */

uint8_t PN532::readspistatus(void) {
  digitalWrite(_ss, LOW);
  delay(2); 
  spiwrite(PN532_SPI_STATREAD);
  // read byte
  uint8_t x = spiread();
  
  digitalWrite(_ss, HIGH);
  return x;
}

void PN532::readspidata(uint8_t* buff, uint8_t n) {
  digitalWrite(_ss, LOW);
  delay(2); 
  spiwrite(PN532_SPI_DATAREAD);

#ifdef PN532DEBUG
  Serial.print("Reading: ");
#endif
  for (uint8_t i=0; i<n; i++) {
    delay(1);
    buff[i] = spiread();
#ifdef PN532DEBUG
    Serial.print(" 0x");
    Serial.print(buff[i], HEX);
#endif
  }

#ifdef PN532DEBUG
  Serial.println();
#endif

  digitalWrite(_ss, HIGH);
}

void PN532::spiwritecommand(uint8_t* cmd, uint8_t cmdlen) {
  uint8_t checksum;

  cmdlen++;
  
#ifdef PN532DEBUG
  Serial.print("\nSending: ");
#endif

  digitalWrite(_ss, LOW);
  delay(2);     // or whatever the delay is for waking up the board
  spiwrite(PN532_SPI_DATAWRITE);

  checksum = PN532_PREAMBLE + PN532_PREAMBLE + PN532_STARTCODE2;
  spiwrite(PN532_PREAMBLE);
  spiwrite(PN532_PREAMBLE);
  spiwrite(PN532_STARTCODE2);

  spiwrite(cmdlen);
  spiwrite(~cmdlen + 1);
 
  spiwrite(PN532_HOSTTOPN532);
  checksum += PN532_HOSTTOPN532;

#ifdef PN532DEBUG
  Serial.print(" 0x"); Serial.print(PN532_PREAMBLE, HEX);
  Serial.print(" 0x"); Serial.print(PN532_PREAMBLE, HEX);
  Serial.print(" 0x"); Serial.print(PN532_STARTCODE2, HEX);
  Serial.print(" 0x"); Serial.print(cmdlen, HEX);
  Serial.print(" 0x"); Serial.print(~cmdlen + 1, HEX);
  Serial.print(" 0x"); Serial.print(PN532_HOSTTOPN532, HEX);
#endif

  for (uint8_t i=0; i<cmdlen-1; i++) {
   spiwrite(cmd[i]);
   checksum += cmd[i];
#ifdef PN532DEBUG
   Serial.print(" 0x"); Serial.print(cmd[i], HEX);
#endif
  }
  
  spiwrite(~checksum);
  spiwrite(PN532_POSTAMBLE);
  digitalWrite(_ss, HIGH);

#ifdef PN532DEBUG
  Serial.print(" 0x"); Serial.print(~checksum, HEX);
  Serial.print(" 0x"); Serial.print(PN532_POSTAMBLE, HEX);
  Serial.println();
#endif
} 
/************** low level SPI */

void PN532::spiwrite(uint8_t c) {
  int8_t i;
  digitalWrite(_clk, HIGH);

  for (i=0; i<8; i++) {
    digitalWrite(_clk, LOW);
    if (c & _BV(i)) {
      digitalWrite(_mosi, HIGH);
    } else {
      digitalWrite(_mosi, LOW);
    }    
    digitalWrite(_clk, HIGH);
  }
}

uint8_t PN532::spiread(void) {
  int8_t i, x;
  x = 0;
  digitalWrite(_clk, HIGH);

  for (i=0; i<8; i++) {
    if (digitalRead(_miso)) {
      x |= _BV(i);
    }
    digitalWrite(_clk, LOW);
    digitalWrite(_clk, HIGH);
  }
  return x;
}
