#ifndef _IBEACON_H
#define _IBEACON_H
#include "esp_ibeacon_api.h"

typedef struct {
    int major;
    int minor;
    esp_bd_addr_t* addr;
    int rssi;
}__attribute__((packed)) iBeacon;

typedef void (*FuniBeaconScanCallback)(iBeacon*, int);
void startScanIbeacon(FuniBeaconScanCallback callback);

#endif