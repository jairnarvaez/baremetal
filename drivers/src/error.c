#include "error.h"

static uint8_t ERROR[] = {
    0b11111,
    0b10000,
    0b11100,
    0b10000,
    0b11111
};

void error_blink()
{
    GPIO0.DIR = LED_MASK0;
    GPIO1.DIR = LED_MASK1;
    while (1) {
        display(ERROR);
    }
}
