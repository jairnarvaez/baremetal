#include "gpiote.h"

#define WEAK __attribute__((weak))

WEAK void GPIOTE_Channel0_Callback(void) { }
WEAK void GPIOTE_Channel1_Callback(void) { }
WEAK void GPIOTE_Channel2_Callback(void) { }
WEAK void GPIOTE_Channel3_Callback(void) { }
WEAK void GPIOTE_Channel4_Callback(void) { }
WEAK void GPIOTE_Channel5_Callback(void) { }
WEAK void GPIOTE_Channel6_Callback(void) { }
WEAK void GPIOTE_Channel7_Callback(void) { }
WEAK void GPIOTE_Port_Callback(void) { }

void GPIOET_IRQHandler(void)
{
    static void (*const callbacks[8])(void) = {
        GPIOTE_Channel0_Callback,
        GPIOTE_Channel1_Callback,
        GPIOTE_Channel2_Callback,
        GPIOTE_Channel3_Callback,
        GPIOTE_Channel4_Callback,
        GPIOTE_Channel5_Callback,
        GPIOTE_Channel6_Callback,
        GPIOTE_Channel7_Callback,
    };

    for (uint32_t i = 0; i < 8; i++) {
        if (GPIOET.EVENTS_IN[i]) {
            GPIOET.EVENTS_IN[i] = 0;
            callbacks[i]();
        }
    }

    if (GPIOET.EVENTS_PORT) {
        GPIOET.EVENTS_PORT = 0;
        GPIOTE_Port_Callback();
    }
}
