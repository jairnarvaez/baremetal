#include "timer.h"
#include "nvic.h"

void timer_init()
{
    TIMER0.TASKS_STOP = 1;
    TIMER0.MODE = 0;
    TIMER0.BITMODE = 3;
    TIMER0.PRESCALER = 4;

    TIMER0.CC[0] = 1000000; // 1000000 ticks = 1s

    TIMER0.SHORTS = 1;

    TIMER0.INTENSET = 1 << 16;

    NVIC_EnableIRQ(8);

    TIMER0.TASKS_START = 1;
}
