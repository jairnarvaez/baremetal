#include <stdint.h>

#include "delay.h"
#include "gpio.h"

void error_blink();
void show_row(int row, uint8_t pattern);
void show(const uint8_t* row);

void blink_main();
void timer0_irqhandler(void);
