#include "led.h"
#include "utils.h"

const uint8_t ROWS[] = { ROW1, ROW2, ROW3, ROW4, ROW5 };
const uint8_t COLS[] = { COL1, COL2, COL3, COL4, COL5 };

const uint8_t ERROR[] = {
    0b11111,
    0b10000,
    0b11100,
    0b10000,
    0b11111
};

void error_blink()
{
    while (1) {
        show(ERROR, 100);
    }
}

void set_col(int col, LedState state)
{
    if (col < 0 || col >= 5) {
        error_blink();
        return;
    }

    uint32_t mask_col = (1 << COLS[col]);
    volatile struct _gpio* gpio_port = (col == 3) ? &GPIO1 : &GPIO0;

    GPIO0.DIRSET = _ROW_MASK_GPIO0;
    gpio_port->DIRSET = mask_col;

    if (state) {
        GPIO0.OUTSET = _ROW_MASK_GPIO0;
        gpio_port->OUTCLR = mask_col;
    } else {
        GPIO0.OUTCLR = _ROW_MASK_GPIO0;
        gpio_port->OUTSET = mask_col;
    }
}

void set_row(int row, LedState state)
{
    if (row < 0 || row >= 5) {
        error_blink();
        return;
    }

    uint32_t mask_row = (1 << ROWS[row]);

    GPIO0.DIRSET = mask_row | _COL_MASK_GPIO0;
    GPIO1.DIRSET = _COL_MASK_GPIO1;

    if (state) {
        GPIO0.OUTSET = mask_row;
        GPIO0.OUTCLR = _COL_MASK_GPIO0;
        GPIO1.OUTCLR = _COL_MASK_GPIO1;
    } else {
        GPIO0.OUTCLR = mask_row;
        GPIO0.OUTSET = _COL_MASK_GPIO0;
        GPIO1.OUTSET = _COL_MASK_GPIO1;
    }
}

void led_set(int row, int col, LedState state)
{
    if (row < 0 || row >= 5 || col < 0 || col >= 5) {
        error_blink();
        return;
    }

    uint32_t mask_row = (1 << ROWS[row]);
    uint32_t mask_col = (1 << COLS[col]);
    volatile struct _gpio* puerto_col = (col == 3) ? &GPIO1 : &GPIO0;

    GPIO0.DIRSET = mask_row;
    puerto_col->DIRSET = mask_col;

    if (state) {
        GPIO0.OUTSET = mask_row;
        puerto_col->OUTCLR = mask_col;
    } else {
        GPIO0.OUTCLR = mask_row;
        puerto_col->OUTSET = mask_col;
    }
}

void set_led(int row, uint8_t pattern)
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

void show(const uint8_t* img, int time)
{
    for (int row = 0; row < time; row++) {
        set_led(row % 5, img[row % 5]);
        delay(TIME);
    }
}

void* memset(void* s, int c, size_t n)
{
    unsigned char* p = s;
    while (n--)
        *p++ = (unsigned char)c;
    return s;
}

void blink_main()
{
    for (;;) {
    }
}
