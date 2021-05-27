#include <i2c_driver.h>

I2C::I2C(){
    Wire.begin();
    return;
}

I2C::~I2C(){
    return;
}

void I2C::frequency(uint32_t frequency){
    Wire.setClock(frequency);
}

uint8_t I2C::write(uint8_t slaveAddress, char* command, uint8_t size, uint8_t dummy){
    uint8_t *data = new uint8_t[size];

    memcpy(data, command, size);

    Wire.beginTransmission(slaveAddress);
    Wire.write(data,size);

    delete data;

    return Wire.endTransmission();
}

uint8_t I2C::read(uint8_t slaveAddress, char* buffer, uint8_t size, uint8_t dummy){
    Wire.requestFrom(slaveAddress, size);
    uint8_t i = 0;

    while(Wire.available()){
        buffer[i++] = Wire.read();
        Serial.println(buffer[i-1]);
    }

    return 1;
}

void I2C::stop(){
    return;
}

void wait_us(uint32_t microSeconds){
    delayMicroseconds(microSeconds);
}