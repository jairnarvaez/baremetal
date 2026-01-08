#include <stdint.h>

extern uint32_t _stack;
extern uint32_t _etext;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

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

    uint8_t* pDst = (uint8_t*)&_sdata; // SRAM
    uint8_t* pSrc = (uint8_t*)&_etext; // Flash

    for (uint32_t i = 0; i < size; i++) {
        *pDst++ = *pSrc++;
    }

    // Init the .bss section to zero in SRAM
    size = (uint32_t)&_ebss - (uint32_t)&_sbss;

    pDst = (uint8_t*)&_sbss;
    for (uint32_t i = 0; i < size; i++) {
        *pDst++ = 0;
    }

    // Call main()
    main();

    while (1)
        ;
}
