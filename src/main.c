#include "animations.h"
#include "nvic.h"
#include "timer.h"
#include "uart.h"
#include "utils.h"

uint8_t state = 1;

int main(void)
{
    uart_init();

    display_init(REFRESH_RATE_HZ);

    int i = 0;
    char buffer[10];
    for (;;) {
        int2string(i++, buffer);
        uart_send(buffer, "\r\n");
        display_set_pixel(2, 2, state);
        delay(1000000);
    }
    return 0;
}
