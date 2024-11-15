#ifndef _SPIFFS_SERVER_H_
#define _SPIFFS_SERVER_H_
#include <SPIFFS.h>
#include <FS.h>
#include "FileWebServer.h"

class SPIFFSServer : public FileWebServer
{
private:
    /* data */


    size_t getFsTotalBytes() override {
        return SPIFFS.totalBytes();
    }

    size_t getFsUsedBytes() override {
        return SPIFFS.usedBytes();
    }

    File open(const char* path, const char* mode = FILE_READ) override {
        return SPIFFS.open(path, mode);
    }

    bool fileExists(const char* path) override {
        return SPIFFS.exists(path);
    }

    bool checkAndRemove(const char* path) override {
        if (SPIFFS.exists(path)) {
            return SPIFFS.remove(path);
        }
        return false;
    }

    bool remove(const char* path) override {
        return SPIFFS.remove(path);
    }

    bool rmdir(const char* path) override {
        return SPIFFS.rmdir(path);
    }

    bool mkdir(const char* path) override {
        return SPIFFS.mkdir(path);
    }    
public:

};


#endif