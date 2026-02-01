#ifndef GPIOE_H
#define GPIOE_H

#include "uart.h"
#include <stdint.h>

#define BASE 0x40006000

#define GPIOE (*(volatile struct _gpioe*)BASE)

struct _gpioe {
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
    GPIOE_CONFIG_MODE = 0,
    GPIOE_CONFIG_PSEL = 8,
    GPIOE_CONFIG_PORT = 13,
    GPIOE_CONFIG_POLARITY = 16,
    GPIOE_CONFIG_OUTINIT = 20,
} GPIOE_Config_t;

typedef enum {
    GPIOE_MODE_DISABLED = 0,
    GPIOE_MODE_EVENT = 1,
    GPIOE_MODE_TASK = 3
} Config_Mode_t;

typedef enum {
    GPIOE_POLARITY_NONE = 0,
    GPIOE_POLARITY_LOTOHI = 1,
    GPIOE_POLARITY_HITOLO = 2,
    GPIOE_POLARITY_TOGGLE = 3
} GPIOE_Polarity_t;

typedef enum {
    GPIOE_OUTINIT_LOW,
    GPIOE_OUTINIT_HIGH,
} GPIOE_Outinit_t;

typedef enum {
    GPIOE_PORT_0,
    GPIOE_PORT_1,
} GPIOE_Port_t;

typedef enum {
    GPIOE_CHANNEL_0,
    GPIOE_CHANNEL_1,
    GPIOE_CHANNEL_2,
    GPIOE_CHANNEL_3,
    GPIOE_CHANNEL_4,
    GPIOE_CHANNEL_5,
    GPIOE_CHANNEL_6,
    GPIOE_CHANNEL_7,
} GPIOE_Channel_t;

#define GPIOTE_CONFIG_PACK(mode, psel, port, polarity, outinit) \
    (((mode) << GPIOE_CONFIG_MODE) | ((psel) << GPIOE_CONFIG_PSEL) | ((port) << GPIOE_CONFIG_PORT) | ((polarity) << GPIOE_CONFIG_POLARITY) | ((outinit) << GPIOE_CONFIG_OUTINIT))

#define GPIOE_MAX_CHANNELS 8

static inline void GPIOTE_ConfigTask(volatile struct _gpioe* gpiote, GPIOE_Channel_t channel,
    uint32_t pin, uint32_t port, GPIOE_Outinit_t initial_value)
{
    gpiote->CONFIG[channel] = GPIOTE_CONFIG_PACK(
        GPIOE_MODE_TASK,
        pin,
        port,
        GPIOE_POLARITY_NONE,
        initial_value);
}

static inline void GPIOTE_ConfigEvent(volatile struct _gpioe* gpiote, GPIOE_Channel_t channel,
    uint32_t pin, uint32_t port, GPIOE_Polarity_t polarity)
{
    gpiote->CONFIG[channel] = GPIOTE_CONFIG_PACK(
        GPIOE_MODE_EVENT,
        pin,
        port,
        polarity,
        GPIOE_OUTINIT_LOW);
}

static inline void GPIOTE_Disable(volatile struct _gpioe* gpiote, GPIOE_Channel_t channel)
{
    gpiote->CONFIG[channel] = GPIOTE_CONFIG_PACK(
        GPIOE_MODE_DISABLED,
        0, 0, 0, 0);
}

static inline void GPIOTE_EnableInterrupt(volatile struct _gpioe* gpiote, GPIOE_Channel_t channel)
{
    if (channel < GPIOE_MAX_CHANNELS) {
        gpiote->INTENSET = (1 << channel);
    }
}

static inline void GPIOTE_DisableInterrupt(volatile struct _gpioe* gpiote, GPIOE_Channel_t channel)
{
    if (channel < GPIOE_MAX_CHANNELS) {
        gpiote->INTENCLR = (1 << channel);
    }
}

static inline void GPIOTE_TaskSet(volatile struct _gpioe* gpiote, GPIOE_Channel_t channel)
{
    if (channel < GPIOE_MAX_CHANNELS) {
        gpiote->TASK_SET[channel] = 1;
    }
}

static inline void GPIOTE_TaskClear(volatile struct _gpioe* gpiote, GPIOE_Channel_t channel)
{
    if (channel < GPIOE_MAX_CHANNELS) {
        gpiote->TASK_CLR[channel] = 1;
    }
}

static inline void GPIOTE_TaskOut(volatile struct _gpioe* gpiote, GPIOE_Channel_t channel)
{
    if (channel < GPIOE_MAX_CHANNELS) {
        gpiote->TASK_OUT[channel] = 1;
    }
}

void GPIOE_IRQHandler(void);

#endif
