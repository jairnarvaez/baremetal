#include "blink.h"

const uint8_t ROWS[] = { ROW1, ROW2, ROW3, ROW4, ROW5 };
const uint8_t COLS[] = { COL1, COL2, COL3, COL4, COL5 };

void error_blink()
{
    while (1) {
        set_col(2, LED_ON);
        delay(TIME_ERROR);
        set_col(2, LED_OFF);
        delay(TIME_ERROR);

        set_row(2, LED_ON);
        delay(TIME_ERROR);
        set_row(2, LED_OFF);
        delay(TIME_ERROR);
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

    uint8_t pin_row = ROWS[row];
    uint8_t pin_col = COLS[col];

    GPIO0.DIRSET = (1 << pin_row);

    if (pin_col == 37) {
        GPIO1.DIRSET = (1 << 5);

        if (state) {
            GPIO0.OUTSET = (1 << pin_row);
            GPIO1.OUTCLR = (1 << 5);
        } else {
            GPIO0.OUTCLR = (1 << pin_row);
            GPIO1.OUTCLR = (1 << 5);
        }
    } else {
        GPIO0.DIRSET = (1 << pin_col);

        if (state) {
            GPIO0.OUTSET = (1 << pin_row);
            GPIO0.OUTCLR = (1 << pin_col);
        } else {
            GPIO0.OUTCLR = (1 << pin_row);
            GPIO0.OUTCLR = (1 << pin_col);
        }
    }
}

void led_blink(int row, int col)
{
}

void led_blink_test()
{
    static const unsigned set[] = {
        _ROW(ROW1, 1, 0, 0, 1, 1),
        _ROW(ROW1, 1, 1, 0, 0, 1),
        _ROW(ROW1, 1, 1, 1, 0, 0),
        _ROW(ROW1, 0, 1, 1, 1, 0),
        _ROW(ROW1, 0, 0, 1, 1, 1),
    };

    GPIO0.DIRSET = LED_MASK0;
    GPIO1.DIRSET = LED_MASK1;

    while (1) {
        for (int i = 0; i < 10; i = i + 2) {
            GPIO0.OUTSET = set[i];
            GPIO1.OUTSET = set[i + 1];

            delay(200000);

            GPIO0.OUTCLR = set[i];
            GPIO1.OUTCLR = set[i + 1];
        }
    }
}
