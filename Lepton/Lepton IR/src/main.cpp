#include <Arduino.h>
#include "LeptonFLiR.h"
#include "BluetoothSerial.h"

LeptonFLiR flirController(Wire,5);            // Library using default Wire and default chip select pin D53
BluetoothSerial SerialBT;

void writeBMPFile(byte *imageData, int width, int height, int pitch);

void setup() {
    Serial.begin(115200);
    SerialBT.begin("Lepton IR");

    Wire.begin();                       // Wire must be started first
    Wire.setClock(400000);              // Supported baud rates are 100kHz, 400kHz, and 1000kHz
    SPI.begin();                        // SPI must be started first as well

    // Using 40x30 8bpp memory allocation mode and default celsius temperature mode
    flirController.init(LeptonFLiR_ImageStorageMode_40x30_8bpp);

    flirController.agc_setAGCEnabled(DISABLED);

    flirController.sys_setTelemetryEnabled(DISABLED); // Ensure telemetry is enabled
}

uint32_t lastFrameNumber = -1;

void loop() {
    if (flirController.readNextFrame()) { // Read next frame and store result into internal imageData
        Serial.println("Next frame");
        //uint32_t frameNumber = flirController.getTelemetryFrameCounter();

        //if (frameNumber > lastFrameNumber) { // Frame counter increments every 3rd frame due to export restrictions
            //lastFrameNumber = frameNumber;

            //char fileName[] = "FLIR/IMG0000.BMP";
            //uint16_t fileNumber = (uint16_t)(frameNumber / 3);
            //wordsToHexString((uint16_t *)&fileNumber, 1, &fileName[8], 4);

            //File bmpFile = SD.open(fileName, FILE_WRITE);

            //if (bmpFile) {
                if(SerialBT.hasClient()){
                writeBMPFile(flirController.getImageData(),
                             flirController.getImageWidth(),
                             flirController.getImageHeight(),
                             flirController.getImagePitch());
                Serial.println("Writing file");
                }

                //bmpFile.close();

                //Serial.print(fileName);
                Serial.println(" written...");
        //}

        // Occasionally flat field correction normalization needs ran
        if (flirController.getShouldRunFFCNormalization())
            flirController.sys_runFFCNormalization();
    }
}

/*
#include <Arduino.h>
#include "LeptonFLiR.h"
#include "BluetoothSerial.h"

const byte flirCSPin = 5;
LeptonFLiR flirController(Wire, flirCSPin); // Library using Wire and chip select pin D22
BluetoothSerial SerialBT;

void writeBMPFile(byte *imageData, int width, int height, int pitch);

void setup() {
    Serial.begin(115200);
    SerialBT.begin("Lepton IR");

    Wire.begin();                       // Wire must be started first
    Wire.setClock(400000);              // Supported baud rates are 100kHz, 400kHz, and 1000kHz
    SPI.begin();                        // SPI must be started first as well

    // Using memory allocation mode 80x60 8bpp and Celsius temperature mode
    flirController.init(LeptonFLiR_ImageStorageMode_80x60_8bpp, LeptonFLiR_TemperatureMode_Celsius);

    // Setting use of AGC for histogram equalization (since we only have 8-bit per pixel data anyways)
    flirController.agc_setAGCEnabled(ENABLED);

    flirController.sys_setTelemetryEnabled(ENABLED); // Ensure telemetry is enabled
}

uint32_t lastFrameNumber = -1;          // Tracks for when a new frame is available

void loop() {
    if (flirController.readNextFrame()) { // Read next frame and store result into internal imageData
        Serial.println("Next frame");
        uint32_t frameNumber = flirController.getTelemetryFrameCounter();

        if (frameNumber > lastFrameNumber) { // Frame counter increments every 3rd frame due to export restrictions
            lastFrameNumber = frameNumber;

            //char fileName[] = "FLIR/IMG0000.BMP";
            //uint16_t fileNumber = (uint16_t)(frameNumber / 3);
            //wordsToHexString((uint16_t *)&fileNumber, 1, &fileName[8], 4);

            //File bmpFile = SD.open(fileName, FILE_WRITE);

            //if (bmpFile) {
                writeBMPFile(flirController.getImageData(),
                             flirController.getImageWidth(),
                             flirController.getImageHeight(),
                             flirController.getImagePitch());

                //bmpFile.close();

                //Serial.print(fileName);
                Serial.println(" written...");
        }

        // Occasionally flat field correction normalization needs ran
        if (flirController.getShouldRunFFCNormalization())
            flirController.sys_runFFCNormalization();
    }
}
*/
// Writes a BMP file out, code from: http://stackoverflow.com/questions/2654480/writing-bmp-image-in-pure-c-c-without-other-libraries
void writeBMPFile(byte *imageData, int width, int height, int pitch) {
    
    byte file[14] = {
        'B','M', // magic
        0,0,0,0, // size in bytes
        0,0, // app data
        0,0, // app data
        40+14,0,0,0 // start of data offset
    };
    byte info[40] = {
        40,0,0,0, // info hd size
        0,0,0,0, // width
        0,0,0,0, // heigth
        1,0, // number color planes
        24,0, // bits per pixel
        0,0,0,0, // compression is none
        0,0,0,0, // image bits size
        0x13,0x0B,0,0, // horz resoluition in pixel / m
        0x13,0x0B,0,0, // vert resolutions (0x03C3 = 96 dpi, 0x0B13 = 72 dpi)
        0,0,0,0, // #colors in pallete
        0,0,0,0, // #important colors
    };

    uint32_t padSize  = (4-(width*3)%4)%4;
    uint32_t sizeData = width*height*3 + height*padSize;
    uint32_t sizeAll  = sizeData + sizeof(file) + sizeof(info);

    file[ 2] = (byte)((sizeAll      ) & 0xFF);
    file[ 3] = (byte)((sizeAll >>  8) & 0xFF);
    file[ 4] = (byte)((sizeAll >> 16) & 0xFF);
    file[ 5] = (byte)((sizeAll >> 24) & 0xFF);
    info[ 4] = (byte)((width      ) & 0xFF);
    info[ 5] = (byte)((width >>  8) & 0xFF);
    info[ 6] = (byte)((width >> 16) & 0xFF);
    info[ 7] = (byte)((width >> 24) & 0xFF);
    info[ 8] = (byte)((height      ) & 0xFF);
    info[ 9] = (byte)((height >>  8) & 0xFF);
    info[10] = (byte)((height >> 16) & 0xFF);
    info[11] = (byte)((height >> 24) & 0xFF);
    info[20] = (byte)((sizeData      ) & 0xFF);
    info[21] = (byte)((sizeData >>  8) & 0xFF);
    info[22] = (byte)((sizeData >> 16) & 0xFF);
    info[23] = (byte)((sizeData >> 24) & 0xFF);

    SerialBT.write((byte *)file, sizeof(file));
    SerialBT.write((byte *)info, sizeof(info));

    byte pad[3] = {0,0,0};
    imageData += (height - 1) * pitch;

    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            byte pixel[3]; // blue green red
            pixel[0] = pixel[1] = pixel[2] = imageData[x];

            SerialBT.write((byte *)pixel, 3);
        }

        SerialBT.write((byte *)pad, padSize);
        imageData -= pitch;
    }
    
    SerialBT.end();
}