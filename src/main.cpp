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
using namespace std;
using namespace websockets;

iBeaconFinder finder;
extern WebsocketsClient client;

void scanIBeacons() {
  Serial.println("scanIBeacons");
  finder.find();
  vector<iBeacon> devices = finder.getDevices();
  if (devices.size() >= 3) {
    string mac1 = devices[0].address;
    string mac2 = devices[1].address;
    string mac3 = devices[2].address;

    int rssi1 = devices[0].rssi;
    int rssi2 = devices[1].rssi;
    int rssi3 = devices[2].rssi;

    
    std::stringstream ss;
    ss << "http://192.168.1.4:3000/locate?"  << "mac1=" << mac1 << "&mac2=" << mac2 << "&mac3=" << mac3 << "&rssi1=" << rssi1 << "&rssi2=" << rssi2 << "&rssi3=" << rssi3;

    Serial.println(ss.str().c_str());

    ::httpGet(ss.str());
  }

}

void compassTask(void* params) {
  QMC5883LCompass compass;
  compass.init(17, 18);
  // while(1) {
    int a;
    // Read compass values
    compass.read();
    // Return Azimuth reading
    a = compass.getAzimuth();
    Serial.print("A: ");
    Serial.print(a);
    Serial.println();
  // }
}

  Ticker ticker1(scanIBeacons, 1000, 0, MILLIS);

void setup() {
  // 设置定时器
  ticker1.start();

  ::setUpWebsocket();
  finder.init();


}

void loop() {
    client.poll();
    ticker1.update();
    compassTask(NULL);
}