#include "display.h"
#include "timer.h"
#include "uart.h"
#include "utils.h"
#include <stdint.h>

const GPIO_LedMatrix_t ROWS[] = { LED_ROW1, LED_ROW2, LED_ROW3, LED_ROW4, LED_ROW5 };
const GPIO_LedMatrix_t COLS[] = { LED_COL1, LED_COL2, LED_COL3, LED_COL4, LED_COL5 };

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

    GPIO0.OUTSET = MATRIX_COLS_PORT0_MASK;
    GPIO1.OUTSET = MATRIX_COLS_PORT1_MASK;
    GPIO0.OUTCLR = MATRIX_ROW_MASK;

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

void display_shift_right(void)
{
    for (int y = 0; y < 5; y++) {
        buffer_row_value[y] >>= 1;
    }
}

void display_shift_left(void)
{
    for (int y = 0; y < 5; y++) {
        buffer_row_value[y] <<= 1;
        buffer_row_value[y] &= 0x1F;
    }
}
