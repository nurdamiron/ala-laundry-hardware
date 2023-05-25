unsigned long timing;

#include "GyverButton.h"

#define INPUT_PIN_BUTTON 13//для ускорения в Алекс гайвер сайтндай айтылган
#define OUTPUT_PIN_BUTTON 14
const int outB = 25;//либо const дегенды колдануга болады
const int outA = 26;
const int startStop = 27;
GButton but1(INPUT_PIN_BUTTON);
const int timeEncoder = 100;
int washMode = 7;//стандартный режим при включения стиралки
boolean onOffMode = false;//режим стиралки по умолчанию точнее выключена

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
    for(int i = 0 ; i < scrollValue; i++){
      Serial.print("Выполняеться скорл впередь по счету: ");
      Serial.println(i);
      digitalWrite(outA, HIGH);
      delay(timeEncoder);
      digitalWrite(outB, HIGH);
      delay(timeEncoder);
      digitalWrite(outA, LOW);
      delay(timeEncoder);
      digitalWrite(outB, LOW);
      delay(timeEncoder);
    }
//    digitalWrite(outB, LOW);
  }
  else if(scrollValue < 0){
    for(int i = 0 ; i < abs(scrollValue); i++){
      Serial.print("Выполняеться скорл назад, по счету: ");
      Serial.println(i);
      digitalWrite(outB, HIGH);
      delay(timeEncoder);
      digitalWrite(outA, HIGH);
      delay(timeEncoder);
      digitalWrite(outB, LOW);
      delay(timeEncoder);
      digitalWrite(outA, LOW);
      delay(timeEncoder);
    }
//    digitalWrite(outA, LOW);
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
  if (but1.isPress()) {
    Serial.print("Текущее состоняие: ");
    if(onOffMode == 0){
      Serial.println("отключен");
    }else{
      Serial.println("включен");
    }
    digitalWrite(OUTPUT_PIN_BUTTON, HIGH);
    delay(80);
    digitalWrite(OUTPUT_PIN_BUTTON, LOW);
    onOffMode = !onOffMode;
  }
  if (Serial.available() > 0) {
    int number = Serial.parseInt();
    int number1 = Serial.parseInt();
    if (number >= 0) {
      Serial.println("Выполняется условие кручения энкодера");
      Serial.print("Значения аргумента ");
      Serial.println(number);
      setWashingMode(number);
    }
    else if(number <= 0){
      for(int i = 0; i < 120; i++){
        encoderScroll(1);
//        delay(500);
      }
    }
  }
//  setWashingMode(1);
//  delay(1000);
//  setWashingMode(16);
//  delay(1000);
}
