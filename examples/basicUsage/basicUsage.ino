#include "SerialBLE.h"

SerialBLEClass SerialBLE;

void setup() {
    Serial.begin(115200);
    SerialBLE.begin("UART_BLE_Device");
}

void loop() {
    if(SerialBLE.available()) {
        String receivedString = SerialBLE.readString();
        SerialBLE.printf("I received \"%s\"", receivedString.c_str());
    }
}