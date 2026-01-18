#include "nvic.h"
#include "timer.h"
#include "uart.h"
#include "utils.h"

int cont = 0;
char cont_string[100];

void timer0_irqhandler(void)
{
    int2string(cont, cont_string);
    uart_send("Timer activado: ", cont_string, "\r\n");
    cont++;

    if (TIMER0.EVENTS_COMPARE[0]) {
        TIMER0.EVENTS_COMPARE[0] = 0;
    }
}

int main(void)
{
    uart_init();
    uart_send("\n===========================\r\n");
    uart_send("     UART INICIADO             \r\n");
    uart_send("===========================\r\n");
    timer_init();

    for (;;) {
    }
    return 0;
}
