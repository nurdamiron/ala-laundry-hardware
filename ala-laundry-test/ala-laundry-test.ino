}unsigned long timing;
boolean onOffMode = false;
#include "GyverButton.h"

#define INPUT_PIN_BUTTON 13
#define OUTPUT_PIN_BUTTON 14
GButton but1(INPUT_PIN_BUTTON);
int outB = 25;
int outA = 26;
int startStop = 27;

int washMode = 7;

//void startStop(){
//  startStopStatus = !startStopStatus;
//  digitalWrite(outStartStop,HIGH);
//  delay(150);
//  digitalWrite(outStartStop,LOW);
//  Serial.println("StartStop");
//  delay(100);
//}

void encoderScroll(int scrollValue){
  if(scrollValue > 0){
    for(int i = 0 ; i < scrollValue+1; i++){
      Serial.print("if: ");Serial.println(i);
      digitalWrite(outA, HIGH);
      delay(30);
      digitalWrite(outB, LOW);
      delay(30);
      digitalWrite(outA, LOW);
      delay(30);
      digitalWrite(outB, HIGH);
      delay(30);
    }
    digitalWrite(outB, LOW);
  }
  else if(scrollValue < 0){
    for(int i = 0 ; i < abs(scrollValue)+1; i++){
        Serial.print("elseif: ");Serial.println(i);
      digitalWrite(outB, HIGH);
      delay(30);
      digitalWrite(outA, LOW);
      delay(30);
      digitalWrite(outB, LOW);
      delay(30);
      digitalWrite(outA, HIGH);
      delay(30);
    }
    digitalWrite(outA, LOW);
  }
  Serial.println("encoderScroll");
}
void setWashingMode(int setWashValue){
  Serial.print("wash mode  ");
  Serial.println(washMode);
  int shiftValue = washMode - setWashValue;
  washMode = setWashValue;
  encoderScroll(shiftValue);
  Serial.print("wash mode  ");
  Serial.println(washMode);
  Serial.print("shift value:  ");
  Serial.println(shiftValue);
}




void setup() { 
  Serial.begin(115200);
  // put your setup code here, to run once:
  pinMode(14,OUTPUT);
  pinMode(13,INPUT_PULLUP);
  pinMode(outA,OUTPUT);
  pinMode(outB,OUTPUT);
  pinMode(startStop,OUTPUT);
}

void loop() {
   but1.tick();
   if (but1.isClick()) {
   Serial.print("onOffMode: ");
   Serial.println(onOffMode);
   digitalWrite(OUTPUT_PIN_BUTTON, HIGH);
   Serial.println("HIGH: ");
   delay(100);
   Serial.println("LOW: ");
   digitalWrite(OUTPUT_PIN_BUTTON, LOW);
   onOffMode = !onOffMode;
   }
}
