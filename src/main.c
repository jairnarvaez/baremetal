#include "power.h"
#include "test_delay.h"
#include "test_gpiote.h"
#include "test_lsm303.h"
#include "test_uart.h"

#define TOTAL_TESTS 4
#define DEFAULT_TEST 1

/* GPREGRET: General Purpose Register Retained - survives soft resets
Used to track which test was last executed and cycle through tests on each
reset Address 0x4000051C */

static uint32_t get_next_test_id(void)
{
    uint32_t test_id = POWER.GPREGRET;

    if (test_id < 1 || test_id > TOTAL_TESTS) {
        test_id = DEFAULT_TEST;
    } else {
        test_id = (test_id % TOTAL_TESTS) + 1;
    }

    POWER.GPREGRET = test_id;

    return test_id;
}

int main(void)
{
    uint32_t current_test = get_next_test_id();

    switch (current_test) {
    case 1:
        test_uart();
        break;
    case 2:
        test_buttons();
        break;
    case 3:
        test_delay(1000000);
        break;
    case 4:
        test_lsm303();
        break;
    default:
        break;
    }

    for (;;) {
        __asm("WFE");
    }

    return 0;
}
