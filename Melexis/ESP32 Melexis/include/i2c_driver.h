#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include <stdint.h>
#include <Arduino.h>
#include <Wire.h>

class I2C{
    private:

    public:
        I2C();
        ~I2C();
        void frequency(uint32_t frequency);
        uint8_t write(uint8_t slaveAddress, char* command, uint8_t size, uint8_t dummy);
        uint8_t read(uint8_t slaveAddress, char* buffer, uint8_t size, uint8_t dummy);
        void stop();
};

void wait_us(uint32_t microSeconds);

#endif