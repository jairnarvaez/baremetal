#ifndef DISPLAY_H
#define DISPLAY_H

#include "gpio.h"
#include <stdint.h>

#define DISPLAY_ROWS 5
#define DISPLAY_COLS 5

#define DISPLAY_DEFAULT_FPS 60

typedef enum {
    LED_ROW1 = 21,
    LED_ROW2 = 22,
    LED_ROW3 = 15,
    LED_ROW4 = 24,
    LED_ROW5 = 19,
    LED_COL1 = 28,
    LED_COL2 = 11,
    LED_COL3 = 31,
    LED_COL4 = 5,
    LED_COL5 = 30,
} GPIO_LedMatrix_t;

void display_init(uint32_t refresh_rate_hz);
void display(const uint8_t* row);
void display_clear(void);
void display_set_pixel(uint8_t x, uint8_t y, uint8_t state);
void display_shift_right(void);
void display_shift_left(void);

#endif
