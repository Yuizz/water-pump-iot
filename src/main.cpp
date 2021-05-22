#include <Arduino.h>
#include <Ubidots.h>
#include <Servo.h>

#define PIN_SERVO D2

const char* UBIDOTS_TOKEN = "BBFF-dRIAoPolqh0cwVMi6ha1YJS0bFhgY5";  // Put here your Ubidots TOKEN
const char* WIFI_SSID = "INFINITUM518A_2.4_plus";      // Put here your Wi-Fi SSID
const char* WIFI_PASS = "9YMRaeUrHH";      // Put here your Wi-Fi password

const int OFF = 0;
const int AUTO = 45;
const int MANUAL = 90;

Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP);
Servo myServo;
int servo_state;
int previous_servo_state;


void setup() {    
  Serial.begin(115200);
  myServo.attach(PIN_SERVO);
  myServo.write(AUTO);
  delay(1000);
  myServo.detach();

  ubidots.wifiConnect(WIFI_SSID, WIFI_PASS); 
  servo_state=0;
  previous_servo_state = servo_state;
}

void loop() {

  if(!ubidots.wifiConnected()) ubidots.wifiConnect(WIFI_SSID, WIFI_PASS);
  
  servo_state = ubidots.get("ejc-main-esp8266", "Servo");
  Serial.println(servo_state);

  if(servo_state != ERROR_VALUE){
    if(servo_state != previous_servo_state){
      myServo.attach(PIN_SERVO);
      if ( servo_state == 0 ) myServo.write(AUTO);
      if ( servo_state == 1 ) myServo.write(MANUAL);
      previous_servo_state = servo_state;
    }else{
      myServo.detach();
    }
  }
  else{ 
    if(myServo.read() == MANUAL) myServo.write(AUTO);
    delay(1000);
    myServo.detach();
  }

  delay(1000);
}