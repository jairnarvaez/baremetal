#include <stdint.h>

#include "display.h"
#include "error.h"
#include "gpio.h"
#include "timer.h"
#include "utils.h"

#define LED_MASK0 0xd1688800
#define LED_MASK1 0x00000020

const GPIO_LedMatrix_t ROWS[] = { LED_ROW1, LED_ROW2, LED_ROW3, LED_ROW4, LED_ROW5 };
const GPIO_LedMatrix_t COLS[] = { LED_COL1, LED_COL2, LED_COL3, LED_COL4, LED_COL5 };

uint8_t currentRow = 0;
uint8_t buffer_row_value[5];

static void display_row(int row, uint8_t pattern)
{
    if (row < 0 || row >= DISPLAY_ROWS) {
        error_blink();
        return;
    }

    uint32_t mask_row = SET_BIT(ROWS[row]);
    uint32_t mask_cols_p0 = 0;
    uint32_t mask_cols_p1 = 0;

    for (int i = 0; i < DISPLAY_COLS; i++) {
        if ((pattern >> (4 - i)) & 1) {
            if (i == 3) {
                mask_cols_p1 |= SET_BIT(COLS[i]);
            } else {
                mask_cols_p0 |= SET_BIT(COLS[i]);
            }
        }
    }

    GPIO_Write_Mask(&GPIO0, MATRIX_COL_PORT0_MASK, GPIO_PIN_HIGH);
    GPIO_Write_Mask(&GPIO1, MATRIX_COL_PORT1_MASK, GPIO_PIN_HIGH);

    GPIO_Write_Mask(&GPIO0, MATRIX_ROW_MASK, GPIO_PIN_LOW);

    GPIO_Write_Mask(&GPIO0, mask_row, GPIO_PIN_HIGH);

    if (mask_cols_p0) {
        GPIO_Write_Mask(&GPIO0, mask_cols_p0, GPIO_PIN_LOW);
    }
    if (mask_cols_p1) {
        GPIO_Write_Mask(&GPIO1, mask_cols_p1, GPIO_PIN_LOW);
    }
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
    uint32_t pin_config = GPIO_PIN_CNF_PACK(
        GPIO_DIR_OUTPUT,
        GPIO_INPUT_DISCONNECT,
        GPIO_PULL_DISABLED,
        GPIO_DRIVE_S0S1,
        GPIO_SENSE_DISABLED);

    GPIO_Config_Mask(&GPIO0, MATRIX_ROW_MASK | MATRIX_COL_PORT0_MASK, pin_config);
    GPIO_Config_Mask(&GPIO1, MATRIX_COL_PORT1_MASK, pin_config);

    timer_init(1000000 / (refresh_rate_hz * DISPLAY_ROWS));
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

void timer0_irqhandler(void)
{
    display_row(currentRow, 0b00000);
    currentRow = (currentRow + 1) % 5;
    display_row(currentRow, buffer_row_value[currentRow]);

    if (TIMER0.EVENTS_COMPARE[0]) {
        TIMER0.EVENTS_COMPARE[0] = 0;
    }
}
