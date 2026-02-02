#include "error.h"
#include <stdint.h>

extern uint32_t _stack;

extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

extern uint32_t __xram_start;
extern uint32_t __xram_end;

extern uint32_t _sidata;
extern uint32_t _sixram;

int main(void);
void reset_handler(void);
void TIMER0_irqhandler(void);
void TIMER1_irqhandler(void) { }
void TIMER2_irqhandler(void) { }
void TIMER3_irqhandler(void) { }
void TIMER4_irqhandler(void) { }
void UARTE0_IRQHandler(void);
void GPIOET_IRQHandler(void);

void default_handler(void)
{
    error_blink(); // Loop infinito si hay error
}

__attribute__((section(".isr_vector"))) uint32_t vectors[] = {
    (uint32_t)&_stack,
    (uint32_t)&reset_handler,
    (uint32_t)&default_handler,
    (uint32_t)&default_handler,
    (uint32_t)&default_handler,
    (uint32_t)&default_handler,
    (uint32_t)&default_handler,
    0,
    0,
    0,
    0,
    (uint32_t)&default_handler,
    (uint32_t)&default_handler,
    0,
    (uint32_t)&default_handler,
    (uint32_t)&default_handler,

    [16 ... 17] = 0,
    [18] = (uint32_t)&UARTE0_IRQHandler,
    [19 ... 21] = 0,
    [22] = (uint32_t)&GPIOET_IRQHandler,
    [23] = 0,
    [24] = (uint32_t)&TIMER0_irqhandler,
    [25] = (uint32_t)&TIMER1_irqhandler,
    [26] = (uint32_t)&TIMER2_irqhandler,
    [27 ... 41] = 0,
    [42] = (uint32_t)&TIMER3_irqhandler,
    [43] = (uint32_t)&TIMER4_irqhandler,
};

void reset_handler(void)
{
    // Copy .data to SRAM
    uint32_t size = &_edata - &_sdata;

    uint32_t* pSrc = &_sidata; // Flash
    uint32_t* pDst = &_sdata; // SRAM

    for (uint32_t i = 0; i < size; i++) {
        *pDst++ = *pSrc++;
    }

    // Copy .xram to RAM
    size = &__xram_end - &__xram_start;

    pSrc = &_sixram;
    pDst = &__xram_start;

    for (uint32_t i = 0; i < size; i++) {
        *pDst++ = *pSrc++;
    }

    // Init the .bss section to zero in SRAM
    size = &_ebss - &_sbss;

    pDst = &_sbss;

    for (uint32_t i = 0; i < size; i++) {
        *pDst++ = 0;
    }

    // Call main()
    main();

    while (1)
        ;
}
