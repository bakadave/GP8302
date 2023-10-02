/**
 * @file GP8302.cpp
 * @author David Baka
 * @brief
 * @version 0.2.2
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
    return outputRaw(0);
}

gp8302_return_t GP8302::output_mA(float current) {
    gp8302_return_t ret;

    if(current < GP8302_MIN_CURRENT)
        current = GP8302_MIN_CURRENT;

    if(current > GP8302_MAX_CURRENT)
        current = GP8302_MAX_CURRENT;
    
    gp8302_out_t val = 0;

    if(!calibrated)
        return GP8302_I2C_OTHER_ERR;

    val = (current - GP8302_MIN_CURRENT)*((cal_20mA - cal_4mA)/(GP8302_MAX_CURRENT - GP8302_MIN_CURRENT) + cal_4mA);

    ret = outputRaw(val);

    return ret;
}

bool GP8302::calibrate(uint16_t dac_4mA, uint16_t dac_20mA) {
    if((dac_4mA >= dac_20mA) || (dac_20mA > GP8302_MAX_DAC_VAL))
        return false;
    cal_4mA  = dac_4mA;
    cal_20mA = dac_20mA;
    calibrated = true;
    return true;
}

// ================= PRIVATE METHODS =================
gp8302_return_t GP8302::readRegister(gp8302_reg_t reg, uint8_t* value) {
    uint8_t ret;
    wire->beginTransmission(slaveAddress);
    wire->write(reg);
    ret = wire->endTransmission(false);

    delay(2);

    wire->requestFrom(slaveAddress, 1, (int)true);
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