#ifndef SERIAL_BLE_H
#define SERIAL_BLE_H
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>


class MyBLEServerCallbacks : public BLEServerCallbacks {
public:
    bool deviceConnected;
    
    void onConnect(BLEServer* pServer_) {
        deviceConnected = true;
    }

    void onDisconnect(BLEServer* pServer_) {
        deviceConnected = false;
    }
};

class RxCallbacks : public BLECharacteristicCallbacks {
public:
    void onWrite(BLECharacteristic *pCharacteristic) {
        lastData = pCharacteristic->getValue();
    }

    int available() {return lastData.length();}
    String getLastData() {
        String out = lastData.c_str();
        lastData = "";
        return out;
    }
private:
    std::string lastData = "";
};

class SerialBLEClass {
public:
    void begin(String deviceName="BLE_UART");

    void printf(const char* format, ...);
    void writeString(String msg);

    int available();
    String readString();
    bool isConnected();
private:
    BLEServer* server = NULL;
    MyBLEServerCallbacks serverCallbacks;
    BLEService* service;
    BLECharacteristic* rxCharacteristic = NULL;
    BLECharacteristic* txCharacteristic = NULL;
    bool deviceConnected = false; 
    RxCallbacks *rxCallbacks; 
};


#endif
