#include <stdint.h>

#include "delay.h"
#include "gpio.h"

void error_blink();
void set_led(int row, uint8_t pattern);
void show(const uint8_t* img, int time);

void blink_main();
