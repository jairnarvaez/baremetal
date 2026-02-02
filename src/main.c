#include "test_buttons.h"
#include "test_delay.h"
#include "test_uart.h"

#define SELECTED_TEST 2

#define TEST_UART 1
#define TEST_BUTTONS 2
#define TEST_DELAY 3

int main(void)
{
#if SELECTED_TEST == TEST_UART
    test_uart();

#elif SELECTED_TEST == TEST_BUTTONS
    test_buttons();

#elif SELECTED_TEST == TEST_DELAY
    uint32_t time_us = 1000000;
    test_delay(time_us);

#else
#error "No test selected or invalid test number"
#endif

    for (;;) {
    }

    return 0;
}
