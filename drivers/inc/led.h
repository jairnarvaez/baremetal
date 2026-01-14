#include <stdint.h>

#include "delay.h"
#include "gpio.h"

void blink_main();

void led_set(int row, int col, LedState state);
void led_blink(int row, int col);
void led_blink_test();
void show_error();

void set_row(int row, LedState state);
void set_col(int col, LedState state);

void show(const uint8_t* img, int time);
