#include <arduino.h>
#include <Ticker.h>
#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include <string>
#include "iBeacon.h"
#include "QMC5883LCompass.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "net.h"
#include "Audio.h"
#include "Camera.h"
#include "Command.h"
// #include "Car.h"
#include "Motor_TB6612FNG.h"
#include "UltraSound.h"
#include <ESP32Servo.h>
#include "Led.h"
// #include "Hall.h"

using namespace std;
using namespace websockets;

iBeaconFinder finder;
Net net;

#define I2S_DOUT      40
#define I2S_BCLK      41
#define I2S_LRC       42

void scanIBeacons() {
  Serial.println("scanIBeacons");
  finder.find();
  vector<iBeacon> devices = finder.getDevices();
  // if (devices.size() >= 3) {
  //   string mac1 = devices[0].address;
  //   string mac2 = devices[1].address;
  //   string mac3 = devices[2].address;

  //   int rssi1 = devices[0].rssi;
  //   int rssi2 = devices[1].rssi;
  //   int rssi3 = devices[2].rssi;

    
  //   std::stringstream ss;
  //   ss << "http://192.168.2.153:3000/locate?"  << "mac1=" << mac1 << "&mac2=" << mac2 << "&mac3=" << mac3 << "&rssi1=" << rssi1 << "&rssi2=" << rssi2 << "&rssi3=" << rssi3;

  //   Serial.println(ss.str().c_str());

  //   ::httpGet(ss.str());
  // }
  std::stringstream ss;
  ss << "http://192.168.2.153:3000/locate?";
  for (int i = 0; i < devices.size(); i++) {
    ss << "mac" << i + 1 << "=" << devices[i].address << "&rssi" << i + 1 << "=" << devices[i].rssi << "&";
  }
  // Serial.println(ss.str().c_str());
  Net::httpGet(ss.str());
}

  
QMC5883LCompass compass;
void compassTask(void* params) {
  // Serial.begin(9600);
  // Serial.println("test");

  compass.init(11, 10);
  int a;
  while(1) {
    // compassTask(NULL);

    // Read compass values
    compass.read();
    // Return Azimuth reading
    a = compass.getAzimuth() + 180;
    string s = CommandBuilder::CreateCodeJson(CMD_DIRECTION, a);
    // Serial.println(s.c_str());
    net.ws().send(s.c_str());
    Serial.print("A: ");
    Serial.print(a);
    Serial.println();
    delay(1000);
  }
}


// Ticker beaconTimer(scanIBeacons, 1000, 0, MILLIS);

Audio audio; 
// void audioTask(void* params) {
//   Audio audio;  
//   audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
//   audio.setVolume(10); // 0...21
//   while(1) {
//     audio.connecttohost("http://192.168.1.4:3000/voice2.mp3");
//     audio.loop();
//     delay(15000);
//   }
// }
Camera camera;

Motor_TB6612FNG l = Motor_TB6612FNG(17, 18, 16, 8);
// Motor_TB6612FNG l = Motor_TB6612FNG(20, 21, 19, 8);
Motor_TB6612FNG r = Motor_TB6612FNG(3, 46, 9, 8);
// Servo s = Servo(19);
UltraSound us = UltraSound(7, 15);
// Car car = Car(&l, &r, &s, &us);

#define SERVO_PIN   0
#define MAX_WIDTH   2500
#define MIN_WIDTH   1000

static const int servoPin = 35;

Servo myservo;
void ultrSoundTask(void* params) {
  while (1)
  {
    int d = us.getDistance();
    Serial.println(d);
  }
  
}

// void servoTask(void* params) {
//   s.to(45);
//   delay(1000);
//   s.to(90);
//   delay(1000);
//   s.to(120);
//   delay(1000);
// }
#include "Hall.h"
extern void initHall();

Led led = Led(14);
void setup() {
  Serial.begin(9600);
  
  // create compass task
  xTaskCreatePinnedToCore(compassTask, "CompassTask", 4096, NULL, 1, NULL, 0);
  // xTaskCreatePinnedToCore(ultrSoundTask, "ultrSoundTask", 4096, NULL, 1, NULL, 1);
  // xTaskCreatePinnedToCore(servoTask, "servoTask", 2048, NULL, 1, NULL, 1);


  // xTaskCreatePinnedToCore(audioTask, "AudioTask", 8192 * 2, NULL, 1, NULL, 0);


  // init ble,wifi,websocket
  finder.init();
  net.setUpWifi();
  net.setUpWebsocket();
  // start timeer
  // beaconTimer.start();

  // WiFi.begin("Stark", "fengzhiping,1101");

  // Serial.println("Connecting to WiFi...");
  // while (WiFi.status()!= WL_CONNECTED) {
  //   delay(1000);
  //   Serial.println(".");
  // }
  // Serial.print("Camera Ready! Use 'http://");
  // Serial.print(WiFi.localIP());
  // Serial.println("' to connect");

  // audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  // audio.setVolume(18); // 0...21
  // audio.connecttohost("http://192.168.1.4:3000/voice.mp3");

  // camera.setUp();
  // camera.startStreamServer();

  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(servoPin, 1000, 2000); // attaches the servo on pin 18 to the servo object
	// }

  // pinMode(servoPin, OUTPUT);

	myservo.write(0);

  led.setFlickerInterval(500);
  led.setFadeMount(10);

  l.setSpeed(0.5);
  l.backward();
  r.backward();

  // initHall();
}

float step = 0.1;
int pos = 0;    // variable to store the servo position

void loop() {
  net.loop();
  // beaconTimer.update(); 
  // audio.loop();
  // // delay(15000);

  // for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
	// 	// in steps of 1 degree
	// 	myservo.write(pos);    // tell servo to go to position in variable 'pos'
	// 	delay(15);             // waits 15ms for the servo to reach the position
	// }
	// for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
	// 	myservo.write(pos);    // tell servo to go to position in variable 'pos'
	// 	delay(15);             // waits 15ms for the servo to reach the position
	// }
  float speed = l.getSpeed();
  if (speed >= 1.0) {
    step = -step;
  } else if (speed <= 0.3) {
    step = -step;
  }
  speed = speed + step;
  Serial.println(speed);
  // r.setSpeed(speed);
  // r.setSpeed(speed);
  delay(1000);
  r.forward();
  l.forward();
  delay(1000);
  l.backward();
  r.backward();
  // led.flicker();
  // led.autoFade();


}