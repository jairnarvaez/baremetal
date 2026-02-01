#include <stdint.h>

#include "delay.h"
#include "gpio.h"

#define REFRESH_RATE_HZ 60

#define LED_MASK0 0xd1688800
#define LED_MASK1 0x00000020

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

void error_blink();
void display_row(int row, uint8_t pattern);
void display(const uint8_t* row);
void display_clear();
void display_init(uint32_t display_fps);
void display_set_pixel(uint8_t x, uint8_t y, uint8_t state);
void display_shift_right();
void display_shift_left();

void timer0_irqhandler(void);
