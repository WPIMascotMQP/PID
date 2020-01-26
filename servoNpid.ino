#include <Servo.h>
#include <PID_v1.h>
#include <SPI.h>

const int slaveSelectPin = 10;
const int dataReadyPin = 6;
const int chipSelectPin = 7;

Servo s1;
Servo s2;

#define Kp1 0.7    //proportional coefficient
#define Ki1 0.2    //integral coefficient
#define Kd1 0.2    //derivative coefficient
double Setpoint1, Input1, Output1, Max1, Min1;
PID myPID1(&Input1, &Output1, &Setpoint1,Kp1,Ki1,Kd1, DIRECT);



#define Kp2 0.7    //proportional coefficient
#define Ki2 0.2    //integral coefficient
#define Kd2 0.2    //derivative coefficient
double Setpoint2, Input2, Output2, Max2, Min2;
PID myPID2(&Input2, &Output2, &Setpoint2,Kp2,Ki2,Kd2, DIRECT);


int s1Ipin = 0;
int s2Ipin = 1;  

int s1Opin = 2;
int s2Opin = 3;  

void setup() {
  pinMode(slaveSelectPin, OUTPUT);
  pinMode(dataReadyPin, INPUT);
  pinMode(chipSelectPin, OUTPUT);
  // init SPI
  SPI.begin();
  
  s1.attach(9);
  s2.attach(10); 

  Setpoint1 = 0;
  Setpoint2 = 0;
  Max1 = 180;
  Min1 = 0;
  Max2 = 180;
  Min2 = 0;
  // Min max limits
  myPID1.SetOutputLimits(Min1, Max1);
  myPID2.SetOutputLimits(Min2, Max2);
  // This is speed
  myPID1.SetSampleTime(5);
  myPID2.SetSampleTime(5);
  // reset to pos 0
  Input1 = analogRead(s1Ipin);
  s1.writeMicroseconds(Setpoint1);

  Input2 = analogRead(s2Ipin);
  s2.writeMicroseconds(Setpoint2);

  myPID1.SetMode(AUTOMATIC);
  myPID2.SetMode(AUTOMATIC);
  

}

void loop() {
  // reading pos of servos
  Input1 = analogRead(s1Ipin);
  Input2 = analogRead(s2Ipin);
  
  //if (digitalRead(dataReadyPin) == HIGH) {}

  // setting setpoints(will be set by the SPI input from raspi)
  Setpoint1 = 100;
  Setpoint2 = 100;

  
  myPID1.Compute();
  myPID2.Compute();
  s1.writeMicroseconds(Output1);  
  s1.writeMicroseconds(Output2);  

}

void digitalSPIWrite(int address, int value) {
  // take the SS pin low to select the chip:
  digitalWrite(slaveSelectPin, LOW);
  delay(10);
  //  send in the address and value via SPI:
  SPI.transfer(address);
  SPI.transfer(value);
  delay(10);
  // take the SS pin high to de-select the chip:
  digitalWrite(slaveSelectPin, HIGH);
}
