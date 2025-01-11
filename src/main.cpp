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
// #include "PCA9555.h"
// #include "Gesture.h"
// #include "RevEng_PAJ7620.h"
// #include "paj7620.h"
// #include <Arduino_APDS9960.h>
// #include "IR.h"
// #define DECODE_DENON        // Includes Sharp
// #define DECODE_JVC
// #define DECODE_KASEIKYO
// #define DECODE_PANASONIC    // alias for DECODE_KASEIKYO
// #define DECODE_LG
// #define DECODE_NEC          // Includes Apple and Onkyo
// #define DECODE_SAMSUNG
// #define DECODE_SONY
// #define DECODE_RC5
// #define DECODE_RC6

// #define DECODE_BOSEWAVE
// #define DECODE_LEGO_PF
// #define DECODE_MAGIQUEST
// #define DECODE_WHYNTER
// #define DECODE_FAST
// #define DECODE_DISTANCE_WIDTH

// #include <IRremoteESP8266.h>
// #include <IRrecv.h>
// #include <IRutils.h>
// PCA9555 ioport(0x27, 4, 5);

using namespace std;
using namespace websockets;


extern QMC5883LCompass compass;
extern iBeaconFinder finder;
extern Net net;

FileWebServer fileWebServer;
// QMI8658Handler qmi8658Handler;

/* 
Notice: When you want to recognize the Forward/Backward gestures, your gestures' reaction time must less than GES_ENTRY_TIME(0.8s). 
        You also can adjust the reaction time according to the actual circumstance.
*/
#define GES_REACTION_TIME		500				// You can adjust the reaction time according to the actual circumstance.
#define GES_ENTRY_TIME			800				// When you want to recognize the Forward/Backward gestures, your gestures' reaction time must less than GES_ENTRY_TIME(0.8s). 
#define GES_QUIT_TIME			1000
// PAJ7620 Gesture;
// #define GES_REACTION_TIME		500				// You can adjust the reaction time according to the actual circumstance.
// #define GES_ENTRY_TIME			800				// When you want to recognize the Forward/Backward gestures, your gestures' reaction time must less than GES_ENTRY_TIME(0.8s). 
// #define GES_QUIT_TIME			1000

Mic mic;

Speaker speaker; 
// Camera camera;
// IR ir;

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

// IRrecv irrecv(8);

// decode_results results;
// AnalogMic analogMic;
void irCallback() {
  Serial.println("irCallback");
}

void setup() {
  Serial.begin(115200);

  // net.setUpWifi();
  // net.setUpWebsocket([](int cmd, JsonDocument& doc) {
  //   Serial.print(cmd);
  //   switch(cmd) {
  //     case CMD_SET_DIRECTION: {
  //         float d = doc["data"];
  //         Serial.print(" direction=");
  //         Serial.println(d);
  //         // car.direct(d);
  //       }
  //       break;
  //     case CMD_SET_SPEED: {
  //         float speed = doc["data"];
  //         Serial.print(" speed=");
  //         Serial.println(speed);
  //         // car.drive(speed);
  //       }
  //       break;
  //   }
  // });

  // startTasks();

  // fileWebServer.setup();
  // analogMic.setup();
  // analogMic.record("/analogvoice.wav", 5);

  mic.setup(38, 37, 36);
  mic.recordWav("", 20, SAMPLE_RATE, SAMPLE_BITS);

  // camera.setUp();
  // camera.startStreamServer();

  // ir.setup(8);
  // Irrecv.begin(8, false)
    // irrecv.enableIRIn();  // Start the receiver
  // speaker.setup();
  // speaker.play("http://192.168.2.153:4000/voice.mp3");

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
  
//  if(Gesture.init()) {
//         Serial.println("PAJ7620U2 initialization failed");
//     } else {
//         Serial.println("PAJ7620U2 initialization success");
//     }
//     Serial.println("Please input your gestures:\n");

// bool r = Wire.setPins(20, 19);
// Serial.println(r);


//     uint8_t error = 0;

// 	Serial.begin(9600);
// 	Serial.println("\nPAJ7620U2 TEST DEMO: Recognize 9 gestures.");

// 	error = paj7620Init();			// initialize Paj7620 registers
// 	if (error) 
// 	{
// 		Serial.print("INIT ERROR,CODE:");
// 		Serial.println(error);
// 	}
// 	else
// 	{
// 		Serial.println("INIT OK");
// 	}
// 	Serial.println("Please input your gestures:\n");
	//  if (!APDS.begin()) {
    // Serial.println("Error initializing APDS-9960 sensor.");
    // while (true); // Stop forever
//   }
}

