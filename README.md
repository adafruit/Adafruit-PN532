# Adafruit-PN532
<!-- START COMPATIBILITY TABLE -->

## Compatibility

MCU                | Tested Works | Doesn't Work | Not Tested  | Notes
------------------ | :----------: | :----------: | :---------: | -----
Atmega328 @ 16MHz  |      X       |             |            | SPI &amp; I2C Works
Atmega328 @ 12MHz  |      X       |             |            | SPI &amp; I2C Works
Atmega32u4 @ 16MHz |      X       |             |            | Follow instructions at https://learn.adafruit.com/adafruit-pn532-rfid-nfc/shield-wiring#using-with-the-arduino-leonardo-and-yun to move pin 2.
Atmega32u4 @ 8MHz  |      X       |             |            | SPI &amp; I2C Works
ESP8266            |             |      X       |            | SPI only, I2C clock stretching not supported
Atmega2560 @ 16MHz |      X       |             |            | SPI &amp; I2C Works
ATSAM3X8E          |      X       |             |            | SPI &amp; I2C Works
ATSAM21D           |             |      X       |            | SPI only, I2C clock stretching not supported. Use programming port.
ATtiny85 @ 16MHz   |             |             |     X       | 
ATtiny85 @ 8MHz    |             |             |     X       | 
Intel Curie @ 32MHz |             |             |     X       | 
STM32F2            |             |             |     X       | 

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
