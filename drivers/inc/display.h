#include <stdint.h>

#include "delay.h"
#include "gpio.h"

void error_blink();
void display_row(int row, uint8_t pattern);
void display(const uint8_t* row);
void display_clear();

void blink_main();
void timer0_irqhandler(void);
