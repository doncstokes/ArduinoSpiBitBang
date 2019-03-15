// File:    spi_slave.ino
// Author:  Don Stokes <don@donstokes.com>
// Purpose: Demonstration of SPI communiation between two Arduinos

// GPIO Pins
#define SPI_MISO 2  // Data Out
#define SPI_MOSI 3  // Data In
#define SPI_CS   4  // Device Select
#define SPI_CLK  5  // Clock
#define BLINKER 13  // Arduino internal LED

// Pulse times for blinking LED
#define BLINK_TIME 200
#define BLINK_ERROR_TIME (BLINK_TIME * 4)

#define IS_SELECTED(value) (value == LOW)

// Initialization called on boot
void setup() {
  // Configure GPIO Pins
  pinMode(BLINKER,  OUTPUT);
  pinMode(SPI_MISO, OUTPUT);
  pinMode(SPI_MOSI, INPUT);
  pinMode(SPI_CS,   INPUT);
  pinMode(SPI_CLK,  INPUT);

  // Initialize Output Values
  digitalWrite(BLINKER, LOW);
  digitalWrite(MISO,    LOW);
  
  // Give master time to initialize
  delay(750);
}

// Blink the LED by specified count
void blink(int count) {
  while (count--) {
    digitalWrite(BLINKER, HIGH);
    delay(BLINK_TIME);
    digitalWrite(BLINKER, LOW);
    delay(BLINK_TIME);
  }
}

// One long blink of LED to signal error during receive
void blinkError() {
  digitalWrite(BLINKER, HIGH);
  delay(BLINK_ERROR_TIME);
  digitalWrite(BLINKER, LOW);
  delay(BLINK_TIME);
}

// Called continuously by Arduino runtime
// Wait for SPI message from master.
// Message is number between 1 and 8.
// Blink LED to display message.
void loop() {
  // Values read from GPIO inputs
  int valSelect = 0;
  int valClock  = 0;
  // Count of rising edges detected
  int bitCount = 0;
  // Data Byte Received
  int rcvData = 0;

  // Wait for beginning of cycle
  do {
    valSelect = digitalRead(SPI_CS);
  } while (IS_SELECTED(valSelect));

  // Wait for device select
  do {
    valSelect = digitalRead(SPI_CS);
  } while (!IS_SELECTED(valSelect));

  // Receive each of 8 bits
  for (int i = 0; i < 8 && IS_SELECTED(valSelect); i++) {
    
    if (false) {
      // Transmit would go here.
    }

    // Wait for clock rising edge
    do {
      valClock = digitalRead(SPI_CLK);
      valSelect = digitalRead(SPI_CS);
    } while (valClock != HIGH && IS_SELECTED(valSelect));

    if (IS_SELECTED(valSelect)) {
      bitCount++;
      // Shift existing bits left
      rcvData = rcvData << 1;
      // Read next data bit
      int valMosi = digitalRead(SPI_MOSI);
      // Combine with previous bits
      if (valMosi == HIGH)
        rcvData |=  1;
      // Wait for falling edge
      do {
        valClock = digitalRead(SPI_CLK);
        valSelect = digitalRead(SPI_CS);
      }  while (valClock != LOW && IS_SELECTED(valSelect));
    }
  }
  
  // Display data on LED
  if (bitCount == 8 && rcvData > 0 && rcvData <= 8)
    blink(rcvData);
  else
    blinkError();
}

