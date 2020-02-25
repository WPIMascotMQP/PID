#include "esp_timer.h"

bool dir1;
bool dir2;
float currentstep1 = 0;
float currentstep2 = 0;

void setup() {
  currentstep1 = 0;
  currentstep2 = 0;
  //Serial.begin(115200);
  Serial.begin(2000000);

}

class Pulser
{
  long highTime; // micoseconds of HIGH
  long lowTime; // microseconds of LOW

  int serPin;    
  int startState;
  public:  
  int dirPin;  

  public:
  unsigned long previousMicro; // last time servo updated
  unsigned long currentMicro;

  public:
  Pulser(int spin, int dPin, long high, long low)
  {
    serPin = spin;
    pinMode(serPin, OUTPUT);
    dirPin = dPin;     
    pinMode(dirPin, OUTPUT);
          
    highTime = high;
    lowTime = low;
        
    startState = LOW; 
    previousMicro = 0;
    currentMicro = 0;
  }
     
  void Update()
  {
    //currentMicro = micros();
    int64_t currentMicro = esp_timer_get_time();

    if((startState == HIGH) && (currentMicro - previousMicro >= highTime))
    {
      //Serial.print((unsigned long)currentMicro);
      //Serial.print("-");
      //Serial.print((unsigned long)previousMicro);
      //Serial.print("=");
      //Serial.println((unsigned long)currentMicro - previousMicro);
      previousMicro = currentMicro;  
      digitalWrite(serPin, startState);
      startState = LOW;

      if(dir1 == true){
        currentstep1 = currentstep1 + 0.5;
        }
      if(dir1 == false){
        currentstep1 = currentstep1 - 0.5;
        }

      
    }
    else if ((startState == LOW) && (currentMicro - previousMicro >= lowTime))
    {
      //Serial.print((unsigned long)currentMicro);
      //Serial.print("-");
      //Serial.print((unsigned long)previousMicro);
      //Serial.print("=");
      //Serial.println((unsigned long)currentMicro - previousMicro);
      previousMicro = currentMicro;
      digitalWrite(serPin, startState);
      startState = HIGH;
      if(dir1 == true){
        currentstep1 = currentstep1 + 0.5;
        }
      if(dir1 == false){
        currentstep1 = currentstep1 - 0.5;
        }
    }
   

  }
};


//Setting pins and speeds
Pulser Pulser1(0, 4, 100, 50);
//4294840669
//Pulser Pulser2(5, 6, 100, 100);


void motorMove(int mot1, int mot2, int mot3, int mot4){ 
    if(currentstep1<mot1){        
        digitalWrite(Pulser1.dirPin, HIGH);
        dir1 = true;
    }
    else if(currentstep1>mot1){
        digitalWrite(Pulser1.dirPin, LOW);
        dir1 = false;
    }
     /* 
    if(currentstep2<mot2){
        digitalWrite(Pulser2.dirPin, HIGH);
        dir2 = true;
    }
    else if(currentstep2>mot2){
        digitalWrite(Pulser2.dirPin, LOW);
        dir2 = false;
    }
    */
    unsigned long currentMicro2 = esp_timer_get_time();
    unsigned long previousMicro2 = esp_timer_get_time();

    currentMicro2 = micros();
    while(!(currentstep1==mot1)&&!(currentstep2==mot1)){
      //currentMicro = esp_timer_get_time();
      Pulser1.Update();
      //previousMicro = esp_timer_get_time();
      //Serial.println((unsigned long)currentMicro - previousMicro);
      Serial.println(currentstep1);   
    }
    previousMicro2 = micros();
    //Serial.println(previousMicro2-currentMicro2);
}

void loop() {
    motorMove(1,1000,0,0);
    motorMove(12800,1000,0,0);


}
