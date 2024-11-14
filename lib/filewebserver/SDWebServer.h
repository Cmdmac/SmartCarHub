#ifndef _SDWEBSERVER_H_
#include <WebServer.h>
#include <ESPmDNS.h>
#include <SPI.h>
#include <SD.h>
#include "FileWebServer.h"


class SDWebServer : public FileWebServer
{
private:
    /* data */


    File open(const char* path, const char* mode = FILE_READ) override {
        return SD.open(path, mode);
    }

    bool fileExists(const char* path) override {
        return SD.exists(path);
    }

    bool checkAndRemove(const char* path) override {
        if (SD.exists(path)) {
            return SD.remove(path);
        }
        return false;
    }

    bool remove(const char* path) override {
        return SD.remove(path);
    }

    bool rmdir(const char* path) override {
        return SD.rmdir(path);
    }

    bool mkdir(const char* path) override {
        return SD.mkdir(path);
    }    

public:
    // SDWebServer(/* args */);
    // ~SDWebServer();


};

// SDWebServer::SDWebServer(/* args */)
// {
// }

// SDWebServer::~SDWebServer()
// {
// }

#endif