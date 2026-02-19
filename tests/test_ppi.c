#include "test_ppi.h"
#include "display.h"
#include "gpiote.h"
#include "ppi.h"
#include "timer.h"

void test_ppi()
{
    TIMER_InitInterval(&TIMER0, 100000);
    TIMER_Start(&TIMER0);

    GPIOTE_ConfigTask(&GPIOET, GPIOET_CHANNEL_0, LED_ROW3, 0, 1);
    GPIOTE_ConfigTask(&GPIOET, GPIOET_CHANNEL_1, LED_COL3, 0, 0);

    PPI.CH[0].EEP = (unsigned)&TIMER0.EVENTS_COMPARE[0];
    PPI.CH[0].TEP = (unsigned)&GPIOET.TASK_OUT[0];

    PPI.CHENSET = (1 << 0);

    for (;;) {
        __asm("WFE");
    }
}
