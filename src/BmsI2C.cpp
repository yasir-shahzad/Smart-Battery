/**************************************************************************/
/*!
    @file     BmsI2C.cpp

    This is a library for the Byonyks BmsI2C Battery.

    Written by Kevin "Yasir Shahzad" Townsend for Byonyks Industries.

    BSD license, all text here must be included in any redistribution
*/
/**************************************************************************/

extern "C" {
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
}

#include "BmsI2C.h"

//constructor of the class
BmsI2C::BmsI2C() {
    // I2C bus, I2C address in the constructor
    begin(1, BmsI2C_ADDRESS);
}

/**
 * @brief this method opens i2c port
 * @param i2c_bus  i2c bus number on physical board
 * @param i2c_addr salve address
 * @return 1 on success, 0 on failure
 */
bool BmsI2C::begin(uint8_t i2c_bus, uint8_t i2c_addr) {
    char device[32];
    address = i2c_addr;
    snprintf(device, sizeof(device), "/dev/i2c-%u", i2c_bus); // creating device address buffer
    if ((fd = open(device, O_RDWR)) < 0) {
        printf("File descriptor opening error %s\n", strerror(errno));
        return 0;
    }
    else {
        if (ioctl(fd, I2C_SLAVE, address) < 0)
        {
            std::cout << "Open fd error" << errno << std::endl;
            return 0;
        }
        printf("I2C connection established\n");
        return 1;
    }
}

/**
 * @brief the method reads the temperature of the battery
 * @return uint16_t temperature value
 */
float BmsI2C::readTemperature() {
    uint16_t data;
    float result;
    // read temperature from register
    data = read16(BmsI2C_REG_GET_TEMPERATURE);
    if (data > 1)
    {
        result = (data - 2731) / 10.0;
        #ifdef ENABLE_DEBUG_OUTPUT
          cout << "BmsI2C Temperature: " << result << "*C" << endl;
        #endif // DEBUG
    }
    return result;
}

/**
 * @brief this method reads the voltage of the battery in mv
 * @return uint16_t voltage of the battery 
 */
uint16_t BmsI2C::readVoaltage() {
    uint16_t result;
    // read Voltage from register
    result = read16(BmsI2C_REG_GET_VOLTAGE);
    if (result > 1)
    {
     #ifdef ENABLE_DEBUG_OUTPUT
        cout << "BmsI2C Voaltage: " << result << "mv" << endl;
     #endif // DEBUG
    }
    return result;
}

/**
 * @brief this method show the full charge capacity of the battery
 * @return uint16_t full charge capacity in mv
 */
uint16_t BmsI2C::fullChageCapacity() {
    uint16_t result;
    // read full charge capacity from register
    result = read16(BmsI2C_REG_GET_FC_CAPACITY);
    if (result > 1)
    {
     #ifdef ENABLE_DEBUG_OUTPUT
        cout << "BmsI2C FullChargeCapacity: " << result << "ma" << endl;
     #endif // DEBUG
    }
    return result;
}

/**
 * @brief this method reads the remaining capacity of the battery
 * @return uint16_t remaining capacity of battery
 */
uint16_t BmsI2C::remainaningCapacity() {
    uint16_t result;
    // read Voltage from register
    result = read16(BmsI2C_REG_GET_RM_CAPACITY);
    if (result > 1)
    {
     #ifdef ENABLE_DEBUG_OUTPUT
        cout << "BmsI2C RemainingCapacity: " << result << "ma" << endl;
     #endif // DEBUG
    }
    return result;
}

/**
 * @brief  this method reads the vlaue of charge/discharge current in ma
 * @return int32_t -ve discharge current, +ve charge current
 */
int32_t BmsI2C::readCurrent() {
    uint8_t data[5];
    int32_t result;
    uint8_t ret;
    // read Current from register
    ret = i2c_smbus_read_i2c_block_data(fd, BmsI2C_REG_GET_CURRENT, sizeof(data), data);
    // data[0] contains the length of the data
    if(ret > 1)  // number of bytes that were read
    result = data[4] << 24 | data[3] << 16 | data[2] << 8 | data[1]; // actual data

    #ifdef ENABLE_DEBUG_OUTPUT
    // for (int i = 1; i < 5; i++)
    // printf("%x ",data[i]);
    // cout<<endl;
    cout << "BmsI2C Current: " << result << "ma" << endl;
    #endif // DEBUG

    return result;
}

/**
 * @brief this method shows the relativeStateofCharge
 * @return uint16_t (0 - 100) percent
 */
uint16_t BmsI2C::RelativeStateOfCharge() {
    uint16_t percent;
    // read battery status from register
    percent = read16(BmsI2C_REG_GET_PERCENTAGE);
    #ifdef ENABLE_DEBUG_OUTPUT
      cout << "BmsI2C Percentage: " << percent << "%" << endl;
    #endif // DEBUG
    return percent;
}

/**
 * @brief this method write byte on i2c slave
 * @param register_addr value to be written
 * @return uint8_t 1 on success, -1 on failure
 */
uint8_t BmsI2C::write8(uint8_t register_addr) {
    uint8_t reg = register_addr;
    // write value to related register
    int result = i2c_smbus_write_byte(fd, reg);
    if (result < 0)
    {
        printf("BmsI2C Write Byte error: %s\n", strerror(errno));
        return -1;
    }
    return 1;
}

/**
 * @brief Read value from specific register
 * @param register_addr address of the register
 * @return uint16_t value on success, -1 on failure
 */
uint16_t BmsI2C::read16(uint8_t register_addr) {
    uint16_t result;

    if (write8(BmsI2C_PACK_ADDRESS) == -1) {
        return -1;
    }
    result = i2c_smbus_read_word_data(fd, register_addr);
    if (result < 0) {
        printf("BmsI2C Write Word error: %s\n", strerror(errno));
        return -1;
    }
    return result;
}

//destructor
BmsI2C::~BmsI2C()
{
    //close file-descriptor
    close(fd);
}