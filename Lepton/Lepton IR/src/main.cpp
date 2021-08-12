#include <Arduino.h>
#include "BluetoothSerial.h"
#include "SPIFFS.h"

BluetoothSerial SerialBT;

void writeBMPFile();

void setup() {
    Serial.begin(115200);
    SerialBT.begin("Lepton IR");

    if(!SPIFFS.begin(true)){
        Serial.println("An Error has occurred while mounting SPIFFS");
    }
}

void loop(){
    while(!SerialBT.hasClient());
    if(SerialBT.hasClient()){
        writeBMPFile();
    }
    delay(100000);
}

void writeBMPFile(){
    //SerialBT.print('<');

    File bmp = SPIFFS.open("/blackbuck.bmp");
    if(!bmp){
        Serial.println("Failed to open file");
        return;
    }

    while(bmp.available()){
        char byte_1 = bmp.read();
        SerialBT.write(byte_1);
        Serial.print(byte_1);
    }

    //SerialBT.print('>');

    bmp.close();
    SerialBT.end();
}