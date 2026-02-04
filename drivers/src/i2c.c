#include "i2c.h"

void i2c_init(Twim_Frequency_t frequency)
{
    I2C0.TASKS_STOP = 1;

    I2C0.PSEL_SCL = PIN_SCL;
    I2C0.PSEL_SDA = PIN_SDA;

    I2C0.FREQUENCY = frequency;

    I2C0.ENABLE = TWIM_ENABLED;
}

int inline i2c_write(uint32_t address, uint8_t* buffer, uint8_t size_buffer)
{
    I2C0.EVENTS_LASTTX = 0;
    I2C0.EVENTS_ERROR = 0;

    I2C0.ADDRESS = address;

    I2C0.TXD_PTR = (unsigned int)buffer;
    I2C0.TXD_MAXCNT = size_buffer;

    I2C0.TASKS_STARTTX = 1;

    while (I2C0.EVENTS_LASTTX == 0)
        ;

    I2C0.EVENTS_STOPPED = 0;
    I2C0.TASKS_STOP = 1;

    while (I2C0.EVENTS_STOPPED == 0)
        ;

    if (I2C0.ERRORSRC != 0) {
        I2C0.ERRORSRC = 0;
        return -1;
    }

    return 0;
}

int inline i2c_read(uint32_t address, uint8_t* buffer, uint8_t size_buffer)
{
    I2C0.EVENTS_LASTRX = 0;
    I2C0.EVENTS_ERROR = 0;

    I2C0.ADDRESS = address;

    I2C0.RXD_PTR = (unsigned int)buffer;
    I2C0.RXD_MAXCNT = size_buffer;

    I2C0.TASKS_STARTRX = 1;

    while (I2C0.EVENTS_LASTRX == 0)
        ;

    I2C0.TASKS_STOP = 1;
    I2C0.EVENTS_STOPPED = 0;

    while (I2C0.EVENTS_STOPPED == 0)
        ;

    if (I2C0.ERRORSRC != 0) {
        I2C0.ERRORSRC = 0;
        return -1;
    }

    return 0;
}
