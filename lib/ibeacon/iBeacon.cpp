#include "iBeacon.h"
#include <Arduino.h>

BleCallback::BleCallback() : len(0) {
      
}

void BleCallback::onResult(BLEAdvertisedDevice adv) {
    // Serial.println("设备名称: " + adv.getName() + "MAC 地址: " + adv.getAddress().toString() + "RSSI: " + adv.getRSSI());
    const char*name = adv.getName().c_str();
    // Serial.println(adv.getName().c_str());
    if (strcmp(name, "Holy-IOT") == 0) {
        // devices[len++] = beacon;
        string address = adv.getAddress().toString();
        bool isFound = false;
        for (std::vector<iBeacon>::iterator it = devices.begin(); it!= devices.end(); ++it) {
            if (it->address.compare(address) == 0) {
                it->rssi - adv.getRSSI();
                isFound = true;
                break;
            }
        }
        if (!isFound) {
            iBeacon beacon;
            beacon.name = adv.getName();
            beacon.address = adv.getAddress().toString();
            beacon.rssi = adv.getRSSI();
            devices.push_back(beacon);
            Serial.print("add beacon ");
            Serial.println(beacon.toString().c_str());
        }
    }
}

vector<iBeacon> BleCallback::getIbeacons() {
    return devices;
}

iBeaconFinder::iBeaconFinder():pBleCallback(NULL) {

}

void iBeaconFinder::init() {
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan();
}

void iBeaconFinder::find() {
    pBLEScan->clearResults();
    if (pBleCallback == NULL) {
        pBleCallback = new BleCallback();
    }
    pBLEScan->setAdvertisedDeviceCallbacks(pBleCallback);
    pBLEScan->setActiveScan(true);
    pBLEScan->start(8, false);  
}

vector<iBeacon> iBeaconFinder::getDevices() {
    if (pBleCallback != NULL) {
        vector<iBeacon> devices = pBleCallback->getIbeacons();
        std::sort(devices.begin(), devices.end(), iBeaconFinder::compareiBeacon);
        return devices;
    } else {
        vector<iBeacon> devices;
        return devices;
    }
}