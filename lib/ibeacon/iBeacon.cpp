#include "iBeacon.h"
#include <Arduino.h>
#include "net.h"

BleCallback::BleCallback() : len(0) {
      
}

void BleCallback::onResult(BLEAdvertisedDevice adv) {
    // Serial.println("设备名称: " + adv.getName() + "MAC 地址: " + adv.getAddress().toString() + "RSSI: " + new String(adv.getRSSI()));
    const char*name = adv.getName().c_str();
    // Serial.println(adv.getName().c_str());
    if (strcmp(name, "Holy-IOT") == 0) {
        // devices[len++] = beacon;
        string address = adv.getAddress().toString();
        bool isFound = false;
        for (std::vector<iBeacon>::iterator it = devices.begin(); it!= devices.end(); ++it) {
            if (it->address.compare(address) == 0) {
                it->rssi = adv.getRSSI();
                // Serial.print("found:");
                // Serial.print(adv.getAddress().toString().c_str());
                // Serial.println(adv.getRSSI());
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

void iBeaconFinder::reportTask() {
    // Serial.println("scanIBeacons");
    find();
    vector<iBeacon> devices = getDevices();
    std::stringstream ss;
    ss << "http://192.168.2.153:3000/locate?";
    for (int i = 0; i < devices.size(); i++) {
        ss << "mac" << i + 1 << "=" << devices[i].address << "&rssi" << i + 1 << "=" << devices[i].rssi << "&";
    }
    // Serial.println(ss.str().c_str());
    Net::httpGet(ss.str());
}

void iBeaconFinder::findAndReportToServer() {
    // beaconTimer.attach_ms(1000, &iBeaconFinder::searchBeacons, this);
    // xTaskCreatePinnedToCore(&iBeaconFinder::searchBeacons, "SearchBeaconsTask", 4096, this, 1, NULL, 0);
// (scanIBeacons, 1000, 0, MILLIS)
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