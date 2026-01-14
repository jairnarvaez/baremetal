#include "blink.h"

#define TIME 100000

int main(void)
{
    GPIO0.DIRCLR = LED_MASK0;
    GPIO1.DIRCLR = LED_MASK1;

    // Implementacion de la matriz de leds
    blink_main();

    return 0;
}
