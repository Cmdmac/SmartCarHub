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
#include "code.h"

using namespace std;
using namespace websockets;

iBeaconFinder finder;
extern WebsocketsClient client;


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
  ::httpGet(ss.str());
}

void compassTask(void* params) {
  // Serial.begin(9600);
  // Serial.println("test");
  QMC5883LCompass compass;
  compass.init(17, 18);
  int a;
  while(1) {
    // compassTask(NULL);

    // Read compass values
    compass.read();
    // Return Azimuth reading
    a = compass.getAzimuth() + 180;
    string s = CodeBuilder::CreateCodeJson(DIRECTION, a);
    // Serial.println(s.c_str());
    client.send(s.c_str());
    Serial.print("A: ");
    Serial.print(a);
    Serial.println();
    delay(1000);
  }
}



Ticker beaconTimer(scanIBeacons, 1000, 0, MILLIS);

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
void setup() {
  Serial.begin(9600);
  
  // create compass task
  xTaskCreatePinnedToCore(compassTask, "CompassTask", 4096, NULL, 1, NULL, 0);
  // xTaskCreatePinnedToCore(audioTask, "AudioTask", 8192 * 2, NULL, 1, NULL, 0);


  // init ble,wifi,websocket
  finder.init();
  ::setUpWifi();
  ::setUpWebsocket();
  // start timeer
  beaconTimer.start();

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

}

void loop() {
  client.poll();
  beaconTimer.update(); 
  audio.loop();
  // delay(15000);
}