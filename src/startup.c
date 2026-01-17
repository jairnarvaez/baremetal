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

__attribute__((section(".isr_vector"))) uint32_t vectors[] = {
    (uint32_t)&_stack,
    (uint32_t)&reset_handler,
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
