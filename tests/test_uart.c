#include "uart.h"

#define ECHO_BUFFER_SIZE 64

char echo_buffer[ECHO_BUFFER_SIZE] = { 0 };

void on_receive_msg(size_t bytes)
{
    uart_send(echo_buffer);
    uart_receive(echo_buffer, 1);
}

int test_uart(void)
{
    uart_init(UART_BAUDRATE_115200);
    uart_send("\r\n");
    uart_send("================================\r\n");
    uart_send("   micro:bit v2 - UART Test    \r\n");
    uart_send("================================\r\n");

    uart_send("Waiting for data...\r\n\r\n");

    uart_receive(echo_buffer, 1);
    while (1) { }
}
