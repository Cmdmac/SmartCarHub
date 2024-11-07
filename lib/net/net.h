#ifndef _NET_H_
#include <HTTPClient.h>
#include <ArduinoWebsockets.h>
#include <string>
using namespace std;
using namespace websockets;


class Net {
    public:
        static string httpGet(string url);
        void setUpWifi();
        void setUpWebsocket();
        WebsocketsClient& ws();
        void loop();
    private:
        void onMessageCallback(WebsocketsMessage message);
        void onEventsCallback(WebsocketsEvent event, String data);
        WebsocketsClient client;
};

#endif