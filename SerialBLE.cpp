#include "SerialBLE.h"

void SerialBLEClass::begin(String deviceName) {
    BLEDevice::init(deviceName.c_str());
    server = BLEDevice::createServer();
    serverCallbacks = MyBLEServerCallbacks();
    server->setCallbacks(&serverCallbacks);
    service = server->createService(BLEUUID("6E400001-B5A3-F393-E0A9-E50E24DCCA9E"));
    rxCharacteristic = service->createCharacteristic(
        BLEUUID("6E400002-B5A3-F393-E0A9-E50E24DCCA9E"),
        BLECharacteristic::PROPERTY_WRITE
    );

    txCharacteristic = service->createCharacteristic(
        BLEUUID("6E400003-B5A3-F393-E0A9-E50E24DCCA9E"),
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
    );
    txCharacteristic->addDescriptor(new BLE2902());
    rxCharacteristic->addDescriptor(new BLE2902());

    rxCallbacks = new RxCallbacks();
    rxCharacteristic->setCallbacks(rxCallbacks);

    service->start();
    server->getAdvertising()->start();
}

bool SerialBLEClass::isConnected() {
    return serverCallbacks.deviceConnected;
}

void SerialBLEClass::printf(const char* format, ...) {
    char buffer[100];
    va_list args;
    va_start(args, format);
    int nChar = std::vsprintf(buffer, format, args);
    va_end(args);
    txCharacteristic->setValue(buffer);
    txCharacteristic->notify();
    // for(int i = 0; i < nChar; i++) {
    //     Serial.printf("%d\t", (int)buffer[i]);
    // }
    // Serial.println();
}

int SerialBLEClass::available() {
    return rxCallbacks->available();
}

String SerialBLEClass::readLastData() {
    return rxCallbacks->getLastData();
}