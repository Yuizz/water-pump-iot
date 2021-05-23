#include <Arduino.h>
#include <Ubidots.h>
#include <Servo.h>
#include <ESP8266WiFi.h>

#include "credentials.h"

#define PIN_SERVO D2

const int OFF = 0;
const int AUTO = 45;
const int MANUAL = 90;

Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP);
Servo myServo;
int servo_state;
int previous_servo_state;
int tries = 0;

void setup(){
  Serial.begin(115200);
  myServo.attach(PIN_SERVO);
  myServo.write(AUTO);
  delay(1000);
  myServo.detach();

  Serial.print("Connecting");
  ubidots.wifiConnect(WIFI_SSID, WIFI_PASS);
  servo_state = 0;
  previous_servo_state = servo_state;
}

void loop(){
    servo_state = ubidots.get("ejc-main-esp8266", "Servo");
    Serial.print(servo_state);

    if (servo_state != ERROR_VALUE){
      tries = 0;
      if (servo_state != previous_servo_state){
        myServo.attach(PIN_SERVO);
        delay(10);
        if (servo_state == 0) myServo.write(AUTO);
        if (servo_state == 1) myServo.write(MANUAL);
        previous_servo_state = servo_state;
        delay(500);
        myServo.detach();
      }
    }else{
      Serial.println("...ERROR_VALUE");
      Serial.print("Reconnecting");
      ubidots.wifiConnect(WIFI_SSID, WIFI_PASS);
      tries++;
      if (myServo.read() == MANUAL){
        myServo.attach(PIN_SERVO);
        myServo.write(AUTO);
        delay(500);
        myServo.detach();
        }
    }

  if(tries>=5){
    Serial.println("Restarting...");
    ESP.restart();
  }
    delay(1000);
}