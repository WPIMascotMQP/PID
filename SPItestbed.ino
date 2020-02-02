#include <SPI.h>


#define HSPI_MISO   12
#define HSPI_MOSI   13
#define HSPI_SCLK   15
#define HSPI_SS     14


static const int spiClk = 1000000; // 1 MHz

//uninitalised pointers to SPI objects
SPIClass * hspi = NULL;

void setup() {
  //initialise two instances of the SPIClass attached to VSPI and HSPI respectively
  hspi = new SPIClass(HSPI);
  

  //alternatively route through GPIO pins
  hspi->begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_SS); //SCLK, MISO, MOSI, SS


  //set up slave select pins as outputs as the Arduino API
  //doesn't handle automatically pulling SS low
  pinMode(HSPI_SS, OUTPUT); //HSPI SS

}

// the loop function runs over and over again until power down or reset
void loop() {
  //use the SPI buses
  hspiCommand();
  delay(100);
}

void hspiCommand() {
  byte stuffO = 0b11001100;
  byte stuffI;
    
  hspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(HSPI_SS, LOW);
  hspi->transfer(stuff);
  digitalWrite(HSPI_SS, HIGH);
  hspi->endTransaction();
}
