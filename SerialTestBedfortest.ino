

#include <HardwareSerial.h>

HardwareSerial MySerial(1);

void setup() {
MySerial.begin(9600, SERIAL_8N1, 16, 17);
  pinMode(2, OUTPUT);
}

void loop() {
while (MySerial.available() > 0) {
  uint8_t byteFromSerial = 0;
  Serial1.write(byteFromSerial);
  
}
}
/*
#include <HardwareSerial.h>

HardwareSerial MySerial(1);

void setup() {
MySerial.begin(9600, SERIAL_8N1, 17, 16);
  pinMode(2, OUTPUT);
}

void loop() {
while (MySerial.available() > 0) {
  uint8_t byteFromSerial = Serial.read();
  if(byteFromSerial == 100){
    while(1){
    digitalWrite(2, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(1000);  // wait for a second
    digitalWrite(2, LOW);  // turn the LED off by making the voltage LOW
    delay(1000);  // wait for a second
    }
   }
}
}


#include "HardwareSerial.h"
void setup() {
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial.begin(115200);
  //Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial2.begin(115200, SERIAL_8N1, 17, 16);
  Serial.println("Serial Txd is on pin: "+String(16));
  Serial.println("Serial Rxd is on pin: "+String(17));
}
void loop() { //Choose Serial1 or Serial2 as required
  while (Serial2.available()) {
    Serial.print(char(Serial2.read()));
  }
}
*/
