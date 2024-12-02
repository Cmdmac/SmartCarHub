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
#include "Mic.h"
#include "AnalogMic.h"
#include "Speaker.h"
#include "FileWebServer.h"

#include "QMI8658Handler.h"
#include "PCA9555.h"

PCA9555 ioport(0x27, 4, 5);

using namespace std;
using namespace websockets;


extern QMC5883LCompass compass;
extern iBeaconFinder finder;
extern Net net;

FileWebServer fileWebServer;
QMI8658Handler qmi8658Handler;
// Mic mic;

// Speaker speaker; 
// Camera camera;

// Motor_TB6612FNG l = Motor_TB6612FNG(17, 18, 16, 8);
// // Motor_TB6612FNG l = Motor_TB6612FNG(20, 21, 19, 8);
// Motor_TB6612FNG r = Motor_TB6612FNG(3, 46, 9, 8);
// // Servo s = Servo(19);
// UltraSound us = UltraSound(7, 15);
// Car car = Car(&l, &r, /*&s,*/ &us);

// #define SERVO_PIN   0
// #define MAX_WIDTH   2500
// #define MIN_WIDTH   1000

// static const int servoPin = 35;

// Servo myservo;
// void ultrSoundTask(void* params) {
//   while (1)
//   {
//     int d = us.getDistance();
//     Serial.println(d);
//   }
  
// }

// #include "Hall.h"
// extern void initHall();

Led led = Led();

// AnalogMic analogMic;

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
          // car.direct(d);
        }
        break;
      case CMD_SET_SPEED: {
          float speed = doc["data"];
          Serial.print(" speed=");
          Serial.println(speed);
          // car.drive(speed);
        }
        break;
    }
  });

  // startTasks();

  // fileWebServer.setup();
  // analogMic.setup();
  // analogMic.record("/analogvoice.wav", 5);

  // mic.setup(-1, -1, -1);
  // mic.recordWav("", 20, SAMPLE_RATE, SAMPLE_BITS);

  // camera.setUp();
  // camera.startStreamServer();

  // speaker.setup();
  // speaker.play("http://192.168.2.153:4000/voice.mp3")

  // led.setFlickerInterval(500);
  // led.setFadeMount(10);
  // car.setSpeed(0.5);
  // initHall();
  // led.setUp(0);

  // SPIClass spi;
  // spi.begin(SPI_CLK, SPI_MISO, SPI_MOSI, SPI_CS);
  // if (!SD.begin(SPI_CS, spi)) {
  //     Serial.println("sdcard init failure");
  // }

  // File file = SD.open("/", "r");
  // if (file.isDirectory()) {
  //   Serial.println("is dir");
  // }

  // qmi.begin(Wire, 14, 13, QMI8658_L_SLAVE_ADDRESS);
    //Using WIRE !!
  
  //
	// start I2C
	//
	ioport.begin();
	//
	// sets the I2C clock to 400kHz
	//
	ioport.setClock(400000);
	//
	// set first 14 pins to output
	//
	for (uint8_t i = 0; i < 17; i++){
		ioport.pinMode(i, OUTPUT);
	}
	//
	// set pin 15  (=digital 15) to Input
	//
	// ioport.pinMode(ED14, INPUT);
}

float step = 0.1;
int pos = 0;    // variable to store the servo position

void loop() {
  net.loop();
  // speaker.loop();
  // car.speedDown();
  // delay(1000);
  // beaconTimer.update(); 
  // audio.loop();
  // // delay(15000);

  // led.flicker();
  // led.autoFade();

  //
	// check if button is LOW
	//
	// if (ioport.digitalRead(ED14) == LOW) {
		//
		// turn all 14 leds on
		//
		for (uint8_t i = 0; i < 17; i++){
			ioport.digitalWrite(i, HIGH);
			delay(10);
		}
		//
		// turn all 14 leds off
		//
		for (uint8_t i = 0; i < 17; i++){
			ioport.digitalWrite(i, LOW);
			delay(10);
		}
	// }
}