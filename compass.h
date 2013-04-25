/*
 * compass.h
 *
 *  Created on: 25.04.2013
 *      Author: igor
 */

#ifndef COMPASS_H_
#define COMPASS_H_

#include <string>
#include <stdexcept>
#include "i2c.h"

#define HMC5883L_ADDRESS 0x1E
#define CONFIGURATION_REGISTERA 0x00
#define CONFIGURATION_REGISTERB 0x01
#define MODE_REGISTER 0x02
#define DATA_REGISTER_BEGIN 0x03
#define DATA_REGISTER_COUNT 6

#define MEASUREMENT_CONTINUOUS 0x00
#define MEASUREMENT_SINGLE_SHOT 0x01
#define MEASUREMENT_IDLE 0x03

struct MagnetometerScaled
{
    double XAxis;
    double YAxis;
    double ZAxis;
};

struct MagnetometerRaw
{
    int XAxis;
    int YAxis;
    int ZAxis;
};

class CompassError : public std::runtime_error
{
public:
    explicit CompassError(const std::string& text_error) : std::runtime_error(text_error) {}
    virtual ~CompassError() throw() {}
};

class Compass
{
public:
    explicit Compass(const std::string& name_file);
    virtual ~Compass() throw();
    MagnetometerRaw readRawAxis();
    MagnetometerScaled readScaledAxis();
    void setMeasurementMode(uint8_t mode);
    void setScale(double gauss);
private:
    I2c wire_;
    double scale_;
};


#endif /* COMPASS_H_ */
