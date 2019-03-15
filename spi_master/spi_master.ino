// File:    spi_master.ino
// Author:  Don Stokes <don@donstokes.com>
// Purpose: Demonstration of SPI communiation between two Arduinos

// GPIO Pins
#define SPI_MISO 12  // Input  Data
#define SPI_MOSI 11  // Output Data
#define SPI_CS   10  // Output Device Select
#define SPI_CLK  13  // Output Clock
// Communication Timing
#define CYCLE_TIME (1000*4)   // Milliseconds
#define PULSE_TIME_MICRO 10   // Microseconds

// Initialization called on boot
void setup() {
  // Configure GPIO Pins
  pinMode(SPI_MISO, INPUT);
  pinMode(SPI_MOSI, OUTPUT);
  pinMode(SPI_CLK,  OUTPUT);
  pinMode(SPI_CS,   OUTPUT);
  // Set initial outputs
  digitalWrite(SPI_CS,   HIGH);
  digitalWrite(SPI_CLK,  LOW);
  digitalWrite(SPI_MOSI, LOW);
  // Give slave time to initialize
  delayMicroseconds(1000);
}

// Cycles from 1 to 8
static int blinkCount = 1;
#define BLINK_MAX 8

// Called continuously by Arduino runtime
void loop() {
  // Select device
  digitalWrite(SPI_CS, LOW);
  // Copy data for shifting  
  int data = blinkCount;
  // Transmit each bit
  for (int i = 0; i < 8; i++) {
    // Transmit the MSB
    digitalWrite(SPI_MOSI, (data & 0x80) >> 7);
    // Position next bit
    data = data << 1;
    // Pause for low part of clock signal
    delayMicroseconds(PULSE_TIME_MICRO);
    // Low to High clock transition
    digitalWrite(SPI_CLK, HIGH);
    if (0) {
      // Read data goes here
    }
    // Pause for high part of clock signal
    delayMicroseconds(PULSE_TIME_MICRO);
    // High to low clock transition
    digitalWrite(SPI_CLK, LOW);
  }
  // Unselect the slave device
  digitalWrite(SPI_CS, HIGH);
  // Prepare next message
  blinkCount++;
  if (blinkCount > BLINK_MAX)
    blinkCount = 1;
  // Pause between messages
  delay(CYCLE_TIME);  
}
