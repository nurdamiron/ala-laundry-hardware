#define WIFI_SSID "TechnoPark"
#define WIFI_PASSWORD "techno2020"
#define API_KEY "AIzaSyDV1GlR0n3e6oVAPE7dauJy09LQj-vcoXY"
#define DATABASE_URL "https://fir-esp32-project-default-rtdb.firebaseio.com"
#define USER_EMAIL "USER_EMAIL"
#define USER_PASSWORD "USER_PASSWORD"

FirebaseData stream;
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

FirebaseJsonData mode;
FirebaseJsonData trigger;
FirebaseJsonData door_status;

int default_mode = 7;
int local_on_off = 0;
int local_run = 0;
int local_start_stop = 0;
int local_door_status = 0;
int local_trigger = 0;

// pins
const int plus_pin = 2;     // button +
//const int minus_pin = ;      // button -
int encoder_pin_1 = 26; // encoder pin 1
int encoder_pin_2 = 25; // encoder pin 2
int on_off_pin = 14;    // on/off button
int start_stop_pin = 27; // start/stop button
int on_off_status = 0;
int start_stop_status = 0;
int washing_mode = 7; // 12 modes in total, default is mode 6
int door_pin = 13; // sensor pin to detect if the door is open or closed, uses pin 13 to avoid conflicts with other pins
int trigger_pin = 4;

unsigned long send_data_prev_millis = 0;
int count = 0;
bool signup_ok = false;

volatile bool data_changed = false;

void streamCallback(FirebaseStream data)
{
  if (data.dataTypeEnum() == fb_esp_rtdb_data_type_json) {
  FirebaseJson *json = data.to<FirebaseJson *>();
  
  // Get washing mode value from Firebase
  if (json->get(mode, "mode")) {
    washing_mode = mode.to<int>();
    Serial.print("Washing mode: ");
    Serial.println(washing_mode);
  }

  // Get wash trigger value from Firebase
  if (json->get(trigger, "trigger")) {
    local_trigger = trigger.to<int>();
    Serial.print("Wash trigger: ");
    Serial.println(local_trigger);
  }

  // Get door status value from Firebase
  if (json->get(door_status, "door_status")) {
    local_door_status = door_status.to<int>();
    Serial.print("Door status: ");
    Serial.println(local_door_status);

    // If the washing trigger is true and the door is closed, start the washing process
    if (local_trigger == 1 && local_door_status == 0) {
      local_run = 1; // Set local_run to true to start the washing process
      digitalWrite(trigger_pin, HIGH); // Turn on the trigger pin to activate the washing process
    } else {
      local_run = 0; // Set local_run to false to stop the washing process
      digitalWrite(trigger_pin, LOW); // Turn off the trigger pin to stop the washing process
    }
  }
  
if (true) {
  onOff(); // switch on the washing machine
  int diffMode = default_mode - washing_mode; // calculate the difference between the default mode and the current washing mode
  washing_mode = default_mode; // update the washing mode to the default mode
  encoderScroll(diffMode); // scroll the encoder by the calculated difference
  Serial.print("diffMode: ");
  Serial.println(diffMode);
  setDoor(1); // lock the door
  startStop(); // start the washing machine
  washing_mode = 7; // set the washing mode back to the default value
}
else {
  Serial.println("else is running");
  if (data.dataPath().equals("/mode")) { // check if the updated data is the washing mode
    default_mode = data.to<int>(); // update the default mode with the received value
    Serial.println(default_mode);
  }
  if (data.dataPath().equals("/trigger")) { // check if the updated data is the door trigger
    local_trigger = data.to<int>(); // update the local trigger value with the received value
    Serial.println(local_trigger);
  }
  if (local_trigger == 1) { // if the door is triggered
     Serial.println("local_trigger = 1 ");
     onOff(); // switch on the washing machine
     int diffMode = default_mode - washing_mode; // calculate the difference between the default mode and the current washing mode
     washing_mode = default_mode; // update the washing mode to the default mode
     encoderScroll(diffMode); // scroll the encoder by the calculated difference
     delay(2000);
     Serial.print("diffMode: ");
     Serial.println(diffMode);
     setTrigger(1); // lock the door
     startStop(); // start the washing machine
     default_mode = 7; // set the default mode back to the default value
     washing_mode = default_mode; // set the washing mode back to the default value
     Serial.println(default_mode);
  }
  else { // if the door is not triggered
    onOff(); // switch off the washing machine
    washing_mode = 7; // set the washing mode back to the default value
  }
 }
}

data_changed = true; // indicate that data has been changed
}

