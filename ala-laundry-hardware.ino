#include "GyverButton.h"

#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#define WIFI_SSID "TechnoPark"  
#define WIFI_PASSWORD "techno2020"
#define API_KEY "AIzaSyB95Rp0pvwjcFi0dHEvvrRh0svfTkuL7MA"
#define DATABASE_URL "https://ala-laundry-902e5-default-rtdb.firebaseio.com"
//#define USER_EMAIL "laundry01@gmail.com"
//#define USER_PASSWORD "12345678"
#define USER_EMAIL "USER_EMAIL"
#define USER_PASSWORD "USER_PASSWORD"
#define WM_MACHINE_STREAM_DIRECTORY "/id5/input"

#define INPUT_PIN_BUTTON 13
#define OUTPUT_PIN_BUTTON 14
const int outB = 25;//либо const дегенды колдануга болады
const int outA = 26;
const int startStop = 27;
const int timeEncoder = 100;

FirebaseData stream;
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

FirebaseJsonData washModeJson;
FirebaseJsonData washTriggerJson;
FirebaseJsonData premOnOffModeJson;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
unsigned long count = 0;

boolean onOffMode = false;
int premOnOffMode = 0;
boolean ssMode = false;


int washMode = 1;
int washTrigger = 0;
int setWashMode = 0;


GButton but1(INPUT_PIN_BUTTON);


void streamTimeoutCallback(bool timeout)
{
  if(timeout){
    // Stream timeout occurred
    Serial.println("Stream timeout, resume streaming...");
  }  
}

void startStops()
{
  ssMode = !ssMode;
  digitalWrite(startStop,HIGH);
  delay(100);
  digitalWrite(startStop,LOW);
  Serial.println("StartStop");
}
//void encoderScroll(int scrollValue){
//  if(scrollValue > 0){
//    for(int i = 0 ; i < scrollValue; i++){
//      Serial.print("Выполняеться скорл назад, по счету: ");
//      Serial.println(i);
//      digitalWrite(outB, HIGH);
//      delay(timeEncoder);
//      digitalWrite(outA, HIGH);
//      delay(timeEncoder);
//      digitalWrite(outB, LOW);
//      delay(timeEncoder);
//      digitalWrite(outA, LOW);
//      delay(timeEncoder);
//    }
////    digitalWrite(outB, LOW);
//  }
//  else if(scrollValue < 0){
//    for(int i = 0 ; i < abs(scrollValue); i++){
//      Serial.print("Выполняеться скорл впередь по счету: ");
//      Serial.println(i);
//      digitalWrite(outA, HIGH);
//      delay(timeEncoder);
//      digitalWrite(outB, HIGH);
//      delay(timeEncoder);
//      digitalWrite(outA, LOW);
//      delay(timeEncoder);
//      digitalWrite(outB, LOW);
//      delay(timeEncoder);
//    }
////    digitalWrite(outA, LOW);
//  }
//  Serial.println("Конец метода скролиннга: ");
//}
//void setWashingMode(int setWashValue){
//  Serial.print("Режим который стоит: ");
//  Serial.println(washMode);
//  int shiftValue = washMode - setWashValue;
//  washMode = setWashValue;
//  Serial.print("Режим который выбран: ");
//  Serial.println(washMode);
//  Serial.print("На сколько делений должны перемещаться: ");
//  Serial.println(shiftValue);
//  encoderScroll(shiftValue);
//}
void streamCallback(FirebaseStream data)
{
  Serial.printf("stream path, %s\nevent path, %s\ndata type, %s\nevent type, %s\n\n",
                data.streamPath().c_str(),
                data.dataPath().c_str(),
                data.dataType().c_str(),
                data.eventType().c_str());
  printResult(data); // see addons/RTDBHelper.h
  Serial.println();
  Serial.printf("Received stream payload size: %d (Max. %d)\n\n", data.payloadLength(), data.maxPayloadLength());

  if(data.dataTypeEnum() == fb_esp_rtdb_data_type_json){
    FirebaseJson *json = data.to<FirebaseJson *>();

    json->get(premOnOffModeJson, "admin");
    
    if(premOnOffModeJson.success){
      premOnOffMode = premOnOffModeJson.to<int>();
      Serial.print("prem on off mode: ");
      Serial.println(premOnOffMode);
    }
  }
}
void setup()
{
  Serial.begin(115200);
  pinMode(OUTPUT_PIN_BUTTON,OUTPUT);
  pinMode(INPUT_PIN_BUTTON,INPUT_PULLUP);
  pinMode(outA,OUTPUT);
  pinMode(outB,OUTPUT);
  pinMode(startStop,OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.api_key = API_KEY;
  
  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  if (!Firebase.RTDB.beginStream(&stream, WM_MACHINE_STREAM_DIRECTORY))
    Serial.printf("stream begin error, %s\n\n", stream.errorReason().c_str());
  Firebase.RTDB.setStreamCallback(&stream, streamCallback, streamTimeoutCallback);
}

void loop()
{
 // Firebase.ready() should be called repeatedly to handle authentication tasks.
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 1500 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    if(Firebase.RTDB.setInt(&fbdo, "/id5/output/timer", sendDataPrevMillis)){
      Serial.println(premOnOffMode);
    }else{
      Serial.println(fbdo.errorReason());
    }
  }
  but1.tick();
  if (but1.isPress()) {
    Serial.print("Текущее состоняие: ");
    Serial.println(premOnOffMode);
    if(onOffMode == 0){
      Serial.println("отключен");
    }else{
      Serial.println("включен");
    }
    if(premOnOffMode){
      digitalWrite(OUTPUT_PIN_BUTTON, HIGH);
      delay(80);
      digitalWrite(OUTPUT_PIN_BUTTON, LOW);
    }else{
      Serial.println("Button is blocked");
    }
    onOffMode = !onOffMode;
  }
}
