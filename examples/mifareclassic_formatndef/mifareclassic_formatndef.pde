/**************************************************************************/
/*! 
    @file     mifareclassic_formatndef.pde
    @author   Adafruit Industries
	@license  BSD (see license.txt)

    This example attempts to format a Mifare Classic
    card for NDEF Records and writes an NDEF URI Record 
   
    Note that you need the baud rate to be 115200 because we need to print
	out the data and read from the card at the same time!

This is an example sketch for the Adafruit PN532 NFC/RFID breakout boards
This library works with the Adafruit NFC breakout 
  ----> https://www.adafruit.com/products/364
 
Check out the links above for our tutorials and wiring diagrams 
These chips use SPI to communicate, 4 required to interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!
*/
/**************************************************************************/

#include <Adafruit_PN532.h>

#define SCK  (2)
#define MOSI (3)
#define SS   (4)
#define MISO (5)

Adafruit_PN532 nfc(SCK, MISO, MOSI, SS);

const char * url = "adafruit.com";

void setup(void) {
  Serial.begin(115200);
  Serial.println("Looking for PN532...");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // configure board to read RFID tags
  nfc.SAMConfig();
  
  Serial.println("");
  Serial.println("PLEASE NOTE: Formatting your card for NDEF records will change the");
  Serial.println("authentication keys and you will no longer be able to read the");
  Serial.println("card as a normal Mifare card without resetting all keys.  Try to keep");
  Serial.println("seperate cards for NDEF and non-NDEF purposes.");
  Serial.println("");
  Serial.println("Place your Mifare Classic card on the reader to format with NDEF");
  Serial.println("and press any key to continue ...");
  // Wait for user input before proceeding
  Serial.flush();
  while (!Serial.available());
  Serial.flush();
}

void loop(void) {
  uint8_t success;                          // Flag to check if there was an error with the PN532
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  bool authenticated = false;               // Flag to indicate if the sector is authenticated

  // Use the default key
  uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    
  // Wait for an ISO14443A type card (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if (success) 
  {
    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");
    
    // Make sure this is a Mifare Classic card
    if (uidLength != 4)
    {
      Serial.println("Ooops ... this doesn't seem to be a Mifare Classic card!"); 
      return;
    }
    
    // We probably have a Mifare Classic card ... 
    Serial.println("Seems to be a Mifare Classic card (4 byte UID)");

    // Try to format the card for NDEF data
    success = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, 0, 0, keya);
    if (!success)
    {
      Serial.println("Unable to authenticate block 0 to enable card formatting!");
      return;
    }
    success = nfc.mifareclassic_FormatNDEF();
    if (!success)
    {
      Serial.println("Unable to format the card for NDEF");
      return;
    }
    
    Serial.println("Card has been formatted for NDEF data using MAD1");
    
    // Try to authenticate block 4 (first block of sector 1) using our key
    success = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, 4, 0, keya);

    // Make sure the authentification process didn't fail    
    if (!success)
    {
      Serial.println("Authentication failed.");
      return;
    }
    
    // Try to write a URL
    Serial.println("Writing URI to sector 1 as an NDEF Message");
    
    // Authenticated seems to have worked
    // Try to write an NDEF record to sector 1
    // Use 0x01 for the URI Identifier Code to prepend "http://www."
    // to the url (and save some space).  For information on URI ID Codes
    // see http://www.ladyada.net/wiki/private/articlestaging/nfc/ndef
    if (strlen(url) > 38)
    {
      // The length is also checked in the WriteNDEFURI function, but lets
      // warn users here just in case they change the value and it's bigger
      // than it should be
      Serial.println("URI is too long ... must be less than 38 characters long");
      return;
    }
    
    // URI is within size limits ... write it to the card and report success/failure
    success = nfc.mifareclassic_WriteNDEFURI(1, NDEF_URIPREFIX_HTTP_WWWDOT, url);
    if (success)
    {
      Serial.println("NDEF URI Record written to sector 1");
    }
    else
    {
      Serial.println("NDEF Record creation failed! :(");
    }
  }
  
  // Wait a bit before trying again
  Serial.println("\n\nDone!");
  Serial.flush();
  while (!Serial.available());
  Serial.flush();
}
