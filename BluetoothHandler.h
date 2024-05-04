#ifndef INCLUDE_BLUETOOTHHANDLER_H_
#define INCLUDE_BLUETOOTHHANDLER_H_

#include <condition_variable>
#include <sdbus-c++/sdbus-c++.h>

using devicelist_t = std::map<sdbus::ObjectPath, std::map<std::string, std::map<std::string, sdbus::Variant>>>;

class BluetoothHandler {
    private:
        std::unique_ptr<sdbus::IProxy> bluezProxy;
        std::unique_ptr<sdbus::IProxy> interfaceProxy;
        std::mutex mtx;
        std::mutex mtx2;
        std::condition_variable cv;
        std::condition_variable cv2;
        bool connected;
        bool bleConnected;
        bool bleSpeakerFound;
        std::mutex bleSpeakerMtx;
        std::condition_variable bleSpeakerCV;
        std::string speakerDeviceBluezPath;
        void findPowerOnCharacteristicAndWriteMagicString();
        
    public:
        BluetoothHandler();

        bool getBluetoothStatus();
        devicelist_t getDevices();

        void startScanning();
        void stopScanning();

        void powerOnUEBoom();
        bool checkConnected();

        void setSpeakerPath(const std::string speakerPath);
        std::string getAdapterAddress();
};


#endif