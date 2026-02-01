#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

#define BASE0 0x50000500
#define BASE1 0x50000800

#define LED_MASK0 0xd1688800
#define LED_MASK1 0x00000020

#define GPIO0 (*(volatile struct _gpio*)BASE0)
#define GPIO1 (*(volatile struct _gpio*)BASE1)

struct _gpio {
    char _pad1[4]; // 0x500
    unsigned OUT; // 0x504
    unsigned OUTSET; // 0x508
    unsigned OUTCLR; // 0x50C
    unsigned IN; // 0x510
    unsigned DIR; // 0x514
    unsigned DIRSET; // 0x518
    unsigned DIRCLR; // 0x51C
    char _pad3[480]; // 0x520
    unsigned PIN_CNF[32]; // 0x700
};

typedef enum {
    LED_ROW1 = 21,
    LED_ROW2 = 22,
    LED_ROW3 = 15,
    LED_ROW4 = 24,
    LED_ROW5 = 19,

    LED_COL1 = 28,
    LED_COL2 = 11,
    LED_COL3 = 31,
    LED_COL4 = 5,
    LED_COL5 = 30,
} GPIO_LedMatrix_t;

typedef enum {
    GPIO_DIR_INPUT = 0,
    GPIO_DIR_OUTPUT = 1
} GPIO_Dir_t;

typedef enum {
    GPIO_INPUT_CONNECT = 0,
    GPIO_INPUT_DISCONNECT = 1
} GPIO_Input_t;

typedef enum {
    GPIO_PULL_DISABLED = 0,
    GPIO_PULL_DOWN = 1,
    GPIO_PULL_UP = 3
} GPIO_Pull_t;

typedef enum {
    GPIO_DRIVE_S0S1 = 0,
    GPIO_DRIVE_H0S1 = 1,
    GPIO_DRIVE_S0H1 = 2,
    GPIO_DRIVE_H0H1 = 3,
    GPIO_DRIVE_D0S1 = 4,
    GPIO_DRIVE_D0H1 = 5,
    GPIO_DRIVE_S0D1 = 6,
    GPIO_DRIVE_H0D1 = 7
} GPIO_Drive_t;

typedef enum {
    GPIO_SENSE_DISABLED = 0,
    GPIO_SENSE_HIGH = 2,
    GPIO_SENSE_LOW = 3
} GPIO_Sense_t;

typedef enum {
    GPIO_CNF_POS_DIR = 0,
    GPIO_CNF_POS_INPUT = 1,
    GPIO_CNF_POS_PULL = 2,
    GPIO_CNF_POS_DRIVE = 8,
    GPIO_CNF_POS_SENSE = 16
} GPIO_CnfPos_t;

typedef enum {
    GPIO_PIN_LOW = 0,
    GPIO_PIN_HIGH = 1
} GPIO_PinState_t;

#define SET_BIT(p) (1 << (p))

#define MATRIX_ROW_MASK (SET_BIT(LED_ROW1) | SET_BIT(LED_ROW2) | SET_BIT(LED_ROW3) | SET_BIT(LED_ROW4) | SET_BIT(LED_ROW5))

#define MATRIX_COL_PORT0_MASK (SET_BIT(LED_COL1) | SET_BIT(LED_COL2) | SET_BIT(LED_COL3) | SET_BIT(LED_COL5))

#define MATRIX_COL_PORT1_MASK SET_BIT(LED_COL4)

#define GPIO_PIN_CNF_PACK(dir, input, pull, drive, sense) \
    (((dir) << GPIO_CNF_POS_DIR) | ((input) << GPIO_CNF_POS_INPUT) | ((pull) << GPIO_CNF_POS_PULL) | ((drive) << GPIO_CNF_POS_DRIVE) | ((sense) << GPIO_CNF_POS_SENSE))

static inline void GPIO_Output(volatile struct _gpio* port, uint32_t pin)
{
    port->PIN_CNF[pin] = GPIO_PIN_CNF_PACK(
        GPIO_DIR_OUTPUT,
        GPIO_INPUT_DISCONNECT,
        GPIO_PULL_DISABLED,
        GPIO_DRIVE_S0S1,
        GPIO_SENSE_DISABLED);
}

static inline void GPIO_Input(volatile struct _gpio* port, uint32_t pin, GPIO_Pull_t pull)
{
    port->PIN_CNF[pin] = GPIO_PIN_CNF_PACK(
        GPIO_DIR_INPUT,
        GPIO_INPUT_CONNECT,
        pull,
        GPIO_DRIVE_S0S1,
        GPIO_SENSE_DISABLED);
}

static inline void GPIO_Write(volatile struct _gpio* port, uint32_t pin, GPIO_PinState_t state)
{
    if (state == GPIO_PIN_HIGH) {
        port->OUTSET = SET_BIT(pin);
    } else {
        port->OUTCLR = SET_BIT(pin);
    }
}

static inline uint32_t GPIO_Read(volatile struct _gpio* port, uint32_t pin)
{
    return (port->IN >> pin) & 1;
}

static inline void GPIO_Toggle(volatile struct _gpio* port, uint32_t pin)
{
    port->OUT ^= SET_BIT(pin);
}

static inline void GPIO_Config_Mask(volatile struct _gpio* port, uint32_t mask, uint32_t config)
{
    for (uint32_t pin = 0; pin < 32; pin++) {
        if (mask & (1UL << pin)) {
            port->PIN_CNF[pin] = config;
        }
    }
}

#endif
