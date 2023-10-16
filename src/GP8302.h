/**
 * @file GP8302.h
 * @author David Baka
 * @brief
 * @version 0.2.5
 * @date 2023-10-02
 *  
 * @copyright Copyright (c) 2023 David Baka
 * 
 */

#ifndef __GP8302_H__
#define __GP8302_H__

#include <Arduino.h>
#include <Wire.h>
#include "GP8302_defs.h"

class GP8302 {
    public:

        /**
         * @brief constructor for GP8302 class
         * @param _slaveAddress I2C address of the DAC
         * @param _wire TwoWire instance for I2C bus
        */
        GP8302(byte _slaveAddress = GP8302_DEFAULT_ADDR, TwoWire *_wire = &Wire);

        /**
         * @brief sets DAC value directly
         * @note input value is capped at 0xFFF (IC limitation)
         * @param dac value to be written in output register (valied values: 0x00 - 0xFFF)
         * @return success: 0, other: I2C error
        */
        gp8302_return_t outputRaw(gp8302_out_t dac);

        /**
         * @brief sets DAC output to 0
         * @return success: 0, other: I2C error
        */
        gp8302_return_t stopOutput();

        /**
         * @brief sets output in mA
         * @note only values between 0-20 mA are accepted
         * @note returns error if calibration values are not called before calling this function
         * @param current desired current in mA
         * @return success: 0, other: I2C error
        */
        gp8302_return_t output_mA(float current);

        /**
         * @brief sets calibration values for 4mA and 20mA to use in current loops
         * @param dac raw DAC value for calibraiton current current
         * @param current calibration current value in mA
         * @return true: sucess false: setting failed
        */
        bool calibrate(uint16_t dac, float current = (20.0F));

        /**
         * @brief sets minimum and maximum current values for output_mA function
         * @param min minimum current value in mA
         * @param max maximum current value in mA
        */
        void setMinMaxCurrent(float min, float max);

    private:
        /**
         * @brief private method to read single register on I2C bus
         * @param reg register to read
         * @param value pointer to variable hold the data
         * @return success: 0, other: I2C error
        */
        gp8302_return_t readRegister(gp8302_reg_t reg, uint8_t* value);

        /**
         * @brief private method to read single register on I2C bus
         * @param reg register to write
         * @param value data to be written
         * @return success: 0, other: I2C error
        */
        gp8302_return_t writeRegister(gp8302_reg_t reg, gp8302_out_t value);

        uint8_t slaveAddress;
        TwoWire* wire;
        float curr_min = 4;
        float curr_max = 20;
        float calib_val;
        bool calibrated = false;
};

#endif // __GP8302_H__