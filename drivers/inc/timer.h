#ifndef TIMER_H
#define TIMER_H

#include "nvic.h"
#include <stdint.h>

#define BASETIMER0 0x40008000
#define BASETIMER1 0x40009000
#define BASETIMER2 0x4000A000
#define BASETIMER3 0x4001A000
#define BASETIMER4 0x4001B000

#define TIMER0 (*(volatile struct _timer*)BASETIMER0)
#define TIMER1 (*(volatile struct _timer*)BASETIMER1)
#define TIMER2 (*(volatile struct _timer*)BASETIMER2)
#define TIMER3 (*(volatile struct _timer*)BASETIMER3)
#define TIMER4 (*(volatile struct _timer*)BASETIMER4)

#define TIMER0_IRQn 8
#define TIMER1_IRQn 9
#define TIMER2_IRQn 10
#define TIMER3_IRQn 26
#define TIMER4_IRQn 27

struct _timer {
    unsigned TASKS_START; // 0x000
    unsigned TASKS_STOP; // 0x004
    unsigned TASKS_COUNT; // 0x008
    unsigned TASKS_CLEAR; // 0x00C
    unsigned TASKS_SHUTDOWN; // 0x010
    char _pad1[44];
    unsigned TASKS_CAPTURE[6]; // 0x040
    char _pad2[232];
    unsigned EVENTS_COMPARE[6]; // 0x140
    char _pad3[168];
    unsigned SHORTS; // 0x200
    char _pad4[256];
    unsigned INTENSET; // 0x304
    unsigned INTENCLR; // 0x308
    char _pad5[504];
    unsigned MODE; // 0x504
    unsigned BITMODE; // 0x508
    char _pad6[4];
    unsigned PRESCALER; // 0x510
    char _pad7[44];
    unsigned CC[6]; // 0x540
};

typedef enum {
    TIMER_MODE_TIMER = 0,
    TIMER_MODE_COUNTER = 1,
    TIMER_MODE_LOW_POWER_COUNTER = 2
} timer_mode_t;

typedef enum {
    TIMER_BITMODE_16BIT = 0,
    TIMER_BITMODE_08BIT = 1,
    TIMER_BITMODE_24BIT = 2,
    TIMER_BITMODE_32BIT = 3
} timer_bitmode_t;

typedef enum {
    TIMER_PRESCALER_1MHz = 4, // 1 tick = 1 µs
    TIMER_PRESCALER_500kHz = 5, // 1 tick = 2 µs
    TIMER_PRESCALER_250kHz = 6, // 1 tick = 4 µs
    TIMER_PRESCALER_125kHz = 7, // 1 tick = 8 µs
    TIMER_PRESCALER_62500Hz = 8, // 1 tick = 16 µs
    TIMER_PRESCALER_31250Hz = 9, // 1 tick = 32 µs
} timer_prescaler_t;

typedef enum {
    TIMER_INT_COMPARE0 = 16,
    TIMER_INT_COMPARE1 = 17,
    TIMER_INT_COMPARE2 = 18,
    TIMER_INT_COMPARE3 = 19,
    TIMER_INT_COMPARE4 = 20,
    TIMER_INT_COMPARE5 = 21,
} timer_interrupt_t;

typedef enum {
    TIMER_SHORT_COMPARE0_CLEAR = 0,
    TIMER_SHORT_COMPARE1_CLEAR = 1,
    TIMER_SHORT_COMPARE2_CLEAR = 2,
    TIMER_SHORT_COMPARE3_CLEAR = 3,
    TIMER_SHORT_COMPARE4_CLEAR = 4,
    TIMER_SHORT_COMPARE5_CLEAR = 5,
    TIMER_SHORT_COMPARE0_STOP = 8,
    TIMER_SHORT_COMPARE1_STOP = 9,
    TIMER_SHORT_COMPARE2_STOP = 10,
    TIMER_SHORT_COMPARE3_STOP = 11,
    TIMER_SHORT_COMPARE4_STOP = 12,
    TIMER_SHORT_COMPARE5_STOP = 13,
} timer_shortcut_t;

typedef enum {
    TIMER_CHANNEL_0,
    TIMER_CHANNEL_1,
    TIMER_CHANNEL_2,
    TIMER_CHANNEL_3,
    TIMER_CHANNEL_4,
    TIMER_CHANNEL_5,
} timer_channel_t;

static inline void TIMER_Start(volatile struct _timer* timer)
{
    timer->TASKS_START = 1;
}

static inline void TIMER_Stop(volatile struct _timer* timer)
{
    timer->TASKS_STOP = 1;
}

static inline void TIMER_Clear(volatile struct _timer* timer)
{
    timer->TASKS_CLEAR = 1;
}

static inline void TIMER_Shutdown(volatile struct _timer* timer)
{
    timer->TASKS_SHUTDOWN = 1;
}

static inline void TIMER_SetMode(volatile struct _timer* timer,
    timer_mode_t mode)
{
    timer->MODE = mode;
}

static inline void TIMER_SetBitMode(volatile struct _timer* timer,
    timer_bitmode_t bitmode)
{
    timer->BITMODE = bitmode;
}

static inline void TIMER_SetPrescaler(volatile struct _timer* timer,
    timer_prescaler_t prescaler)
{
    timer->PRESCALER = prescaler & 0xF;
}

