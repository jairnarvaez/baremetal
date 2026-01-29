#include "uart.h"
#include "gpio.h"
#include "nvic.h"
#include "utils.h"

char ram_buffer[UART_TX_BUFFER_SIZE];
char ram_buffer_rx[UART_RX_BUFFER_SIZE];

char buffer_rx_irq[UART_RX_BUFFER_SIZE];

void uart_init()
{
    GPIO1.DIRSET = _SET(PIN_TX);
    GPIO1.PIN_CNF[PIN_TX] = (CNF_DIR_OUTPUT << PIN_CNF_DIR)
        | (CNF_INPUT_DISCONNECT << PIN_CNF_INPUT)
        | (CNF_PULL_DISABLED << PIN_CNF_PULL);

    GPIO1.DIRCLR = _SET(PIN_RX);
    GPIO1.PIN_CNF[PIN_RX] = (CNF_DIR_INPUT << PIN_CNF_DIR)
        | (CNF_INPUT_CONNECT << PIN_CNF_INPUT)
        | (CNF_PULL_DISABLED << PIN_CNF_PULL);

    UART.PSEL_TXD = PIN_TX;
    UART.PSEL_RXD = PIN_RX | 1 << 5;
    UART.PSEL_CTS = 0xFFFFFFFF;
    UART.PSEL_RTS = 0xFFFFFFFF;

    UART.BAUDRATE = 0x01D7E000;
    UART.CONFIG = 0;
    UART.ENABLE = 8;
}

void uart_send_internal_polling(const char* str, ...)
{
    char* t;
    char* temp = ram_buffer;

    int len = string_length(str);
    memcpy(ram_buffer, str, len);
    temp += len;

    va_list ap;
    va_start(ap, str);

    while ((t = va_arg(ap, char*))) {
        memcpy(temp, t, string_length(t) + 1);
        temp += string_length(temp);
    }
    va_end(ap);

    int final_len = temp - ram_buffer;

    UART.TXD_PTR = (unsigned int)ram_buffer;
    UART.TXD_MAXCNT = final_len;
    UART.EVENTS_ENDTX = 0;
    UART.TASKS_STARTTX = 1;

    while (UART.EVENTS_ENDTX == 0)
        ;

    UART.TASKS_STOPTX = 1;
}

void uart_receive_internal_polling(const unsigned int num_bytes)
{
    UART.RXD_PTR = (unsigned int)ram_buffer_rx;
    UART.RXD_MAXCNT = num_bytes;
    UART.EVENTS_ENDRX = 0;
    UART.TASKS_STARTRX = 1;
    while (UART.EVENTS_ENDRX == 0)
        ;
}

void uart_enable_rx_irq()
{
    UART.EVENTS_ENDRX = 0;
    UART.RXD_PTR = (unsigned int)buffer_rx_irq;
    UART.RXD_MAXCNT = UART_RX_BUFFER_SIZE;
    UART.INTENSET = 1 << 4;
    NVIC_EnableIRQ(2);
    UART.TASKS_STARTRX = 1;
}

void receive_rx_irq(void)
{
    UART.EVENTS_ENDRX = 0;
    UART.TASKS_STARTRX = 1;
}
