#include "uart.h"
#include "gpio.h"
#include "nvic.h"
#include "utils.h"

char uart_tx_buffer_dma[UART_TX_BUFFER_SIZE];
char uart_rx_buffer_dma[UART_RX_BUFFER_SIZE];

char buffer_tx_irq[UART_TX_BUFFER_SIZE];
char buffer_rx_irq[UART_RX_BUFFER_SIZE];

void uart_init(const unsigned int BAUDRATE)
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
    UART.PSEL_RXD = PIN_RX | 1 << UART_PSEL_PORT_BIT;
    UART.PSEL_CTS = 0xFFFFFFFF;
    UART.PSEL_RTS = 0xFFFFFFFF;

    UART.BAUDRATE = BAUDRATE;
    UART.CONFIG = 0;
    UART.ENABLE = UART_ENABLE_VALUE;
}

void uart_tx_polling(const char* str, ...)
{
    char* write_ptr = uart_tx_buffer_dma;

    int len = string_length(str);
    memcpy(write_ptr, str, len);
    write_ptr += len;

    va_list args;
    va_start(args, str);
    char* next_string;
    while ((next_string = va_arg(args, char*))) {
        int next_len = string_length(next_string);
        memcpy(write_ptr, next_string, next_len);
        write_ptr += next_len;
    }
    va_end(args);

    int total_length = write_ptr - uart_tx_buffer_dma;
    UART.TXD_PTR = (unsigned int)uart_tx_buffer_dma;
    UART.TXD_MAXCNT = total_length;

    UART.EVENTS_ENDTX = 0;
    UART.TASKS_STARTTX = 1;

    while (UART.EVENTS_ENDTX == 0)
        ;

    UART.TASKS_STOPTX = 1;
}

void uart_rx_polling(const unsigned int num_bytes)
{
    UART.RXD_PTR = (unsigned int)uart_rx_buffer_dma;
    UART.RXD_MAXCNT = num_bytes;
    UART.EVENTS_ENDRX = 0;
    UART.TASKS_STARTRX = 1;
    while (UART.EVENTS_ENDRX == 0)
        ;
}

void uart_tx_irq(char* msg)
{
    uint32_t len_msg = string_length(msg) + 1;
    UART.TXD_MAXCNT = len_msg;
    memcpy(buffer_tx_irq, msg, len_msg);
    UART.TASKS_STARTTX = 1;
}

void uart_rx_irq_enable()
{
    UART.EVENTS_ENDRX = 0;
    UART.RXD_PTR = (unsigned int)buffer_rx_irq;
    UART.RXD_MAXCNT = UART_RX_BUFFER_SIZE;
    UART.INTENSET = 1 << UART_INT_ENDRX;

    NVIC_EnableIRQ(UART_IRQ_NUMBER);

    UART.TASKS_STARTRX = 1;
}

void uart_tx_irq_enable()
{
    UART.EVENTS_ENDTX = 0;
    UART.TXD_PTR = (unsigned int)buffer_tx_irq;
    UART.INTENSET = 1 << UART_INT_ENDTX;

    NVIC_EnableIRQ(UART_IRQ_NUMBER);
}

void UARTE0_IRQHandler(void)
{
    if (UART.EVENTS_ENDRX) {
        UART.EVENTS_ENDRX = 0;
        UART.TASKS_STARTRX = 1;
    }

    if (UART.EVENTS_ENDTX) {
        UART.EVENTS_ENDTX = 0;
        UART.TASKS_STOPTX = 1;
    }
}
