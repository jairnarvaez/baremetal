#include "delay.h"

void delay(unsigned interval_us)
{
    unsigned cont = 16 * interval_us;
    while (cont > 0) {
        nop();
        cont--;
    }
}
