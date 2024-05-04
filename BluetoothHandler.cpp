#include "BluetoothHandler.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <regex>

static const std::string SERVICE_BLUEZ{"org.bluez"};
static const std::string INTERFACE_ADAPTER { "org.bluez.Adapter1" };
static const std::string MEMBER_PROP_CHANGED { "PropertiesChanged" };
static const std::string MEMBER_IFACE_ADDED {"InterfacesAdded"};
static const std::string INTERFACE_OBJ_MGR {"org.freedesktop.DBus.ObjectManager"};
static const std::string INTERFACE_PROP_MGR { "org.freedesktop.DBus.Properties" };
static const std::string INTERFACE_PROPERTIES { "org.freedesktop.DBus.Properties" };
static const std::string INTERFACE_DEVICE {"org.bluez.Device1"};
static const std::string METHOD_CONNECT{"Connect"};
static const std::string METHOD_PROFILE_CONNECT{"ConnectProfile"};
static const std::string METHOD_GET { "Get" };
static const std::string INTERFACE_CHAR{"org.bluez.GattCharacteristic1"};
static const std::string GATT_PROFILE_GENERIC_ACCESS {"00001800-0000-1000-8000-00805f9b34fb"};

void propertyChanged (const std::string& interface_name, const std::map<std::string, sdbus::Variant>& changed_properties, const std::vector<std::string>& invalidated_properties)
{
        std::cout << "property changed: " << interface_name << std::endl;
        std::cout << "changed properties:"<< std::endl;
        for (auto const& [key, val] : changed_properties) {
            std::cout << " --- " << key << " : " << val.peekValueType() << std::endl;
        }
        std::cout << "invalidated properties:"<< std::endl;
        for (auto const& key : invalidated_properties) {
            std::cout << " --- " << key << std::endl;
        }
        // auto RSSI = changed_properties.at("RSSI").get<int16_t>();
        // std::cout << "RSSI: " << RSSI << std::endl;
};

void interfaceChange (sdbus::ObjectPath path, std::map<std::string, std::map<std::string, sdbus::Variant>> dictionary) {
    std::cout << "interface change path: " << path << std::endl;    
    // for (auto const& [key, val] : dictionary) {
    //         std::cout << " --- " << key << " : " << std::endl;
    //         for (auto const& [key2, val2] : val) {
    //             std::cout << " --- " << key2 << " : " << val2.peekValueType() << std::endl;
    //         }
    // }
}

BluetoothHandler::BluetoothHandler() : mtx{}, connected{false}, bleSpeakerMtx{}, bleSpeakerFound{false}, bleConnected{false}
{
    bluezProxy = sdbus::createProxy(SERVICE_BLUEZ, "/");
    interfaceProxy = sdbus::createProxy(SERVICE_BLUEZ, "/org/bluez/hci0");
    bluezProxy->uponSignal(MEMBER_IFACE_ADDED).onInterface(INTERFACE_OBJ_MGR).call(interfaceChange);
    bluezProxy->finishRegistration();
}

bool BluetoothHandler::getBluetoothStatus()
{
    const std::string METHOD_GET { "Get" };
    const std::string INTERFACE_PROPERTIES { "org.freedesktop.DBus.Properties" };
    const std::string PROPERTY_POWERED { "Powered" };
    sdbus::Variant variant;

    interfaceProxy->callMethod(METHOD_GET)
        .onInterface(INTERFACE_PROPERTIES)
        .withArguments(INTERFACE_ADAPTER, PROPERTY_POWERED)
        .storeResultsTo(variant);

    return (bool)variant;
}

devicelist_t BluetoothHandler::getDevices() {
    const std::string OBJECT_MANAGER { "org.freedesktop.DBus.ObjectManager" };
    const std::string METHOD_GET_MANAGED_OBJECTS { "GetManagedObjects" };

    std::map<sdbus::ObjectPath, std::map<std::string, std::map<std::string, sdbus::Variant>>> managedObjects;
    bluezProxy->callMethod(METHOD_GET_MANAGED_OBJECTS)
        .onInterface(OBJECT_MANAGER)
        .storeResultsTo(managedObjects);

    return managedObjects;
}

