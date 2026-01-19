#include <stdint.h>

#include "delay.h"
#include "gpio.h"

#define REFRESH_RATE_HZ 60

void error_blink();
void display_row(int row, uint8_t pattern);
void display(const uint8_t* row);
void display_clear();
void display_init(uint32_t display_fps);
void display_set_pixel(uint8_t x, uint8_t y, uint8_t state);
void display_shift_right();
void display_shift_left();

void timer0_irqhandler(void);
