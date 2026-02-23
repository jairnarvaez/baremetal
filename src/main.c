#include "shell.h"

int main(void)
{
    shell_init(UART_BAUDRATE_115200);

    uart_send("================================\r\n");
    uart_send("   micro:bit v2 - SHELL Started \r\n");
    uart_send("================================\r\n");

    for (;;) {
    }
}