void BluetoothHandler::startScanning()  {
    std::cout << "BluetoothHandler: start scanning" << std::endl;
    const std::string METHOD_START_DISCOVERY { "StartDiscovery" };
    interfaceProxy->callMethod(METHOD_START_DISCOVERY)
        .onInterface(INTERFACE_ADAPTER)
        .dontExpectReply();
}

void BluetoothHandler::stopScanning()  {
    std::cout << "BluetoothHandler: stop scanning" << std::endl;
    const std::string METHOD_STOP_DISCOVERY { "StopDiscovery" };
    interfaceProxy->callMethod(METHOD_STOP_DISCOVERY)
        .onInterface(INTERFACE_ADAPTER)
        .dontExpectReply();
}

std::vector<uint8_t> HexToBytes(const std::string& hex) {
 std::vector<uint8_t> bytes;

 for (unsigned int i = 0; i < hex.length(); i += 3) {
   std::string byteString = hex.substr(i, 2);
   uint8_t byte = static_cast<uint8_t>(std::stoi(byteString.c_str(), nullptr, 16));
   bytes.push_back(byte);
 }

 return bytes;
}

void BluetoothHandler::findPowerOnCharacteristicAndWriteMagicString() {    
    const std::regex DEVICE_ATTRS_RE{"^/org/bluez/hci\\d/dev(_[0-9A-F]{2}){6}/service\\d{4}/char\\d{4}"};
    std::string interfaceMAC = getAdapterAddress();

    auto devices = getDevices();
    for (const auto device : devices) {
        const sdbus::ObjectPath& path = device.first;
        std::smatch match;
        if (std::regex_match(path, match, DEVICE_ATTRS_RE)) {
            const auto data = device.second;
            if((data.count(INTERFACE_CHAR) == 1) && 
                    (data.at(INTERFACE_CHAR).count("UUID") == 1)) {
                
                const std::string uuid = (std::string) data.at(INTERFACE_CHAR).at("UUID");                
                
                if (uuid == "c6d6dc0d-07f5-47ef-9b59-630622b01fd3") {
                    std::unique_ptr<sdbus::IProxy> tempAttrProxy = sdbus::createProxy(SERVICE_BLUEZ, path);
                    const std::string METHOD_WRITE{"WriteValue"};
                    std::vector<uint8_t> wakeup = HexToBytes(interfaceMAC);
                    wakeup.push_back(1);
                    std::map<std::string, sdbus::Variant> options{};
                    tempAttrProxy->callMethod(METHOD_WRITE)
                        .onInterface(INTERFACE_CHAR)
                        .withArguments(wakeup, options);
                    std::cout << "Wakeup.... ";
                }

            }    
        }
    }
}

