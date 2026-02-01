#ifndef BUTTONS_H
#define BUTTONS_H

#include "gpio.h"
#include "gpiote.h"
#include "nvic.h"

#define BUTTON_A_PIN 14
#define BUTTON_B_PIN 23

#define button_a_is_pressed() !GPIO_Read(&GPIO0, BUTTON_A_PIN)
#define button_b_is_pressed() !GPIO_Read(&GPIO0, BUTTON_B_PIN)

static inline void buttons_init(void)
{
    GPIO_Input(&GPIO0, BUTTON_A_PIN, GPIO_PULL_UP);
    GPIO_Input(&GPIO0, BUTTON_B_PIN, GPIO_PULL_UP);
}

static inline void buttons_init_gpiote(GPIOET_Channel_t channel_Button_A, GPIOET_Channel_t channel_Button_B)
{
    GPIO_Input(&GPIO0, BUTTON_A_PIN, GPIO_PULL_UP);
    GPIO_Input(&GPIO0, BUTTON_B_PIN, GPIO_PULL_UP);

    GPIOTE_ConfigEvent(&GPIOET, channel_Button_A, BUTTON_A_PIN, GPIOET_PORT_0, GPIOET_POLARITY_TOGGLE);
    GPIOTE_ConfigEvent(&GPIOET, channel_Button_B, BUTTON_B_PIN, GPIOET_PORT_0, GPIOET_POLARITY_TOGGLE);

    GPIOET.INTENSET = (1 << 0) | (1 << 1) | (1 << 31);
    NVIC_EnableIRQ(6);
}

#endif
