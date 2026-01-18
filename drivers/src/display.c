#include "display.h"
#include "timer.h"
#include "uart.h"
#include "utils.h"
#include <stdint.h>

const uint8_t ROWS[] = { ROW1, ROW2, ROW3, ROW4, ROW5 };
const uint8_t COLS[] = { COL1, COL2, COL3, COL4, COL5 };

uint8_t currentRow = 0;
uint8_t buffer_row_value[5];

uint8_t ERROR[] = {
    0b11111,
    0b10000,
    0b11100,
    0b10000,
    0b11111
};

char numero[100];

void timer0_irqhandler(void)
{
    display_row(currentRow, 0b00000);
    currentRow = (currentRow + 1) % 5;
    display_row(currentRow, buffer_row_value[currentRow]);

    if (TIMER0.EVENTS_COMPARE[0]) {
        TIMER0.EVENTS_COMPARE[0] = 0;
    }
}

void error_blink()
{
    GPIO0.DIR = LED_MASK0;
    GPIO1.DIR = LED_MASK1;
    while (1) {
        display(ERROR);
    }
}

void display_row(int row, uint8_t pattern)
{
    if (row < 0 || row >= 5) {
        error_blink();
        return;
    }

    uint32_t mask_row = (1 << ROWS[row]);
    uint32_t mask_cols_p0 = 0;
    uint32_t mask_cols_p1 = 0;

    for (int i = 0; i < 5; i++) {
        if ((pattern >> (4 - i)) & 1) {
            if (i == 3) {
                mask_cols_p1 |= (1 << COLS[i]);
            } else {
                mask_cols_p0 |= (1 << COLS[i]);
            }
        }
    }

    GPIO0.OUTSET = _COL_MASK_GPIO0;
    GPIO1.OUTSET = _COL_MASK_GPIO1;
    GPIO0.OUTCLR = _ROW_MASK_GPIO0;

    GPIO0.OUTSET = mask_row;

    if (mask_cols_p0)
        GPIO0.OUTCLR = mask_cols_p0;
    if (mask_cols_p1)
        GPIO1.OUTCLR = mask_cols_p1;
}

void display(const uint8_t* row)
{
    for (int i = 0; i < 5; i++) {
        buffer_row_value[i] = row[i];
    }
}

void display_clear()
{
    for (int i = 0; i < 5; i++) {
        buffer_row_value[i] = 0;
    }
}

void display_init(uint32_t refresh_rate_hz)
{
    timer_init(1000000 / (refresh_rate_hz * 5));
    GPIO0.DIR = LED_MASK0;
    GPIO1.DIR = LED_MASK1;
}

void display_set_pixel(uint8_t x, uint8_t y, uint8_t state)
{
    if (x > 4 || y > 4)
        return;

    if (state) {
        buffer_row_value[y] |= 1 << (4 - x);
    } else {
        buffer_row_value[y] &= ~(1 << (4 - x));
    }
}
