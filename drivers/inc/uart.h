#define UART_RX_BUFFER_SIZE 256
#define UART_TX_BUFFER_SIZE 512

#define PIN_TX 6
#define PIN_RX 8

#define UART (*(volatile struct _uarte*)(0x40002000))

struct _uarte {
    char _pad1[0x008]; // 0x000 - 0x007
    unsigned TASKS_STARTTX; // 0x008
    char _pad2[0x00C - 0x00C];
    unsigned TASKS_STOPTX; // 0x00C

    char _pad3[0x120 - 0x010]; // 0x010 - 0x11F
    unsigned EVENTS_ENDTX; // 0x120

    char _pad4[0x500 - 0x124]; // 0x124 - 0x4FF
    unsigned ENABLE; // 0x500

    char _pad5[0x50C - 0x504]; // 0x504 - 0x50B
    unsigned PSEL_TXD; // 0x50C

    char _pad6[0x514 - 0x510]; // 0x510 - 0x513
    unsigned PSEL_RXD; // 0x514

    char _pad7[0x518 - 0x518];
    unsigned PSEL_CTS; // 0x518
    unsigned PSEL_RTS; // 0x51C

    char _pad8[0x524 - 0x520]; // 0x520 - 0x523
    unsigned BAUDRATE; // 0x524

    char _pad9[0x544 - 0x528]; // 0x528 - 0x543
    unsigned TXD_PTR; // 0x544
    unsigned TXD_MAXCNT; // 0x548

    char _pad10[0x56C - 0x54C]; // 0x54C - 0x56B
    unsigned CONFIG; // 0x56C
};

void uart_init();
void uart_send(const char* str);
