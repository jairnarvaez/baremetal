#ifndef GPIOET_H
#define GPIOET_H

#include <stdint.h>

#define BASE 0x40006000

#define GPIOET (*(volatile struct _gpiote*)BASE)

struct _gpiote {
    unsigned TASK_OUT[8]; // 0x000-0x01C
    char _pad1[0x030 - 0x020];
    unsigned TASK_SET[8]; // 0x030-0x04C
    char _pad2[0x060 - 0x050];
    unsigned TASK_CLR[8]; // 0x060-0x07C
    char _pad3[0x100 - 0x080];
    unsigned EVENTS_IN[8]; // 0x100-0x11C
    char _pad4[0x17C - 0x120];
    unsigned EVENTS_PORT; // 0x17C
    char _pad5[0x304 - 0x180];
    unsigned INTENSET; // 0x304
    unsigned INTENCLR; // 0x308
    char _pad6[0x510 - 0x30C];
    unsigned CONFIG[8]; // 0x510-0x52C
};

typedef enum {
    GPIOET_CONFIG_MODE = 0,
    GPIOET_CONFIG_PSEL = 8,
    GPIOET_CONFIG_PORT = 13,
    GPIOET_CONFIG_POLARITY = 16,
    GPIOET_CONFIG_OUTINIT = 20,
} GPIOET_Config_t;

typedef enum {
    GPIOET_MODE_DISABLED = 0,
    GPIOET_MODE_EVENT = 1,
    GPIOET_MODE_TASK = 3
} Config_Mode_t;

typedef enum {
    GPIOET_POLARITY_NONE = 0,
    GPIOET_POLARITY_LOTOHI = 1,
    GPIOET_POLARITY_HITOLO = 2,
    GPIOET_POLARITY_TOGGLE = 3
} GPIOET_Polarity_t;

typedef enum {
    GPIOET_OUTINIT_LOW,
    GPIOET_OUTINIT_HIGH,
} GPIOET_Outinit_t;

typedef enum {
    GPIOET_PORT_0,
    GPIOET_PORT_1,
} GPIOET_Port_t;

typedef enum {
    GPIOET_CHANNEL_0,
    GPIOET_CHANNEL_1,
    GPIOET_CHANNEL_2,
    GPIOET_CHANNEL_3,
    GPIOET_CHANNEL_4,
    GPIOET_CHANNEL_5,
    GPIOET_CHANNEL_6,
    GPIOET_CHANNEL_7,
} GPIOET_Channel_t;

#define GPIOTE_CONFIG_PACK(mode, psel, port, polarity, outinit) \
    (((mode) << GPIOET_CONFIG_MODE) | ((psel) << GPIOET_CONFIG_PSEL) | ((port) << GPIOET_CONFIG_PORT) | ((polarity) << GPIOET_CONFIG_POLARITY) | ((outinit) << GPIOET_CONFIG_OUTINIT))

#define GPIOET_MAX_CHANNELS 8

static inline void GPIOTE_ConfigTask(volatile struct _gpiote* gpiote, GPIOET_Channel_t channel,
    uint32_t pin, uint32_t port, GPIOET_Outinit_t initial_value)
{
    gpiote->CONFIG[channel] = GPIOTE_CONFIG_PACK(
        GPIOET_MODE_TASK,
        pin,
        port,
        GPIOET_POLARITY_NONE,
        initial_value);
}

static inline void GPIOTE_ConfigEvent(volatile struct _gpiote* gpiote, GPIOET_Channel_t channel,
    uint32_t pin, uint32_t port, GPIOET_Polarity_t polarity)
{
    gpiote->CONFIG[channel] = GPIOTE_CONFIG_PACK(
        GPIOET_MODE_EVENT,
        pin,
        port,
        polarity,
        GPIOET_OUTINIT_LOW);
}

static inline void GPIOTE_Disable(volatile struct _gpiote* gpiote, GPIOET_Channel_t channel)
{
    gpiote->CONFIG[channel] = GPIOTE_CONFIG_PACK(
        GPIOET_MODE_DISABLED,
        0, 0, 0, 0);
}

static inline void GPIOTE_EnableInterrupt(volatile struct _gpiote* gpiote, GPIOET_Channel_t channel)
{
    if (channel < GPIOET_MAX_CHANNELS) {
        gpiote->INTENSET = (1 << channel);
    }
}

static inline void GPIOTE_DisableInterrupt(volatile struct _gpiote* gpiote, GPIOET_Channel_t channel)
{
    if (channel < GPIOET_MAX_CHANNELS) {
        gpiote->INTENCLR = (1 << channel);
    }
}

static inline void GPIOTE_TaskSet(volatile struct _gpiote* gpiote, GPIOET_Channel_t channel)
{
    if (channel < GPIOET_MAX_CHANNELS) {
        gpiote->TASK_SET[channel] = 1;
    }
}

static inline void GPIOTE_TaskClear(volatile struct _gpiote* gpiote, GPIOET_Channel_t channel)
{
    if (channel < GPIOET_MAX_CHANNELS) {
        gpiote->TASK_CLR[channel] = 1;
    }
}

static inline void GPIOTE_TaskOut(volatile struct _gpiote* gpiote, GPIOET_Channel_t channel)
{
    if (channel < GPIOET_MAX_CHANNELS) {
        gpiote->TASK_OUT[channel] = 1;
    }
}

void GPIOET_IRQHandler(void);

#endif
