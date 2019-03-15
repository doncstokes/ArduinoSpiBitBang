# ArduinoSpiBitBang
Demonstration of SPI communication between two Arduinos using "Bit Banging"

This code does not use the Arduino SPI library or special hardware support, so it is much slower than examples that do. But this gives the user a better understanding of what goes on in the protocol.

## Usage
Obtain two Arduino boards. Download the master code into the first and the slave code into the second. The master code uses the standard Arduino pins for SPI. I used different pins in the slave code because I wanted to use the Arduino built in LED for output. Unfortunately, the default Arduino SPI pins use that output for the SPI clock. The pin assignments appear at the top of each source file and the user can reassign them if desired. Place jumpers between like SPI signal names. Also place a jumper between a Ground pin of each Arduino. When power is applied, the master will send a byte to the slave every four seconds containing an integer between 1 and 8, sequentially. The slave reacts by blinking the built in LED the corresponding number of blinks. If the slave detects an error (insufficient clock pulses or a number out of range), the LED displays a single long blink. The user can speeed up or slow down the communication frequency by modifying the constant "PULSE_TIME_MICRO" in the spi_master source file. I increased error detection in the slave by monitoring the SPI device select signal within clock wait loops. This reduced performance by about half. This can be removed if desired.
