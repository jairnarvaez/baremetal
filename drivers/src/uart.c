#include "uart.h"
#include "gpio.h"
#include "nvic.h"
#include "utils.h"
#include <string.h>

char uart_tx_buffer_dma[UART_TX_BUFFER_SIZE];
char uart_rx_buffer_dma[UART_RX_BUFFER_SIZE];

typedef struct {
    char buffer[UART_TX_QUEUE_SIZE][UART_TX_BUFFER_SIZE];
    uint32_t head;
    uint32_t tail;
    uint32_t count;
} uart_tx_queue_t;

typedef struct {
    char buffer[UART_RX_QUEUE_SIZE][UART_RX_BUFFER_SIZE];
    char* buffer_dest[UART_RX_QUEUE_SIZE];
    size_t size_buffer_dest[UART_RX_QUEUE_SIZE];
    uint32_t head;
    uint32_t tail;
    uint32_t count;
} uart_rx_queue_t;

uart_tx_queue_t tx_queue = { 0 };
uart_rx_queue_t rx_queue = { 0 };

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

    if (UART_TX_MODE_IRQ)
        uart_tx_irq_enable();

    if (UART_RX_MODE_IRQ)
        uart_rx_irq_enable();
}

void uart_tx_polling(const char* str, ...)
{
    va_list args;
    va_start(args, str);

    int total_length = vsconcat_strings(uart_tx_buffer_dma, UART_TX_BUFFER_SIZE, str, args);
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
    int total_length = vsconcat_strings(tx_queue.buffer[tx_queue.head], UART_TX_BUFFER_SIZE, str, args);
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

void uart_rx_polling(char* buffer, const unsigned int num_bytes)
{
    UART.RXD_PTR = (unsigned int)uart_rx_buffer_dma;
    UART.RXD_MAXCNT = num_bytes;
    UART.EVENTS_ENDRX = 0;
    UART.TASKS_STARTRX = 1;
    while (UART.EVENTS_ENDRX == 0)
        ;
    memcpy(buffer, uart_rx_buffer_dma, num_bytes);
}

void uart_rx_irq(char* buffer, const unsigned int num_bytes)
{
    if (rx_queue.count >= UART_RX_QUEUE_SIZE) {
        return;
    }

    rx_queue.buffer_dest[rx_queue.head] = buffer;
    rx_queue.size_buffer_dest[rx_queue.head] = num_bytes;
    memset(rx_queue.buffer[rx_queue.head], 0, UART_RX_BUFFER_SIZE);

    uint32_t old_count = rx_queue.count;

    rx_queue.head = (rx_queue.head + 1) % UART_RX_QUEUE_SIZE;
    rx_queue.count++;

    if (old_count == 0) {
        UART.RXD_PTR = (unsigned int)rx_queue.buffer[rx_queue.tail];
        UART.RXD_MAXCNT = num_bytes;
        UART.EVENTS_ENDRX = 0;
        UART.TASKS_STARTRX = 1;
    }
}

void uart_rx_irq_enable()
{
    UART.EVENTS_ENDRX = 0;
    UART.INTENSET = 1 << UART_INT_ENDRX;

    NVIC_EnableIRQ(UART_IRQ_NUMBER);
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
        UART.TASKS_STOPRX = 1;

        memcpy(rx_queue.buffer_dest[rx_queue.tail],
            rx_queue.buffer[rx_queue.tail],
            UART.RXD_AMOUNT);

        rx_queue.tail = (rx_queue.tail + 1) % UART_RX_QUEUE_SIZE;
        rx_queue.count--;

        if (rx_queue.count > 0) {
            UART.RXD_PTR = (unsigned int)rx_queue.buffer[rx_queue.tail];
            UART.RXD_MAXCNT = rx_queue.size_buffer_dest[rx_queue.tail];
            UART.EVENTS_ENDRX = 0;
            UART.TASKS_STARTRX = 1;
        }
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