static inline void TIMER_SetCompare(volatile struct _timer* timer,
    timer_channel_t channel,
    uint32_t value)
{
    if (channel < 6) {
        timer->CC[channel] = value;
    }
}

static inline uint32_t TIMER_GetCounter(volatile struct _timer* timer,
    timer_channel_t channel)
{
    if (channel < 6) {
        timer->TASKS_CAPTURE[channel] = 1;
        return timer->CC[channel];
    }
    return 0;
}

static inline void TIMER_EnableShortcut(volatile struct _timer* timer,
    timer_shortcut_t shortcut)
{
    timer->SHORTS |= (1 << shortcut);
}

static inline void TIMER_DisableShortcut(volatile struct _timer* timer,
    timer_shortcut_t shortcut)
{
    timer->SHORTS &= ~(1 << shortcut);
}

static inline void TIMER_EnableInterrupt(volatile struct _timer* timer,
    timer_interrupt_t interrupt)
{
    timer->INTENSET = (1 << interrupt);
}

static inline void TIMER_DisableInterrupt(volatile struct _timer* timer,
    timer_interrupt_t interrupt)
{
    timer->INTENCLR = (1 << interrupt);
}

static inline void TIMER_ClearEvent(volatile struct _timer* timer,
    timer_channel_t channel)
{
    if (channel < 6) {
        timer->EVENTS_COMPARE[channel] = 0;
    }
}

static inline uint8_t TIMER_EventPending(volatile struct _timer* timer,
    timer_channel_t channel)
{
    if (channel < 6) {
        return (timer->EVENTS_COMPARE[channel] != 0);
    }
    return 0;
}

static inline void TIMER_InitInterval(volatile struct _timer* timer,
    uint32_t interval_us)
{
    TIMER_Stop(timer);
    TIMER_Clear(timer);
    TIMER_SetMode(timer, TIMER_MODE_TIMER);
    TIMER_SetBitMode(timer, TIMER_BITMODE_32BIT);
    TIMER_SetPrescaler(timer, TIMER_PRESCALER_1MHz); // 1 tick = 1 µs
    TIMER_SetCompare(timer, 0, interval_us);
    TIMER_EnableShortcut(timer, TIMER_SHORT_COMPARE0_CLEAR);
}

static inline void TIMER_InitFrequency(volatile struct _timer* timer,
    uint32_t frequency_hz)
{
    uint32_t interval_us = 1000000 / frequency_hz;
    TIMER_InitInterval(timer, interval_us);
}

static inline int8_t TIMER_GetIRQn(volatile struct _timer* timer)
{
    uint32_t base = (uint32_t)timer;
    switch (base) {
    case BASETIMER0:
        return TIMER0_IRQn;
    case BASETIMER1:
        return TIMER1_IRQn;
    case BASETIMER2:
        return TIMER2_IRQn;
    case BASETIMER3:
        return TIMER3_IRQn;
    case BASETIMER4:
        return TIMER4_IRQn;
    default:
        return -1;
    }
}

static inline void TIMER_RunPeriodic(volatile struct _timer* timer,
    uint32_t frequency_hz)
{
    TIMER_InitFrequency(timer, frequency_hz);
    TIMER_EnableInterrupt(timer, TIMER_INT_COMPARE0);

    int irq_num = TIMER_GetIRQn(timer);

    if (irq_num >= 0) {
        NVIC_EnableIRQ(irq_num);
    }

    TIMER_Start(timer);
}

static inline void TIMER_StopPeriodic(volatile struct _timer* timer)
{
    TIMER_Stop(timer);
    TIMER_DisableInterrupt(timer, TIMER_INT_COMPARE0);

    int irq_num = TIMER_GetIRQn(timer);
    if (irq_num >= 0) {
        NVIC_DisableIRQ(irq_num);
    }

    TIMER_ClearEvent(timer, 0);
    TIMER_Clear(timer);
}

static inline uint8_t TIMER_CheckAndClearEvent(volatile struct _timer* timer,
    timer_channel_t channel)
{
    if (channel < 6 && timer->EVENTS_COMPARE[channel]) {
        timer->EVENTS_COMPARE[channel] = 0;
        return 1;
    }
    return 0;
}

static inline void TIMER_Init(volatile struct _timer* timer,
    timer_mode_t mode,
    timer_bitmode_t bitmode,
    timer_prescaler_t prescaler,
    timer_channel_t channel,
    uint32_t cc_value)
{
    TIMER_Stop(timer);
    TIMER_Clear(timer);

    TIMER_SetMode(timer, mode);
    TIMER_SetBitMode(timer, bitmode);
    TIMER_SetPrescaler(timer, (uint32_t)prescaler);

    TIMER_SetCompare(timer, channel, cc_value);

    timer->SHORTS = (1 << channel);
}

#define TIMER_MS_TO_US(ms) ((ms) * 1000UL)
#define TIMER_SEC_TO_US(s) ((s) * 1000000UL)

__attribute__((weak)) void TIMER0_Compare0_Callback(void) { }
__attribute__((weak)) void TIMER1_Compare1_Callback(void) { }
__attribute__((weak)) void TIMER0_Compare2_Callback(void) { }
__attribute__((weak)) void TIMER1_Compare3_Callback(void) { }
__attribute__((weak)) void TIMER0_Compare4_Callback(void) { }

#endif
