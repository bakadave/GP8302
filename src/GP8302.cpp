/**
 * @file GP8302.cpp
 * @author David Baka
 * @brief
 * @version 0.2.5
 * @date 2023-10-02
 *  
 * @copyright Copyright (c) 2023 David Baka
 * 
 */

#include "GP8302.h"

GP8302::GP8302(byte _slaveAddress, TwoWire *_wire) {
    slaveAddress = _slaveAddress;
    wire = _wire;
}

gp8302_return_t GP8302::outputRaw(gp8302_out_t dac) {
    gp8302_return_t ret;
    dac &= GP8302_MAX_DAC_VAL;

    ret = writeRegister(GP8302_CURRENT_REG, dac);

    return ret;
}

gp8302_return_t GP8302::stopOutput() {
    return outputRaw((gp8302_out_t)0x0000);
}

gp8302_return_t GP8302::output_mA(float current) {
    if(current < curr_min)
        current = curr_min;

    if(current > curr_max)
        current = curr_max;

    if(!calibrated)
        return GP8302_I2C_OTHER_ERR;
    
    gp8302_out_t val = current * calib_val;
    return outputRaw(val);
}

bool GP8302::calibrate(uint16_t dac, float current) {
    if((dac <= 0x00) || (dac > GP8302_MAX_DAC_VAL) || (current <= 0))
        return false;

    calib_val = (float)dac / current;
    calibrated = true;
    return true;
}

void GP8302::setMinMaxCurrent(float min, float max) {
    curr_min = min;
    
    if(max > GP8302_MAX_CURRENT)
        curr_max = GP8302_MAX_CURRENT;
    
    curr_max = max;
}

// ================= PRIVATE METHODS =================
gp8302_return_t GP8302::readRegister(gp8302_reg_t reg, uint8_t* value) {
    uint8_t ret;
    wire->beginTransmission(slaveAddress);
    wire->write(reg);
    ret = wire->endTransmission(false);

    delay(2);

    wire->requestFrom((int)slaveAddress, 1, (int)true);
    wire->readBytes(value, 1);

    return (gp8302_return_t)ret;
}

gp8302_return_t GP8302::writeRegister(gp8302_reg_t reg, gp8302_out_t value) {
    uint8_t ret;
    wire->beginTransmission(slaveAddress);
    wire->write(reg);
    wire->write((uint8_t)value << 8);
    wire->write((uint8_t)value & 0xFF);

    ret = wire->endTransmission(true);

    return (gp8302_return_t)ret;
}