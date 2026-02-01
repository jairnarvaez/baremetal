#ifndef TIMER_H
#define TIMER_H

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

void timer_init(uint32_t interval_us);

#endif
