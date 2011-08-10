// PN532 library by adafruit/ladyada
// MIT license

// authenticateBlock, readMemoryBlock, writeMemoryBlock contributed
// by Seeed Technology Inc (www.seeedstudio.com)


#include <WProgram.h>

#define PN532_PREAMBLE 0x00
#define PN532_STARTCODE1 0x00
#define PN532_STARTCODE2 0xFF
#define PN532_POSTAMBLE 0x00

#define PN532_HOSTTOPN532 0xD4

#define PN532_FIRMWAREVERSION 0x02
#define PN532_GETGENERALSTATUS 0x04
#define PN532_SAMCONFIGURATION  0x14
#define PN532_INLISTPASSIVETARGET 0x4A
#define PN532_INDATAEXCHANGE 0x40
#define PN532_MIFARE_READ 0x30
#define PN532_MIFARE_WRITE 0xA0

#define PN532_AUTH_WITH_KEYA 0x60
#define PN532_AUTH_WITH_KEYB 0x61


#define PN532_WAKEUP 0x55

#define  PN532_SPI_STATREAD 0x02
#define  PN532_SPI_DATAWRITE 0x01
#define  PN532_SPI_DATAREAD 0x03
#define  PN532_SPI_READY 0x01

#define PN532_MIFARE_ISO14443A 0x0

#define KEY_A	1
#define KEY_B	2


class PN532{
public:
    PN532(uint8_t cs, uint8_t clk, uint8_t mosi, uint8_t miso);

    void begin(void);

    boolean SAMConfig(void);
    uint32_t getFirmwareVersion(void);
    uint32_t readPassiveTargetID(uint8_t cardbaudrate);
    uint32_t authenticateBlock(	uint8_t cardnumber /*1 or 2*/,
				uint32_t cid /*Card NUID*/,
				uint8_t blockaddress /*0 to 63*/,
				uint8_t authtype /*Either KEY_A or KEY_B */,
				uint8_t * keys);

    uint32_t readMemoryBlock(uint8_t cardnumber /*1 or 2*/,uint8_t blockaddress /*0 to 63*/, uint8_t * block);
    uint32_t writeMemoryBlock(uint8_t cardnumber /*1 or 2*/,uint8_t blockaddress /*0 to 63*/, uint8_t * block);

    boolean sendCommandCheckAck(uint8_t *cmd, uint8_t cmdlen, uint16_t timeout = 1000);

    //

private:
    uint8_t _ss, _clk, _mosi, _miso;

    boolean spi_readack();
    uint8_t readspistatus(void);
    void readspidata(uint8_t* buff, uint8_t n);
    void spiwritecommand(uint8_t* cmd, uint8_t cmdlen);
    void spiwrite(uint8_t c);
    uint8_t spiread(void);
};
