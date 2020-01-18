/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP8266 chip.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right ESP8266 module
  in the Tools -> Board menu!

  NOTE: SmartConfig might not work in your environment.
        Please try basic ESP8266 SmartConfig examples
        before using this sketch!

  Change Blynk auth token to run :)

 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266Ping.h>
#include <SPI.h>
#include <EEPROM.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "9BX4rqLk0uvOEdJoKgGNDIbXXmiJNWA9";
char ssid[] = "Borey R-J37";
char pass[] = "";

int ledPin = 4;
int avg_time_ms;
int fadeValue;
int monitoring_state = 1;
int start_indicator = 1;
const char* remote_host = "www.google.com";

void setup()
{
  Serial.begin(9600);
  EEPROM.begin(32);
  Blynk.begin(auth, ssid, pass, IPAddress(10,254,253,248), 8080);
}

BLYNK_WRITE(V0) {
  monitoring_state = param.asInt(); // Gets the value stored in V2 as an integer
}

void loop()
{
  Blynk.run();
  if (start_indicator == 1) {
    monitoring_state = EEPROM.read(0);
    Blynk.virtualWrite(V0, monitoring_state);
    start_indicator = 0;
  }

  EEPROM.put(0, monitoring_state);
  EEPROM.commit();
  
  Ping.ping(remote_host, 2);  //10 time ping to google, You can change value to higher or lower
  avg_time_ms = Ping.averageTime(); // reading string and Int for easy display integration.
  fadeValue = 255 - avg_time_ms;
  if (fadeValue <= 0)
  fadeValue = 5;

  Serial.println(monitoring_state);
  
  if (monitoring_state == 1){
    analogWrite(ledPin, fadeValue);
    Blynk.virtualWrite(V1, "PING MODE");
    Blynk.virtualWrite(V2, avg_time_ms);
  }
  else{
    digitalWrite(ledPin, LOW);
    Blynk.virtualWrite(V1, "SILENCE MODE");
  }
}
