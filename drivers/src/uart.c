#include "uart.h"
#include "gpio.h"

int string_length(const char* str)
{
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

void uart_init()
{
    GPIO0.DIRSET = _SET(PIN_TX);
    GPIO0.PIN_CNF[PIN_TX] = (CNF_DIR_OUTPUT << PIN_CNF_DIR)
        | (CNF_INPUT_DISCONNECT << PIN_CNF_INPUT)
        | (CNF_PULL_DISABLED << PIN_CNF_PULL);

    GPIO0.DIRCLR = _SET(PIN_RX);
    GPIO0.PIN_CNF[PIN_RX] = (CNF_DIR_INPUT << PIN_CNF_DIR)
        | (CNF_INPUT_CONNECT << PIN_CNF_INPUT)
        | (CNF_PULL_DISABLED << PIN_CNF_PULL);

    UART.PSEL_TXD = PIN_TX;
    UART.PSEL_RXD = PIN_RX;
    UART.PSEL_CTS = 0xFFFFFFFF;
    UART.PSEL_RTS = 0xFFFFFFFF;

    UART.BAUDRATE = 0x01D7E000;
    UART.CONFIG = 0;
    UART.ENABLE = 8;
}

void uart_send(const char* str)
{
    int len = string_length(str);

    // Copiar a buffer en RAM
    static char ram_buffer[256];
    for (int i = 0; i < len; i++) {
        ram_buffer[i] = str[i];
    }

    UART.TXD_PTR = (unsigned int)ram_buffer;
    UART.TXD_MAXCNT = len;

    UART.EVENTS_ENDTX = 0;

    UART.TASKS_STARTTX = 1;

    while (UART.EVENTS_ENDTX == 0)
        ;

    UART.TASKS_STOPTX = 1;
}
