#ifndef I2C_H
#define I2C_H

#include <sys/types.h>
#define BASE0 0x40003000
#define BASE1 0x40004000

#define I2C0 (*(volatile struct _twim*)BASE0)
#define I2C1 (*(volatile struct _twim*)BASE1)

#define PIN_SCL 8
#define PIN_SDA 16

struct _twim {
    unsigned TASKS_STARTRX; // 0x000
    char _pad1[0x004]; // 0x004 - 0x007
    unsigned TASKS_STARTTX; // 0x008
    char _pad2[0x008]; // 0x00C - 0x013
    unsigned TASKS_STOP; // 0x014
    char _pad3[0x004]; // 0x018 - 0x01B
    unsigned TASKS_SUSPEND; // 0x01C
    unsigned TASKS_RESUME; // 0x020
    char _pad4[0x0E0]; // 0x024 - 0x103
    unsigned EVENTS_STOPPED; // 0x104
    char _pad5[0x01C]; // 0x108 - 0x123
    unsigned EVENTS_ERROR; // 0x124
    char _pad6[0x020]; // 0x128 - 0x147
    unsigned EVENTS_SUSPENDED; // 0x148
    unsigned EVENTS_RXSTARTED; // 0x14C
    unsigned EVENTS_TXSTARTED; // 0x150
    char _pad7[0x008]; // 0x154 - 0x15B
    unsigned EVENTS_LASTRX; // 0x15C
    unsigned EVENTS_LASTTX; // 0x160
    char _pad8[0x09C]; // 0x164 - 0x1FF
    unsigned SHORTS; // 0x200
    char _pad9[0x0FC]; // 0x204 - 0x2FF
    unsigned INTEN; // 0x300
    unsigned INTENSET; // 0x304
    unsigned INTENCLR; // 0x308
    char _pad10[0x1B8]; // 0x30C - 0x4C3
    unsigned ERRORSRC; // 0x4C4
    char _pad11[0x038]; // 0x4C8 - 0x4FF
    unsigned ENABLE; // 0x500
    char _pad12[0x004]; // 0x504 - 0x507
    unsigned PSEL_SCL; // 0x508
    unsigned PSEL_SDA; // 0x50C
    char _pad13[0x014]; // 0x510 - 0x523
    unsigned FREQUENCY; // 0x524
    char _pad14[0x00C]; // 0x528 - 0x533
    unsigned RXD_PTR; // 0x534
    unsigned RXD_MAXCNT; // 0x538
    unsigned RXD_AMOUNT; // 0x53C
    unsigned RXD_LIST; // 0x540
    unsigned TXD_PTR; // 0x544
    unsigned TXD_MAXCNT; // 0x548
    unsigned TXD_AMOUNT; // 0x54C
    unsigned TXD_LIST; // 0x550
    char _pad15[0x034]; // 0x554 - 0x587
    unsigned ADDRESS; // 0x588
};

typedef enum {
    TWIM_FREQ_100KBPS = 0x01980000,
    TWIM_FREQ_250KBPS = 0x04000000,
    TWIM_FREQ_400KBPS = 0x06400000,
} Twim_Frequency_t;

typedef enum {
    TWIM_DISABLED = 0,
    TWIM_ENABLED = 6,
} Twim_Enable_t;

void i2c_init(Twim_Frequency_t frequency);
int i2c_write(uint32_t address, uint8_t* buffer, uint8_t size);
int i2c_read(uint32_t address, uint8_t* buffer, uint8_t size);

#endif
