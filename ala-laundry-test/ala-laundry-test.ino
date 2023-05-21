unsigned long timing;
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
      Serial.print("Выполняеться скорл впередь по счету: ");
      Serial.println(i);
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
      Serial.print("Выполняеться скорл назад, по счету: ");
      Serial.println(i);
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
  Serial.println("Конец метода скролиннга: ");
}
void setWashingMode(int setWashValue){
  Serial.print("Режим который стоит: ");
  Serial.println(washMode);
  int shiftValue = washMode - setWashValue;
  washMode = setWashValue;
  Serial.print("Режим который выбран: ");
  Serial.println(washMode);
  Serial.print("На сколько делений должны перемещаться: ");
  Serial.println(shiftValue);
  encoderScroll(shiftValue);
}




void setup() { 
  Serial.begin(115200);
  pinMode(14,OUTPUT);
  pinMode(13,INPUT_PULLUP);
  pinMode(outA,OUTPUT);
  pinMode(outB,OUTPUT);
  pinMode(startStop,OUTPUT);
}

void loop() {
   but1.tick();
   if (but1.isClick()) {
     Serial.print("Текущее состоняие: ");
     digitalWrite(OUTPUT_PIN_BUTTON, HIGH);
     Serial.println("HIGH: ");
     delay(100);
     Serial.println("LOW: ");
     digitalWrite(OUTPUT_PIN_BUTTON, LOW);
     onOffMode = !onOffMode;
   }
   if (Serial.available() > 0) {
     int number = Serial.parseInt();

     if (number >= 0) {
       // Выполнение условия при получении числа 1
       Serial.println("Выполняется условие кручения энкодера");
       // Вставьте здесь код, который должен выполняться при числе 1
       
    }
  }
}
