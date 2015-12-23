# Adafruit-PN532
<!-- START COMPATIBILITY TABLE -->

## Compatibility

MCU               | Tested Works | Doesn't Work | Not Tested  | Notes
----------------- | :----------: | :----------: | :---------: | -----
Atmega328 @ 16MHz |      X       |             |            | 
Atmega328 @ 12MHz |      X       |             |            | 
Atmega32u4 @ 16MHz |      X       |             |            | Follow instructions at https://learn.adafruit.com/adafruit-pn532-rfid-nfc/shield-wiring to move pin 2.
Atmega32u4 @ 8MHz |      X       |             |            | 
ESP8266           |             |             |     X       | 
Atmega2560 @ 16MHz |      X       |             |            | 
ATSAM3X8E         |      X       |             |            | 
ATSAM21D          |             |      X       |            | /Users/james/Documents/Arduino/libraries/Adafruit_PN532/Adafruit_PN532.cpp: In function &#39;void i2c_send(uint8_t)&#39;:
/Users/james/Documents/Arduino/libraries/Adafruit_PN532/Adafruit_PN532.cpp:55:15: error: &#39;Wire1&#39; was not declared in this scope
  #define WIRE Wire1
               ^
/Users/james/Documents/Arduino/libraries/Adafruit_PN532/Adafruit_PN532.cpp:90:5: note: in expansion of macro &#39;WIRE&#39;
     WIRE.write((uint8_t)x);
     ^
/Users/james/Documents/Arduino/libraries/Adafruit_PN532/Adafruit_PN532.cpp: In function &#39;uint8_t i2c_recv()&#39;:
/Users/james/Documents/Arduino/libraries/Adafruit_PN532/Adafruit_PN532.cpp:55:15: error: &#39;Wire1&#39; was not declared in this scope
  #define WIRE Wire1
               ^
/Users/james/Documents/Arduino/libraries/Adafruit_PN532/Adafruit_PN532.cpp:104:12: note: in expansion of macro &#39;WIRE&#39;
     return WIRE.read();
            ^
/Users/james/Documents/Arduino/libraries/Adafruit_PN532/Adafruit_PN532.cpp: In member function &#39;void Adafruit_PN532::begin()&#39;:
/Users/james/Documents/Arduino/libraries/Adafruit_PN532/Adafruit_PN532.cpp:55:15: error: &#39;Wire1&#39; was not declared in this scope
  #define WIRE Wire1
               ^
/Users/james/Documents/Arduino/libraries/Adafruit_PN532/Adafruit_PN532.cpp:213:5: note: in expansion of macro &#39;WIRE&#39;
     WIRE.begin();
     ^
/Users/james/Documents/Arduino/libraries/Adafruit_PN532/Adafruit_PN532.cpp: In member function &#39;void Adafruit_PN532::readdata(uint8_t*, uint8_t)&#39;:
/Users/james/Documents/Arduino/libraries/Adafruit_PN532/Adafruit_PN532.cpp:55:15: error: &#39;Wire1&#39; was not declared in this scope
  #define WIRE Wire1
               ^
/Users/james/Documents/Arduino/libraries/Adafruit_PN532/Adafruit_PN532.cpp:1600:5: note: in expansion of macro &#39;WIRE&#39;
     WIRE.requestFrom((uint8_t)PN532_I2C_ADDRESS, (uint8_t)(n+2));
     ^
/Users/james/Documents/Arduino/libraries/Adafruit_PN532/Adafruit_PN532.cpp: In member function &#39;void Adafruit_PN532::writecommand(uint8_t*, uint8_t)&#39;:
/Users/james/Documents/Arduino/libraries/Adafruit_PN532/Adafruit_PN532.cpp:55:15: error: &#39;Wire1&#39; was not declared in this scope
  #define WIRE Wire1
               ^
/Users/james/Documents/Arduino/libraries/Adafruit_PN532/Adafruit_PN532.cpp:1701:5: note: in expansion of macro &#39;WIRE&#39;
     WIRE.beginTransmission(PN532_I2C_ADDRESS);
     ^
Error compiling.
ATtiny85 @ 16MHz  |             |             |     X       | 
ATtiny85 @ 8MHz   |             |             |     X       | 

  * ATmega328 @ 16MHz : Arduino UNO, Adafruit Pro Trinket 5V, Adafruit Metro 328, Adafruit Metro Mini
  * ATmega328 @ 12MHz : Adafruit Pro Trinket 3V
  * ATmega32u4 @ 16MHz : Arduino Leonardo, Arduino Micro, Arduino Yun, Teensy 2.0
  * ATmega32u4 @ 8MHz : Adafruit Flora, Bluefruit Micro
  * ESP8266 : Adafruit Huzzah
  * ATmega2560 @ 16MHz : Arduino Mega
  * ATSAM3X8E : Arduino Due
  * ATSAM21D : Arduino Zero, M0 Pro
  * ATtiny85 @ 16MHz : Adafruit Trinket 5V
  * ATtiny85 @ 8MHz : Adafruit Gemma, Arduino Gemma, Adafruit Trinket 3V

<!-- END COMPATIBILITY TABLE -->
