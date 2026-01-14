#include "animations.h"

int main(void)
{
    GPIO0.DIR = LED_MASK0;
    GPIO1.DIR = LED_MASK1;

    uint32_t tiempo = 150;
    uint32_t repetitions = 5;

    // Implementacion de la matriz de leds
    for (;;) {
        animation_star(tiempo, repetitions);
        animation_x_rotate(tiempo, repetitions);
        animation_heart(tiempo, repetitions);
        animation_arrow(2 * tiempo, repetitions);
        animation_smile(tiempo, repetitions);
        animation_pacman(tiempo, repetitions);
    }

    return 0;
}
