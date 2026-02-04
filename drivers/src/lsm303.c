#include "lsm303.h"
#include "i2c.h"
#include <stdint.h>

static lsm_operating_mode_t mode;

static lsm_ctrl_reg1_a_t reg1 = { 0 };
static lsm_ctrl_reg4_a_t reg4 = { 0 };

void lsm_acc_read_mult(LSM_Registers_t reg, uint8_t* buffer, uint8_t len)
{
    uint8_t reg_addr = (uint8_t)reg | LSM303_AUTO_INCREMENT;
    i2c_write(LSM303AGR_ACC_ADDR, &reg_addr, 1);
    i2c_read(LSM303AGR_ACC_ADDR, buffer, len);
}

void lsm303_acc_get_raw_data(int16_t* ax, int16_t* ay, int16_t* az)
{
    uint8_t buffer_rx[6];
    lsm_acc_read_mult(OUT_X_L_A, buffer_rx, 6);

    uint16_t raw_x = (buffer_rx[1] << 8) | buffer_rx[0];
    uint16_t raw_y = (buffer_rx[3] << 8) | buffer_rx[2];
    uint16_t raw_z = (buffer_rx[5] << 8) | buffer_rx[4];

    switch (mode) {
    case LSM_MODE_LOW_POWER:
        *ax = ((int16_t)raw_x) >> 8;
        *ay = ((int16_t)raw_y) >> 8;
        *az = ((int16_t)raw_z) >> 8;
        break;
    case LSM_MODE_NORMAL:
        *ax = ((int16_t)raw_x) >> 6;
        *ay = ((int16_t)raw_y) >> 6;
        *az = ((int16_t)raw_z) >> 6;
        break;
    case LSM_MODE_HIGH_RESOLUTION:
        *ax = ((int16_t)raw_x) >> 4;
        *ay = ((int16_t)raw_y) >> 4;
        *az = ((int16_t)raw_z) >> 4;
        break;
    }
}

int lsm_get_sensitivity(lsm_operating_mode_t mode, lsm_acc_fs_t full_scale)
{
    static const uint16_t sensitivity_lut[3][4] = {
        [LSM_MODE_LOW_POWER] = { 16, 32, 64, 192 },
        [LSM_MODE_NORMAL] = { 4, 8, 16, 48 },
        [LSM_MODE_HIGH_RESOLUTION] = { 1, 2, 4, 12 }
    };

    return sensitivity_lut[mode][full_scale];
}

lsm_acc_data_t lsm_acc_get_acceleration()
{
    int16_t rx, ry, rz;
    lsm_acc_data_t data;

    lsm303_acc_get_raw_data(&rx, &ry, &rz);

    int sensitivity = lsm_get_sensitivity(mode, reg4.full_scale);

    data.acceleration_x = (int)rx * sensitivity;
    data.acceleration_y = (int)ry * sensitivity;
    data.acceleration_z = (int)rz * sensitivity;
    return data;
}

uint8_t lsm303_acc_read_reg(LSM_Registers_t reg)
{
    uint8_t reg_addr = (uint8_t)reg;
    uint8_t data_received = 0;

    i2c_write(LSM303AGR_ACC_ADDR, &reg_addr, 1);
    i2c_read(LSM303AGR_ACC_ADDR, &data_received, 1);

    return data_received;
}

void lsm303_acc_write_reg(LSM_Registers_t reg, uint8_t value)
{
    uint8_t buffer[2];
    buffer[0] = (uint8_t)reg;
    buffer[1] = value;

    i2c_write(LSM303AGR_ACC_ADDR, buffer, 2);
}

int lsm303_acc_init(uint8_t lsm_operating_mode)
{
    i2c_init(TWIM_FREQ_100KBPS);

    mode = lsm_operating_mode;

    switch (mode) {
    case LSM_MODE_LOW_POWER:
        reg1.enable_low_power = 1;
        reg4.high_res = 0;
        break;
    case LSM_MODE_NORMAL:
        reg1.enable_low_power = 0;
        reg4.high_res = 0;
        break;
    case LSM_MODE_HIGH_RESOLUTION:
        reg1.enable_low_power = 0;
        reg4.high_res = 1;
        break;
    }

    reg1.data_rate_odr = LSM_ODR_1_344kHz;
    reg1.enable_x = 1;
    reg1.enable_y = 1;
    reg1.enable_z = 1;

    lsm303_acc_write_reg(CTRL_REG1_A, reg1.raw);

    reg4.bdu = 1;
    reg4.full_scale = LSM_ACC_FS_2G;
    reg4.spi_enable = 0;

    lsm303_acc_write_reg(CTRL_REG4_A, reg4.raw);

    return 0;
}
