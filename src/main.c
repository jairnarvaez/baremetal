#include "animations.h"
#include "delay.h"
#include "uart.h"

int main(void)
{
    uart_init();
    uart_send("Hola mundo :)\r\n");
    animations_main();
    return 0;
}
