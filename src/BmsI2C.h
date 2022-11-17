/**************************************************************************/
/*!
    @file     BmsI2C.h

    This is a library for the Byonyks BmsI2C Battery.

    Written by Kevin "Yasir Shahzad" Townsend for Byonyks Industries.

    BSD license, all text here must be included in any redistribution
*/
/**************************************************************************/
#ifndef __BMSI2C_H
#define __BMSI2C_H

#include <fcntl.h>
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <iostream>

using namespace std;

//#define ENABLE_DEBUG_OUTPUT

/*=========================================================================
    I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
#define BmsI2C_ADDRESS                           (0x0B)

/*=========================================================================
    POINTER REGISTER
    -----------------------------------------------------------------------*/
#define BmsI2C_PACK_ADDRESS                      (0x16)
#define BmsI2C_REG_GET_TEMPERATURE               (0x08)
#define BmsI2C_REG_GET_PERCENTAGE                (0x0D)
#define BmsI2C_REG_GET_VOLTAGE                   (0x09)
#define BmsI2C_REG_GET_CURRENT                   (0x2A)
#define BmsI2C_REG_GET_FC_CAPACITY               (0x10)
#define BmsI2C_REG_GET_RM_CAPACITY               (0x0F)


class BmsI2C {
  private:
    int fd;
    uint8_t address;

  public:
    BmsI2C();
    ~BmsI2C();
    bool begin(uint8_t, uint8_t); 
    uint16_t readVoaltage();
    int32_t readCurrent();
    float readTemperature();
    uint16_t RelativeStateOfCharge();
    uint16_t fullChageCapacity();
    uint16_t remainaningCapacity();

    uint8_t write8(uint8_t);
    uint16_t read16(uint8_t);
};
#endif