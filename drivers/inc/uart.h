#include <stdarg.h>
#define uart_send(str, ...) uart_tx_polling(str, ##__VA_ARGS__, NULL)

#define UART_RX_BUFFER_SIZE 256
#define UART_TX_BUFFER_SIZE 512

#define PIN_TX 6
#define PIN_RX 8

#define UART (*(volatile struct _uarte*)(0x40002000))

extern char uart_tx_buffer_dma[UART_TX_BUFFER_SIZE];
extern char uart_rx_buffer_dma[UART_RX_BUFFER_SIZE];
extern char buffer_tx_irq[UART_TX_BUFFER_SIZE];
extern char buffer_rx_irq[UART_RX_BUFFER_SIZE];

struct _uarte {
    // Tasks
    unsigned TASKS_STARTRX; // 0x000
    unsigned TASKS_STOPRX; // 0x004
    unsigned TASKS_STARTTX; // 0x008
    unsigned TASKS_STOPTX; // 0x00C
    char _pad1[0x02C - 0x010]; // 0x010 - 0x02B
    unsigned TASKS_FLUSHRX; // 0x02C

    // Events
    char _pad2[0x100 - 0x030]; // 0x030 - 0x0FF
    unsigned EVENTS_CTS; // 0x100
    unsigned EVENTS_NCTS; // 0x104
    unsigned EVENTS_RXDRDY; // 0x108
    char _pad3[0x110 - 0x10C]; // 0x10C - 0x10F
    unsigned EVENTS_ENDRX; // 0x110
    char _pad4[0x11C - 0x114]; // 0x114 - 0x11B
    unsigned EVENTS_TXDRDY; // 0x11C
    unsigned EVENTS_ENDTX; // 0x120
    unsigned EVENTS_ERROR; // 0x124
    char _pad5[0x144 - 0x128]; // 0x128 - 0x143
    unsigned EVENTS_RXTO; // 0x144
    char _pad6[0x14C - 0x148]; // 0x148 - 0x14B
    unsigned EVENTS_RXSTARTED; // 0x14C
    unsigned EVENTS_TXSTARTED; // 0x150
    char _pad7[0x158 - 0x154]; // 0x154 - 0x157
    unsigned EVENTS_TXSTOPPED; // 0x158

    // Registers
    char _pad8[0x200 - 0x15C]; // 0x15C - 0x1FF
    unsigned SHORTS; // 0x200
    char _pad9[0x300 - 0x204]; // 0x204 - 0x2FF
    unsigned INTEN; // 0x300
    unsigned INTENSET; // 0x304
    unsigned INTENCLR; // 0x308
    char _pad10[0x480 - 0x30C]; // 0x30C - 0x47F
    unsigned ERRORSRC; // 0x480
    char _pad11[0x500 - 0x484]; // 0x484 - 0x4FF
    unsigned ENABLE; // 0x500
    char _pad12[0x508 - 0x504]; // 0x504 - 0x507
    unsigned PSEL_RTS; // 0x508
    unsigned PSEL_TXD; // 0x50C
    unsigned PSEL_CTS; // 0x510
    unsigned PSEL_RXD; // 0x514
    char _pad13[0x524 - 0x518]; // 0x518 - 0x523
    unsigned BAUDRATE; // 0x524
    char _pad14[0x534 - 0x528]; // 0x528 - 0x533
    unsigned RXD_PTR; // 0x534
    unsigned RXD_MAXCNT; // 0x538
    unsigned RXD_AMOUNT; // 0x53C
    char _pad15[0x544 - 0x540]; // 0x540 - 0x543
    unsigned TXD_PTR; // 0x544
    unsigned TXD_MAXCNT; // 0x548
    unsigned TXD_AMOUNT; // 0x54C
    char _pad16[0x56C - 0x550]; // 0x550 - 0x56B
    unsigned CONFIG; // 0x56C
};

void uart_init();
void uart_tx_polling(const char* str, ...);
void uart_rx_polling(const unsigned int num_bytes);
void uart_rx_irq_enable();
void uart_tx_irq_enable();
void uart_tx_irq(char* msg);
void UARTE0_IRQHandler(void);
