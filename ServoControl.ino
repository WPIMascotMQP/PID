#include "esp_timer.h"

bool dir1;
bool dir2;
float currentstep1 = 0;
float currentstep2 = 0;

void setup() {
  currentstep1 = 0;
  currentstep2 = 0;
  Serial.begin(115200);
 

  

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
    currentMicro = micros();
  }
     
  void Update()
  {
    //currentMicro = micros();
    int64_t currentMicro = esp_timer_get_time();

    if((startState == HIGH) && (currentMicro - previousMicro >= highTime))
    {
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
Pulser Pulser1(16, 17, 1000, 5);
//Pulser Pulser2(5, 6, 100, 100);


void motorMove(int mot1, int mot2, int mot3, int mot4){ 
  while(!(currentstep1==mot1)&&!(currentstep2==mot1)){
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
    if(currentstep1 != mot1){
        Pulser1.Update();
//        Pulser1.Update();
        
//        if(dir1 == true){
//          currentstep1 = currentstep1 + 1;
//          //Serial.println("+ve");
//          }
//        if(dir1 == false){
//          currentstep1 = currentstep1 - 1;
//          //Serial.println("-ve");
//
//          }
        //Serial.println(currentstep1);

  
    }
    /*
    if(currentstep2 != mot2){
        Pulser2.Update();
        Pulser2.Update();
        
        
        if(dir2 == true){
          currentstep2 = currentstep2 - 1;}
        if(dir2 == false){
          currentstep2 = currentstep2 + 1;}
        
    }
    */
  }
    
}

void loop() {
  motorMove(-12600,1000,0,0);
}
