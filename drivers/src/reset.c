#include "reset.h"

void baremetal_reset(void)
{
    __asm volatile("dsb");

    SCB_AIRCR = AIRCR_VECTKEY | AIRCR_SYSRESETREQ;

    __asm volatile("isb");
    while (1)
        ;
}
