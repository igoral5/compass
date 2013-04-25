/*
 * compass.cpp
 *
 *  Created on: 25.04.2013
 *      Author: igor
 */

#include "compass.h"

Compass::Compass(const std::string& name_file) : wire_( name_file, HMC5883L_ADDRESS), scale_(1)
{
}

Compass::~Compass() throw()
{
}

MagnetometerRaw Compass::readRawAxis()
{
    uint8_t buffer[DATA_REGISTER_COUNT];
    uint16_t reg = DATA_REGISTER_BEGIN;
    int ind = 0;
    for(; ind < DATA_REGISTER_COUNT; ++reg, ++ind)
    {
        buffer[ind] = wire_.i2c_smbus_read_byte_data(reg);
    }
    MagnetometerRaw raw = MagnetometerRaw();
    short int tmp = (buffer[0] << 8) | buffer[1];
    raw.XAxis = tmp;
    tmp = (buffer[2] << 8) | buffer[3];
    raw.ZAxis = tmp;
    tmp = (buffer[4] << 8) | buffer[5];
    raw.YAxis = tmp;
    return raw;
}

MagnetometerScaled Compass::readScaledAxis()
{
    MagnetometerRaw raw = readRawAxis();
    MagnetometerScaled scaled = MagnetometerScaled();
    scaled.XAxis = raw.XAxis * scale_;
    scaled.ZAxis = raw.ZAxis * scale_;
    scaled.YAxis = raw.YAxis * scale_;
    return scaled;
}

void Compass::setMeasurementMode(uint8_t mode)
{
    wire_.i2c_smbus_write_byte_data(MODE_REGISTER, mode);
}

void Compass::setScale(double gauss)
{
    uint8_t regValue = 0x00;
    if(gauss == 0.88)
    {
        regValue = 0x00;
        scale_ = 0.73;
    }
    else if(gauss == 1.3)
    {
        regValue = 0x01;
        scale_ = 0.92;
    }
    else if(gauss == 1.9)
    {
        regValue = 0x02;
        scale_ = 1.22;
    }
    else if(gauss == 2.5)
    {
        regValue = 0x03;
        scale_ = 1.52;
    }
    else if(gauss == 4.0)
    {
        regValue = 0x04;
        scale_ = 2.27;
    }
    else if(gauss == 4.7)
    {
        regValue = 0x05;
        scale_ = 2.56;
    }
    else if(gauss == 5.6)
    {
        regValue = 0x06;
        scale_ = 3.03;
    }
    else if(gauss == 8.1)
    {
        regValue = 0x07;
        scale_ = 4.35;
    }
    else
       throw CompassError("Entered scale was not valid, valid gauss values are: 0.88, 1.3, 1.9, 2.5, 4.0, 4.7, 5.6, 8.1");

    // Setting is in the top 3 bits of the register.
    regValue = regValue << 5;
    wire_.i2c_smbus_write_byte_data(CONFIGURATION_REGISTERB, regValue);
}







