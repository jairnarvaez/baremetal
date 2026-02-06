#ifndef RESET_H
#define RESET_H

#include "stdint.h"

#define SCB_AIRCR (*(volatile uint32_t*)0xE000ED0C)
#define AIRCR_VECTKEY (0x05FA << 16)
#define AIRCR_SYSRESETREQ (1 << 2)

void baremetal_reset(void);

#endif
