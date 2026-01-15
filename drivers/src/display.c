#include "display.h"
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

void show_row(int row, uint8_t pattern)
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
        show_row(row % 5, img[row % 5]);
        delay(TIME);
    }
}

void blink_main()
{
    for (;;) {
    }
}
