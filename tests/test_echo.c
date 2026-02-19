#include "test_echo.h"
#include "ppi.h"
#include "uart.h"

char buffer[1];

void test_echo()
{
    uart_init(UART_BAUDRATE_115200);

    UART.SHORTS = (1 << 5);

    PPI.CH[0].EEP = (unsigned)&UART.EVENTS_ENDRX;
    PPI.CH[0].TEP = (unsigned)&UART.TASKS_STARTTX;

    UART.RXD_PTR = (unsigned int)buffer;
    UART.TXD_PTR = (unsigned int)buffer;
    UART.RXD_MAXCNT = 1;
    UART.TXD_MAXCNT = 1;

    PPI.CHENSET = (1 << 0);
    UART.TASKS_STARTRX = 1;

    for (;;) {
    }
}
