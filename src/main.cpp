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
#include "Car.h"
#include "Motor_TB6612FNG.h"
#include "UltraSound.h"
#include <ESP32Servo.h>
#include "Led.h"
#include "DataChannel.h"
// #include "Hall.h"
#include "AudioRecorder.h"
#include "SPIFFSServer.h"

using namespace std;
using namespace websockets;


extern QMC5883LCompass compass;
extern iBeaconFinder finder;
extern Net net;

SPIFFSServer fileWebServer;

#define I2S_DOUT      15
#define I2S_BCLK      16
#define I2S_LRC       17


// Audio audio; 
void audioTask(void* params) {
  Audio audio;  
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(18); // 0...21
  //连接网络不能放在死循环中,loop一次就会播放一次
  audio.connecttohost("http://192.168.2.153:4000/voice.mp3");

  while(1) {
    audio.loop();
    // delay(15000);
  }
}
Camera camera;

Motor_TB6612FNG l = Motor_TB6612FNG(17, 18, 16, 8);
// Motor_TB6612FNG l = Motor_TB6612FNG(20, 21, 19, 8);
Motor_TB6612FNG r = Motor_TB6612FNG(3, 46, 9, 8);
// Servo s = Servo(19);
UltraSound us = UltraSound(7, 15);
Car car = Car(&l, &r, /*&s,*/ &us);

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

  net.setUpWifi();
  net.setUpWebsocket([](int cmd, JsonDocument& doc) {
    Serial.print(cmd);
    switch(cmd) {
      case CMD_SET_DIRECTION: {
          float d = doc["data"];
          Serial.print(" direction=");
          Serial.println(d);
          car.direct(d);
        }
        break;
      case CMD_SET_SPEED: {
          float speed = doc["data"];
          Serial.print(" speed=");
          Serial.println(speed);
          car.drive(speed);
        }
        break;
    }
  });

  // xTaskCreatePinnedToCore(audioTask, "AudioTask", 8192 * 2, NULL, 1, NULL, 0);

  // startTasks();

  fileWebServer.setup();
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
  // audio.connecttohost("http://192.168.2.153:4000/voice2.mp3");

  // camera.setUp();
  // camera.startStreamServer();

  // ESP32PWM::allocateTimer(0);
	// ESP32PWM::allocateTimer(1);
	// ESP32PWM::allocateTimer(2);
	// ESP32PWM::allocateTimer(3);
	// myservo.setPeriodHertz(50);    // standard 50 hz servo
	// myservo.attach(servoPin, 1000, 2000); // attaches the servo on pin 18 to the servo object
	// }

  // pinMode(servoPin, OUTPUT);

	// myservo.write(0);

  // led.setFlickerInterval(500);
  // led.setFadeMount(10);
  // car.setSpeed(0.5);
  // initHall();
}

float step = 0.1;
int pos = 0;    // variable to store the servo position

void loop() {
  net.loop();
  fileWebServer.loop();
  // car.speedDown();
  // delay(1000);
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
  // led.flicker();
  // led.autoFade();


}