void BluetoothHandler::powerOnUEBoom() {
    if(speakerDeviceBluezPath.empty()) {
        return;
    }
    bool connectionStatus = checkConnected();
    std::cout << "connection status: " << connectionStatus << std::endl;
    if(connectionStatus) {
        return;
    }
    connected = false;
    bleConnected = false;
    bleSpeakerFound = false;
    const std::regex DEVICE_INSTANCE_RE{"^/org/bluez/hci[0-9]/dev(_[0-9A-F]{2}){6}$"};
        
    auto devices = getDevices();
    std::vector<std::unique_ptr<sdbus::IProxy>> list;
    for (const auto device : devices) {
        const sdbus::ObjectPath& path = device.first;
        std::smatch match;
        if (!std::regex_match(path, match, DEVICE_INSTANCE_RE)) {
            continue;
        }
        
        std::unique_ptr<sdbus::IProxy> deviceProxy = sdbus::createProxy(SERVICE_BLUEZ, path);
        deviceProxy->uponSignal(MEMBER_PROP_CHANGED).onInterface(INTERFACE_PROP_MGR).call([path, this](const std::string& interface_name, const std::map<std::string, sdbus::Variant>& changed_properties, const std::vector<std::string>& invalidated_properties)  {
            std::cout << "property changed: " << path << " (interface name:" << interface_name << ")" << std::endl;
            if(path == speakerDeviceBluezPath) {
                for (auto const& [key, val] : changed_properties) {
                    if(key == "RSSI") {
                        std::unique_lock<std::mutex> lock(bleSpeakerMtx);
                        std::cout << "Found!!!" << std::endl;
                        bleSpeakerFound = true;
                        lock.unlock();
                        bleSpeakerCV.notify_one();
                    } else if (key == "ServicesResolved") {
                        std::unique_lock<std::mutex> lock(mtx);
                        bleConnected = true;
                        lock.unlock();
                        cv.notify_one();
                    } else {
                        std::cout << "key: " << key << std::endl;
                    }
                }
            }
        });
        deviceProxy->finishRegistration();
        list.push_back(std::move(deviceProxy));        
    }
    
    std::cout << "powerOnUEBoom" << std::endl;
    startScanning();
    std::unique_lock<std::mutex> lock(bleSpeakerMtx);
    auto now = std::chrono::system_clock::now();
    bleSpeakerCV.wait_for(lock, std::chrono::seconds(1), [this]()
                 { return bleSpeakerFound; });
   
    std::unique_ptr<sdbus::IProxy> deviceProxy = sdbus::createProxy(SERVICE_BLUEZ, speakerDeviceBluezPath);
    if(bleSpeakerFound) {
        std::cout << "FOUND UE BOOM via BLE" << std::endl;        
        
        deviceProxy->callMethodAsync(METHOD_PROFILE_CONNECT).onInterface(INTERFACE_DEVICE).withArguments(GATT_PROFILE_GENERIC_ACCESS).uponReplyInvoke([this](const sdbus::Error *error){});

        cv.wait(lock, [this]() { return bleConnected; });

        stopScanning();

        findPowerOnCharacteristicAndWriteMagicString();

        // wait for power up    
        std::this_thread::sleep_for(std::chrono::seconds(5));

    } else {
        stopScanning();
    }    

    auto connectionCallback = [this](const sdbus::Error *error)
    {
        std::cout << "connection callback" << std::endl;
        if (error != nullptr)
        {
            // Got connection error org.bluez.Error.Failed with message Protocol not available
            std::cerr << "Got connection error "
                    << error->getName() << " with message "
                    << error->getMessage() << std::endl;
            return;
        }
        std::cout << "connected" << std::endl;
        std::unique_lock<std::mutex> lock(mtx2);
        connected = true;
        bleConnected = false;
        lock.unlock();
        cv2.notify_one();
    };

    deviceProxy->callMethodAsync(METHOD_CONNECT).onInterface(INTERFACE_DEVICE).uponReplyInvoke(connectionCallback);
    std::unique_lock<std::mutex> lock2(mtx2);
    cv2.wait_for(lock2, std::chrono::seconds(4),[this]()
                { return connected; });
    std::cout << "done" << std::endl;
}

bool BluetoothHandler::checkConnected() {    
    const std::string PROPERTY_CONNECTED { "Connected" };
    sdbus::Variant variant;

    try {
        std::unique_ptr<sdbus::IProxy> deviceProxy = sdbus::createProxy(SERVICE_BLUEZ, speakerDeviceBluezPath);

        deviceProxy->callMethod(METHOD_GET)
            .onInterface(INTERFACE_PROPERTIES)
            .withArguments(INTERFACE_DEVICE, PROPERTY_CONNECTED)
            .storeResultsTo(variant);

        connected = (bool)variant;
        return connected;
    } catch (const sdbus::Error& e) {
    }
    connected = false;
    return  connected;
}

void BluetoothHandler::setSpeakerPath(const std::string speakerPath) {
    this->speakerDeviceBluezPath = speakerPath;
}

std::string BluetoothHandler::getAdapterAddress() {
    
    sdbus::Variant variant;
    interfaceProxy->callMethod(METHOD_GET)
            .onInterface(INTERFACE_PROPERTIES)
            .withArguments(INTERFACE_ADAPTER, "Address")
            .storeResultsTo(variant);
    return (std::string) variant;
}
