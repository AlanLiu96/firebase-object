//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseRoom_ESP8266 is a sample that demo using multiple sensors
// and actuactor with the FirebaseArduino library.

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "aib-object.firebaseio.com"
#define FIREBASE_AUTH "Jh04XGLCRZco3K0EBwxXrSoaEY8zGUi9x5MS1WcJ"
#define WIFI_SSID "yale wireless"
#define WIFI_PASSWORD ""

//set led to pin built in led
const int led = BUILTIN_LED;
int brightness = 0;    // how bright the LED is
int fadeAmount = 100;    // how many points to fade the LED by
int pattern = 0; // enum, 0 --> start/neutral, 1 --> fade, 2 --> blink
String mac = WiFi.macAddress();
void setup() {
  Serial.begin(115200);
  delay(500);

   Serial.println();
   Serial.print("MAC: ");
   Serial.println();

  pinMode(led, OUTPUT);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  // Firebase.setString("Mac", mac);
  Firebase.set(mac + "/pattern", 0);
  Firebase.set(mac + "/brightness", 255);
}

void loop() {

  pattern = Firebase.getInt(mac+ "/pattern");
  if (pattern == 0){ // manual control
    brightness = Firebase.getInt(mac + "/brightness")
  } else if (pattern == 1){ // fade
    brightness = brightness + fadeAmount;

    // reverse the direction of the fading at the ends of the fade:
    if (brightness <= 0 || brightness >= 1023) {
      fadeAmount = -fadeAmount;
    }
  } else if (pattern == 2){ // blink
    brightness = brightness < 500 ? 500 : 0;
  }

  // set the brightness of built in led:
  analogWrite(led, Firebase.getInt(mac + "/brightness"));

  Serial.println(brightness);
  // wait for 100 milliseconds to see the pattern effect
  delay(100);
}
