/**
 * @file GP8302_defs.h
 * @author David Baka
 * @brief
 * @version 0.2.5
 * @date 2023-10-02
 *  
 * @copyright Copyright (c) 2023 David Baka
 * 
 */

#ifndef __GP8302_DEFS_H__
#define __GP8302_DEFS_H__

typedef enum : uint8_t{
    GP8302_I2C_SUCCESS = 0,
    GP8302_I2C_DATA_LONG,
    GP8302_I2C_ADDR_NACK,
    GP8302_I2C_DATA_NACK,
    GP8302_I2C_OTHER_ERR
} gp8302_return_t;

#define GP8302_DEFAULT_ADDR         0x58
#define GP8302_CURRENT_REG          0x02
#define GP8302_MAX_DAC_VAL          0xFFF
#define GP8302_MAX_CURRENT          25      // mA

typedef uint16_t gp8302_out_t;
typedef uint8_t gp8302_reg_t;

#endif // __GP8302_DEFS_H__