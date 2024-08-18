#include <arduino.h>
#include <Ticker.h>
#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include <string>
#include "iBeacon.h"
#include <HTTPClient.h>
#include "QMC5883LCompass.h"
const char* ssid     = "Stark";  // 替换为您的 Wi-Fi 网络名称
const char* password = "fengzhiping,1101";  // 替换为您的 Wi-Fi 密码
const char* websockets_url = "ws://192.168.1.4:3000/mobile"; //Enter server adress

using namespace std;
using namespace websockets;

void onMessageCallback(WebsocketsMessage message) {
    Serial.print("Got Message: ");
    Serial.println(message.data());
}

void onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened");
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("Connnection Closed");
    } else if(event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
    } else if(event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
    }
}


WebsocketsClient client;
QMC5883LCompass compass;

iBeaconFinder finder;
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

    HTTPClient http;

    std::stringstream ss;
    ss << "http://192.168.1.4:3000/locate?"  << "mac1=" << mac1 << "&mac2=" << mac2 << "&mac3=" << mac3 << "&rssi1=" << rssi1 << "&rssi2=" << rssi2 << "&rssi3=" << rssi3;

    Serial.println(ss.str().c_str());

    http.begin(ss.str().c_str());  // 替换为您要请求的 URL

    int httpCode = http.GET();

    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println("Response:");
        Serial.println(payload);
      }
    } else {
      Serial.printf("HTTP request failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

}

Ticker ticker1(scanIBeacons, 1000, 0, MILLIS);

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);

  while (WiFi.status()!= WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  // run callback when messages are received
  client.onMessage(onMessageCallback);
  // run callback when events are occuring
  client.onEvent(onEventsCallback);
  // Connect to server
  client.connect(websockets_url);

  finder.init();
  // 设置定时器
  // ticker1.start();

  compass.init(17, 18);

}

void loop() {
    client.poll();
    // ticker1.update();

    int a;
  
    // Read compass values
    compass.read();

    // Return Azimuth reading
    a = compass.getAzimuth();
    
    Serial.print("A: ");
    Serial.print(a);
    Serial.println();
    
}