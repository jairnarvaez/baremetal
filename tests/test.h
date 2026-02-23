#ifndef TEST_DELAY_H
#define TEST_DELAY_H

#include <stdint.h>

void test_delay(uint32_t interval_us);
void test_echo();
void test_buttons(void);
int test_lsm303(void);
void test_ppi();
void test_uart(void);

#endif
