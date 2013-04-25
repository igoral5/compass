/*
 * i2c.cpp
 *
 *  Created on: 25.04.2013
 *      Author: igor
 */

#include "i2c.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

I2c::I2c(const std::string& name_file)
{
    open(name_file);
}

I2c::I2c(const std::string& name_file, uint16_t slave_address)
{
   open(name_file);
   i2c_smbus_set_slave(slave_address);
}

I2c::~I2c() throw()
{
    close(fd_);
}

void I2c::i2c_smbus_set_slave(uint16_t slave_address)
{
    if (ioctl( fd_, I2C_SLAVE, slave_address))
    {
        throw I2cError(strerror(errno));
    }
}

void I2c::open(const std::string& name_file)
{
    if ( ( fd_ = ::open(name_file.c_str(), O_RDWR ) ) < 0 )
    {
        throw I2cError(strerror(errno));
    }
}

uint8_t I2c::i2c_smbus_read_byte_data(uint8_t address)
{
    union i2c_smbus_data data;
    i2c_smbus_access(I2C_SMBUS_READ, address, I2C_SMBUS_BYTE_DATA, &data);
    return 0xff & data.byte;
}

void I2c::i2c_smbus_write_byte_data(uint8_t address, uint8_t value)
{
    union i2c_smbus_data data;
    data.byte = value;
    i2c_smbus_access(I2C_SMBUS_WRITE, address, I2C_SMBUS_BYTE_DATA, &data);
}

uint16_t I2c::i2c_smbus_read_word_data(uint8_t address)
{
    union i2c_smbus_data data;
    i2c_smbus_access(I2C_SMBUS_READ, address, I2C_SMBUS_WORD_DATA, &data);
    return 0xffff & data.word;
}

void I2c::i2c_smbus_write_word_data(uint8_t address, uint16_t value)
{
    union i2c_smbus_data data;
    data.word = value;
    i2c_smbus_access(I2C_SMBUS_WRITE, address, I2C_SMBUS_WORD_DATA, &data);
}

void I2c::i2c_smbus_access(char read_write, uint8_t command, int size, union i2c_smbus_data *data)
{
    struct i2c_smbus_ioctl_data args;
    args.read_write = read_write;
    args.command = command;
    args.size = size;
    args.data = data;
    if (ioctl(fd_, I2C_SMBUS, &args))
    {
        throw I2cError(strerror(errno));
    }
}



