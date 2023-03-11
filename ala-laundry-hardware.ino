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

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;


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
  dataChanged = true;  
}
void streamTimeoutCallback(bool timeout)
{
  if(timeout){
    // Stream timeout occurred
    Serial.println("Stream timeout, resume streaming...");
  }  
}



void setup(){
  Serial.begin(115200);
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

  if (!Firebase.RTDB.beginStream(&stream, "/id1"))
    Serial.printf("sream begin error, %s\n\n", stream.errorReason().c_str());
  Firebase.RTDB.setStreamCallback(&stream, streamCallback, streamTimeoutCallback);
}
void loop()
{
 if (Firebase.ready())
  {
    // Firebase is ready to use now.
    
  } 
}
