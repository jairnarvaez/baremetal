#ifndef NVIC_H
#define NVIC_H

#include "stdint.h"

#define NVIC_ISER0 *((volatile uint32_t*)0xE000E100)
#define NVIC_ICER0 *((volatile uint32_t*)0xE000E180)

#define NVIC_EnableIRQ(irq) (NVIC_ISER0 = 1 << irq)
#define NVIC_DisableIRQ(irq) (NVIC_ICER0 = 1 << irq)

#endif
