#include "animations.h"
#include "nvic.h"
#include "timer.h"
#include "uart.h"
#include "utils.h"

int main(void)
{
    uart_init();
    display_init(REFRESH_RATE_HZ);
    uart_send("\n===========================\r\n");
    uart_send("     UART INICIADO             \r\n");
    uart_send("===========================\r\n");

    for (;;) {
        uart_send("\nAnimacion 1\r\n");
        display(HEART_BIG);
        delay(700000);

        uart_send("Animacion 2\r\n");
        display(HEART_SMALL);
        delay(250000);
    }
    return 0;
}
