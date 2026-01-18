#include "stdint.h"

#define NVIC_ISER0 *((volatile uint32_t*)0xE000E100)

#define NVIC_EnableIRQ(irq) (NVIC_ISER0 = 1 << irq)
