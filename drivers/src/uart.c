#include "uart.h"
#include "gpio.h"
#include "nvic.h"
#include "utils.h"

char uart_tx_buffer_dma[UART_TX_BUFFER_SIZE];
char uart_rx_buffer_dma[UART_RX_BUFFER_SIZE];

char buffer_rx_irq[UART_RX_BUFFER_SIZE];

typedef struct {
    char buffer[UART_TX_QUEUE_SIZE][UART_TX_BUFFER_SIZE];
    volatile uint32_t head;
    volatile uint32_t tail;
    volatile uint32_t count;
} uart_tx_queue_t;

uart_tx_queue_t tx_queue = { 0 };

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
    va_list args;
    va_start(args, str);

    int total_length = concat_strings(uart_tx_buffer_dma, UART_TX_BUFFER_SIZE, str, args);
    va_end(args);

    if (total_length < 0)
        return;

    UART.TXD_PTR = (unsigned int)uart_tx_buffer_dma;
    UART.TXD_MAXCNT = total_length;
    UART.EVENTS_ENDTX = 0;
    UART.TASKS_STARTTX = 1;
    while (UART.EVENTS_ENDTX == 0)
        ;
    UART.TASKS_STOPTX = 1;
}

void uart_tx_irq(const char* str, ...)
{
    if (tx_queue.count >= UART_TX_QUEUE_SIZE) {
        return;
    }

    va_list args;
    va_start(args, str);

    memset(tx_queue.buffer[tx_queue.head], 0, UART_TX_BUFFER_SIZE);
    int total_length = concat_strings(tx_queue.buffer[tx_queue.head], UART_TX_BUFFER_SIZE, str, args);
    va_end(args);

    if (total_length < 0) {
        return;
    }

    tx_queue.head = (tx_queue.head + 1) % UART_TX_QUEUE_SIZE;
    tx_queue.count++;

    UART.TXD_PTR = (unsigned int)tx_queue.buffer[tx_queue.tail];
    UART.TXD_MAXCNT = string_length(tx_queue.buffer[tx_queue.tail]);
    UART.EVENTS_ENDTX = 0;
    UART.TASKS_STARTTX = 1;
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

        tx_queue.tail = (tx_queue.tail + 1) % UART_TX_QUEUE_SIZE;
        tx_queue.count--;

        if (tx_queue.count > 0) {
            UART.TXD_PTR = (unsigned int)tx_queue.buffer[tx_queue.tail];
            UART.TXD_MAXCNT = string_length(tx_queue.buffer[tx_queue.tail]);
            UART.EVENTS_ENDTX = 0;
            UART.TASKS_STARTTX = 1;
        }
    }
}
