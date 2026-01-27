#include "display.h"
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
void timer0_irqhandler(void);

void receive_rx_irq(void)
{
    error_blink();
}

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
    [18] = (uint32_t)&receive_rx_irq,
    [19 ... 23] = 0,
    [24] = (uint32_t)&timer0_irqhandler
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
