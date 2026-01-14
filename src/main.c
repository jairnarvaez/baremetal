#include "animations.h"

#define TIME 100000

int main(void)
{
    GPIO0.DIR = LED_MASK0;
    GPIO1.DIR = LED_MASK1;

    // Implementacion de la matriz de leds
    animation_arrow();

    return 0;
}