// float step = 0.1;
// int pos = 0;    // variable to store the servo position

// int proximity = 0;
// int r = 0, g = 0, b = 0;
// unsigned long lastUpdate = 0;
void loop() {
  net.loop();
  // car.speedDown();
  // delay(1000);
  // beaconTimer.update(); 
  // audio.loop();
  // // delay(15000);

  // led.flicker();
  // led.autoFade();

  // int v = touchRead(1);
  // Serial.println(v);

//  if (irrecv.decode(&results)) {
//     // print() & println() can't handle printing long longs. (uint64_t)
//     serialPrintUint64(results.value, HEX);
//     serialPrintUint64(results.address, HEX);
//     Serial.print("cmd");
//     Serial.println(results.command);
//     Serial.print("type:");
//     Serial.println(results.decode_type);
//     Serial.println("");
//     irrecv.resume();  // Receive the next value
//   }
  // if (IrReceiver.decode()) {

  //       Serial.print(F(", decoded address: "));
  //       Serial.print(IrReceiver.decodedIRData.address, HEX);
  //       Serial.print(F(", decoded command: "));
  //       Serial.println(IrReceiver.decodedIRData.command, HEX);
  //       Serial.print(F(", decoded protocol: "));
  //       Serial.println(IrReceiver.decodedIRData.protocol, HEX);
  //       Serial.print(F(", decoded decodedRawData: "));
  //       Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
        
  //       /*
  //        * Print a summary of received data
  //        */
  //       if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
  //           Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
  //           // We have an unknown protocol here, print extended info
  //           // IrReceiver.printIRResultRawFormatted(&Serial, true);
  //           IrReceiver.resume(); // Do it here, to preserve raw data for printing with printIRResultRawFormatted()
  //       } else {
  //           // mCallback();
  //           IrReceiver.resume(); // Early enable receiving of the next IR frame
  //           // IrReceiver.printIRResultShort(&Serial);
  //           // IrReceiver.printIRSendUsage(&Serial);
  //       }
  //       Serial.println();

  //       /*
  //        * Finally, check the received data and perform actions according to the received command
  //        */
  //       if (IrReceiver.decodedIRData.command == 0x10) {
  //           // do something
  //       } else if (IrReceiver.decodedIRData.command == 0x11) {
  //           // do something else
  //       }
  //   }


// uint8_t data = 0, data1 = 0, error;
	
// 	error = paj7620ReadReg(0x43, 1, &data);				// Read Bank_0_Reg_0x43/0x44 for gesture result.
// 	if (!error) 
// 	{
// 		switch (data) 									// When different gestures be detected, the variable 'data' will be set to different values by paj7620ReadReg(0x43, 1, &data).
// 		{
// 			case GES_RIGHT_FLAG:
// 				delay(GES_ENTRY_TIME);
// 				paj7620ReadReg(0x43, 1, &data);
// 				if(data == GES_FORWARD_FLAG) 
// 				{
// 					Serial.println("Forward");
// 					delay(GES_QUIT_TIME);
// 				}
// 				else if(data == GES_BACKWARD_FLAG) 
// 				{
// 					Serial.println("Backward");
// 					delay(GES_QUIT_TIME);
// 				}
// 				else
// 				{
// 					Serial.println("Right");
// 				}          
// 				break;
// 			case GES_LEFT_FLAG: 
// 				delay(GES_ENTRY_TIME);
// 				paj7620ReadReg(0x43, 1, &data);
// 				if(data == GES_FORWARD_FLAG) 
// 				{
// 					Serial.println("Forward");
// 					delay(GES_QUIT_TIME);
// 				}
// 				else if(data == GES_BACKWARD_FLAG) 
// 				{
// 					Serial.println("Backward");
// 					delay(GES_QUIT_TIME);
// 				}
// 				else
// 				{
// 					Serial.println("Left");
// 				}          
// 				break;
// 			case GES_UP_FLAG:
// 				delay(GES_ENTRY_TIME);
// 				paj7620ReadReg(0x43, 1, &data);
// 				if(data == GES_FORWARD_FLAG) 
// 				{
// 					Serial.println("Forward");
// 					delay(GES_QUIT_TIME);
// 				}
// 				else if(data == GES_BACKWARD_FLAG) 
// 				{
// 					Serial.println("Backward");
// 					delay(GES_QUIT_TIME);
// 				}
// 				else
// 				{
// 					Serial.println("Up");
// 				}          
// 				break;
// 			case GES_DOWN_FLAG:
// 				delay(GES_ENTRY_TIME);
// 				paj7620ReadReg(0x43, 1, &data);
// 				if(data == GES_FORWARD_FLAG) 
// 				{
// 					Serial.println("Forward");
// 					delay(GES_QUIT_TIME);
// 				}
// 				else if(data == GES_BACKWARD_FLAG) 
// 				{
// 					Serial.println("Backward");
// 					delay(GES_QUIT_TIME);
// 				}
// 				else
// 				{
// 					Serial.println("Down");
// 				}          
// 				break;
// 			case GES_FORWARD_FLAG:
// 				Serial.println("Forward");
// 				delay(GES_QUIT_TIME);
// 				break;
// 			case GES_BACKWARD_FLAG:		  
// 				Serial.println("Backward");
// 				delay(GES_QUIT_TIME);
// 				break;
// 			case GES_CLOCKWISE_FLAG:
// 				Serial.println("Clockwise");
// 				break;
// 			case GES_COUNT_CLOCKWISE_FLAG:
// 				Serial.println("anti-clockwise");
// 				break;  
// 			default:
// 				paj7620ReadReg(0x44, 1, &data1);
// 				if (data1 == GES_WAVE_FLAG) 
// 				{
// 					Serial.println("wave");
// 				}
// 				break;
// 		}
// 	}
// 	delay(100);
//  // Check if a proximity reading is available.
//   if (APDS.proximityAvailable()) {
//     proximity = APDS.readProximity();
//   }

//   // Check if a gesture reading is available
//   if (APDS.gestureAvailable()) {
//     int gesture = APDS.readGesture();
//     switch (gesture) {
//       case GESTURE_UP:
//         Serial.println("Detected UP gesture");
//         break;

//       case GESTURE_DOWN:
//         Serial.println("Detected DOWN gesture");
//         break;

//       case GESTURE_LEFT:
//         Serial.println("Detected LEFT gesture");
//         break;

//       case GESTURE_RIGHT:
//         Serial.println("Detected RIGHT gesture");
//         break;

//       default:
//         // Ignore
//         break;
//     }
//   }

//   // Check if a color reading is available
//   if (APDS.colorAvailable()) {
//     APDS.readColor(r, g, b);
//   }

//   // Print updates every 100 ms
//   if (millis() - lastUpdate > 100) {
//     lastUpdate = millis();
//     Serial.print("PR=");
//     Serial.print(proximity);
//     Serial.print(" RGB=");
//     Serial.print(r);
//     Serial.print(",");
//     Serial.print(g);
//     Serial.print(",");
//     Serial.println(b);
//   }
}