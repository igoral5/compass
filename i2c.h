/*
 * i2c.h
 *
 *  Created on: 25.04.2013
 *      Author: igor
 */

#ifndef I2C_H_
#define I2C_H_

#include <string>
#include <stdexcept>
#include <stdint.h>
#include <stdexcept>

#define I2C_SMBUS_READ 1
#define I2C_SMBUS_WRITE 0

#define I2C_SMBUS_BLOCK_MAX 32

#define I2C_SMBUS_BYTE_DATA 2
#define I2C_SMBUS_WORD_DATA 3

union i2c_smbus_data
{
    uint8_t byte;
    uint16_t word;
    uint8_t block[I2C_SMBUS_BLOCK_MAX + 2]; // block[0] is used for length
};

class I2cError : public std::runtime_error
{
public:
    explicit I2cError(const std::string& text_error) : std::runtime_error( text_error ) {}
    virtual ~I2cError() throw() {}
};

class I2c
{
public:
    explicit I2c(const std::string& name_file);
    I2c(const std::string& name_file, uint16_t slave_address);
    virtual ~I2c() throw();
    void i2c_smbus_set_slave(uint16_t slave_address);
    uint8_t i2c_smbus_read_byte_data(uint8_t address);
    void i2c_smbus_write_byte_data(uint8_t address, uint8_t value);
    uint16_t i2c_smbus_read_word_data(uint8_t address);
    void i2c_smbus_write_word_data(uint8_t address, uint16_t value);
private:
    void i2c_smbus_access(char read_write, uint8_t command, int size, union i2c_smbus_data *data);
    void open(const std::string& name_file);
    int fd_;
};


#endif /* I2C_H_ */
