#include <stdint.h>

#include "led_microbit.h"
#include "time_microbit.h"

void led_set(int row, int col, LedState state);
void led_blink(int row, int col);
void led_blink_test();
void show_error();

void set_row(int row, LedState state);
void set_col(int col, LedState state);
