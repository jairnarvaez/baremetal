#include "shell.h"
#include "command_router.h"

#define BACKSPACE 0x7F
#define LINE_BUF_MAX 64

static char rx_byte[1];
static char line_buf[LINE_BUF_MAX];
static int line_pos = 0;

void shell_init(unsigned baudrate)
{
    uart_init(baudrate);

    uart_receive(rx_byte, 1);
    uart_send("OK READY\r\n");
}

static void handle_enter(void)
{
    line_buf[line_pos] = '\0';
    if (line_pos > 0)
        router_process(line_buf);
    line_pos = 0;
}

static void handle_backspace(void)
{
    if (line_pos > 0) {
        line_pos--;
        uart_send("\b \b");
    }
}

static void handle_char(char c)
{
    if (line_pos < LINE_BUF_MAX)
        line_buf[line_pos++] = c;
}

void on_receive_msg(size_t bytes)
{
    (void)bytes;
    char c = rx_byte[0];

    if (c == '\r' || c == '\n')
        handle_enter();
    else if (c == '\b' || c == BACKSPACE)
        handle_backspace();
    else
        handle_char(c);

    uart_send(rx_byte);
    uart_receive(rx_byte, 1);
}
