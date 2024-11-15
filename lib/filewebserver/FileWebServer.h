#ifndef _FILE_WEBSERVER_H_
#define _FILE_WEBSERVER_H_

#include <string>
#include <WebServer.h>
#include <SPI.h>
#include <FS.h>
#include <ESPmDNS.h>

#define FILEWEBSERVER_PORT 88
#define HOST "esp32sd"

class FileWebServer {
    protected:
        /* data */
        WebServer server;
        std::string spiffsIndexFileStr;

        virtual size_t getFsTotalBytes() = 0;
        virtual size_t getFsUsedBytes() = 0;

        void returnOK();
        void returnFail(String msg);
        void return404();
        void handleFileUpload();
        void deleteRecursive(String path);
        void handleDelete();
        void handleCreate();
        void printDirectory();
        void handleIndex();
        void handleNotFound();
        void handleCreateFolder();
        void handleDownloadFile();
        bool loadFile(const char* path);



        virtual File open(const char* path, const char* mode = FILE_READ) = 0;
        virtual bool fileExists(const char* path) = 0;
        virtual bool checkAndRemove(const char* path) = 0;
        virtual bool remove(const char* path) = 0;
        virtual bool rmdir(const char* path) = 0;
        virtual bool mkdir(const char* path) = 0;

    public:
        void setup();
        void loop();

};

#endif