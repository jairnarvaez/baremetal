#ifndef BUTTONS_H
#define BUTTONS_H

#include "gpio.h"

#define BUTTON_A_PIN 14
#define BUTTON_B_PIN 23

#define button_a_is_pressed() !GPIO_Read(&GPIO0, BUTTON_A_PIN)
#define button_b_is_pressed() !GPIO_Read(&GPIO0, BUTTON_B_PIN)

static inline void buttons_init(void)
{
    GPIO_Input(&GPIO0, BUTTON_A_PIN, GPIO_PULL_UP);
    GPIO_Input(&GPIO0, BUTTON_B_PIN, GPIO_PULL_UP);
}

#endif
