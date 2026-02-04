#ifndef LSM303AGR_H
#define LSM303AGR_H

#include "stdint.h"

#define LSM303AGR_ACC_ADDR 0x19
#define LSM303AGR_MAG_ADDR 0x1E

#define LSM303_AUTO_INCREMENT 0x80

#define LSM_TEMP_OFFSET 15
#define LSM_TEMP_ENABLE (0x03 << 6)

typedef enum {
    STATUS_REG_AUX_A = 0x07,
    OUT_TEMP_L_A = 0x0C,
    OUT_TEMP_H_A = 0x0D,
    INT_COUNTER_REG_A = 0x0E,
    WHO_AM_I_A = 0x0F,
    TEMP_CFG_REG_A = 0x1F,
    CTRL_REG1_A = 0x20,
    CTRL_REG2_A = 0x21,
    CTRL_REG3_A = 0x22,
    CTRL_REG4_A = 0x23,
    CTRL_REG5_A = 0x24,
    CTRL_REG6_A = 0x25,
    REFERENCE_DATACAPTURE_A = 0x26,
    STATUS_REG_A = 0x27,
    OUT_X_L_A = 0x28,
    OUT_X_H_A = 0x29,
    OUT_Y_L_A = 0x2A,
    OUT_Y_H_A = 0x2B,
    OUT_Z_L_A = 0x2C,
    OUT_Z_H_A = 0x2D,
    FIFO_CTRL_REG_A = 0x2E,
    FIFO_SRC_REG_A = 0x2F,
    INT1_CFG_A = 0x30,
    INT1_SRC_A = 0x31,
    INT1_THS_A = 0x32,
    INT1_DURATION_A = 0x33,
    INT2_CFG_A = 0x34,
    INT2_SRC_A = 0x35,
    INT2_THS_A = 0x36,
    INT2_DURATION_A = 0x37,
    CLICK_CFG_A = 0x38,
    CLICK_SRC_A = 0x39,
    CLICK_THS_A = 0x3A,
    TIME_LIMIT_A = 0x3B,
    TIME_LATENCY_A = 0x3C,
    TIME_WINDOW_A = 0x3D,
    ACT_THS_A = 0x3E,
    ACT_DUR_A = 0x3F,
    OFFSET_X_REG_L_M = 0x45,
    OFFSET_X_REG_H_M = 0x46,
    OFFSET_Y_REG_L_M = 0x47,
    OFFSET_Y_REG_H_M = 0x48,
    OFFSET_Z_REG_L_M = 0x49,
    OFFSET_Z_REG_H_M = 0x4A,
    WHO_AM_I_M = 0x4F,
    CFG_REG_A_M = 0x60,
    CFG_REG_B_M = 0x61,
    CFG_REG_C_M = 0x62,
    INT_CRTL_REG_M = 0x63,
    INT_SOURCE_REG_M = 0x64,
    INT_THS_L_REG_M = 0x65,
    INT_THS_H_REG_M = 0x66,
    STATUS_REG_M = 0x67,
    OUTX_L_REG_M = 0x68,
    OUTX_H_REG_M = 0x69,
    OUTY_L_REG_M = 0x6A,
    OUTY_H_REG_M = 0x6B,
    OUTZ_L_REG_M = 0x6C,
    OUTZ_H_REG_M = 0x6D,
} LSM_Registers_t;

typedef enum {
    LSM_ODR_POWER_DOWN = 0b0000,
    LSM_ODR_1Hz = 0b0001,
    LSM_ODR_10Hz = 0b0010,
    LSM_ODR_25Hz = 0b0011,
    LSM_ODR_50Hz = 0b0100,
    LSM_ODR_100Hz = 0b0101,
    LSM_ODR_200Hz = 0b0110,
    LSM_ODR_400Hz = 0b0111,
    LSM_ODR_1_620kHz = 0b1000,
    LSM_ODR_1_344kHz = 0b1001
} lsm_acc_odr_t;

typedef union {
    struct {
        uint8_t enable_x : 1;
        uint8_t enable_y : 1;
        uint8_t enable_z : 1;
        uint8_t enable_low_power : 1;
        uint8_t data_rate_odr : 4;
    };
    uint8_t raw;
} lsm_ctrl_reg1_a_t;

typedef enum {
    TEST_NORMAL_MODE,
    TEST_SELF_0,
    TEST_SELF_1,
} lsm_self_test_t;

typedef enum {
    LSM_ACC_FS_2G = 0b00,
    LSM_ACC_FS_4G = 0b01,
    LSM_ACC_FS_8G = 0b10,
    LSM_ACC_FS_16G = 0b11,
} lsm_acc_fs_t;

typedef union {
    struct {
        uint8_t spi_enable : 1;
        uint8_t self_test : 2;
        uint8_t high_res : 1;
        uint8_t full_scale : 2;
        uint8_t ble : 1;
        uint8_t bdu : 1;
    };
    uint8_t raw;
} lsm_ctrl_reg4_a_t;

typedef enum {
    LSM_MODE_LOW_POWER = 0,
    LSM_MODE_NORMAL = 1,
    LSM_MODE_HIGH_RESOLUTION = 2
} lsm_operating_mode_t;

typedef struct {
    int acceleration_x;
    int acceleration_y;
    int acceleration_z;
} lsm_acc_data_t;

int lsm303_acc_init(uint8_t lsm_operating_mode);
void lsm303_tmp_init(void);

void lsm303_acc_get_raw_data(int16_t* ax, int16_t* ay, int16_t* az);

lsm_acc_data_t lsm_acc_get_acceleration();
int lsm303_acc_get_temperature(void);

uint8_t lsm303_acc_read_reg(LSM_Registers_t reg);
void lsm303_acc_write_reg(LSM_Registers_t reg, uint8_t value);

#endif
