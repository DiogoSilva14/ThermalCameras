#include <Arduino.h>
#include "SPIFFS.h"
#include <WiFi.h>

const char* ssid = "Lepton IR";
const char* password = "123123123";

WiFiServer server(80);

void writeBMPFile(WiFiClient client);

void setup() {
    Serial.begin(115200);

    WiFi.softAP(ssid, password);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    server.begin();

    if(!SPIFFS.begin(true)){
        Serial.println("An Error has occurred while mounting SPIFFS");
    }
}

void loop(){
    WiFiClient client = server.available();

    if(client){
        writeBMPFile(client);
    }
}

void writeBMPFile(WiFiClient client){
    File bmp = SPIFFS.open("/Untitled.bmp");
    if(!bmp){
        Serial.println("Failed to open file");
        return;
    }

    String filearray = "";

    while(bmp.available()){
        char byte_1 = bmp.read();
        filearray += byte_1;
    }

    client.print(filearray);

    bmp.close();
}