#ifndef _IBEACON_H
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

class iBeacon {
  public:
    string name;
    string address;
    int rssi;
    string toString() {
      std::stringstream ss;
    
      ss << "name=" << name.c_str() << ",addr=" << address.c_str() << ",rssi=" << rssi;
      return ss.str();
    }
};

class BleCallback : public BLEAdvertisedDeviceCallbacks {
  private:
    vector<iBeacon> devices;
    int len;

  public:
    BleCallback();
    void onResult(BLEAdvertisedDevice adv);
    vector<iBeacon> getIbeacons();
};

class iBeaconFinder {
    private:
        BLEScan* pBLEScan;
        BleCallback* pBleCallback;
        
        static bool compareiBeacon(const iBeacon& p1, const iBeacon& p2) {
            return p1.rssi < p2.rssi;
        }
    public:
        iBeaconFinder();
        void init();
        void find();
        vector<iBeacon> getDevices();
};
#endif