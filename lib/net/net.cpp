#include "net.h"
#include <ArduinoWebsockets.h>

using namespace websockets;

string httpGet(string url) {
    HTTPClient http;
    http.begin(url.c_str());  // 替换为您要请求的 URL

    int httpCode = http.GET();

    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println("Response:");
        Serial.println(payload);
        return payload.c_str();
      }
    } else {
      Serial.printf("HTTP request failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
    return string();
}

WebsocketsClient client;
const char* ssid     = "Stark";  // 替换为您的 Wi-Fi 网络名称
const char* password = "fengzhiping,1101";  // 替换为您的 Wi-Fi 密码
const char* websockets_url = "ws://192.168.2.153:3000/mobile/hub"; //Enter server adress


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

void setUpWifi() {
  WiFi.begin(ssid, password);

  while (WiFi.status()!= WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void setUpWebsocket() {
  Serial.begin(9600);
  Serial.println("setup websocket");
  client.onMessage(onMessageCallback);
  client.onEvent(onEventsCallback);
  client.connect(websockets_url);

}