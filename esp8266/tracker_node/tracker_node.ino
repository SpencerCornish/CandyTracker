#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>


extern "C" {
#include "user_interface.h"
}




FirebaseData firebaseData;

#define WIFI_SSID "WIFI_SSID"
#define WIFI_PASSWORD "WIFI_PASS"
#define FIREBASE_HOST "HOST"
#define FIREBASE_AUTH "TOKEN"

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.begin(115200);

  byte rtcStore[2];
  system_rtc_mem_read(65, rtcStore, 2);
  Serial.print("current value = ");
  Serial.println(*rtcStore);

  if (*rtcStore != 0) {
    Serial.println("First Run, going back to sleep for now...");
    *rtcStore = 0;
    system_rtc_mem_write(65, rtcStore, 2); //offset is 65
    ESP.deepSleep(0);
  }

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("*");
  }

  Serial.println("");
  Serial.println("WiFi connection Successful");
  Serial.print("The IP Address of ESP8266 Module is: ");
  Serial.print(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Firebase.setMaxRetry(firebaseData, 3);
  Firebase.setMaxErrorQueue(firebaseData, 30);

  if (Firebase.pushTimestamp(firebaseData, "/log"))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.print("PUSH NAME: ");
    Serial.println(firebaseData.pushName());

    digitalWrite(LED_BUILTIN, LOW);
    delay(400);
    digitalWrite(LED_BUILTIN, HIGH);

  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
    for (int i = 0; i < 50; i++) {
      delay(30);
      digitalWrite(LED_BUILTIN, LOW);
      delay(30);
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }


  Serial.println("GOING TO SLEEP");
  ESP.deepSleep(0);
}


void loop(){}
