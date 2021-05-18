#include <Arduino.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup() {
  SerialBT.begin("ESP32IR");
  Serial.begin(115200);
}

void loop() {
  if(SerialBT.available()){
    SerialBT.write(SerialBT.read());
  }
}