#ifndef _NET_H_
#include <HTTPClient.h>
#include <ArduinoWebsockets.h>
#include <string>
using namespace std;
using namespace websockets;

#define WS_SERVER "http://192.168.2.153:3000"

typedef std::function<void(int)> CommandCallback;
class Net {
    public:
        static string httpGet(string url);
        void setUpWifi();
        void setUpWebsocket(CommandCallback callback);
        WebsocketsClient& ws();
        void loop();

    private:
        void onMessageCallback(WebsocketsMessage message);
        void onEventsCallback(WebsocketsEvent event, String data);
        WebsocketsClient client;
        CommandCallback commandCallback;
};

#endif