#include "command_router.h"
#include "display.h"
#include "error.h"
#include "gpio.h"
#include "reset.h"
#include "uart.h"
#include "utils.h"

#define MAX_ARGS 8
#define MAX_PIN 31

static volatile struct _gpio* get_port(int port_num)
{
    if (port_num == 0)
        return &GPIO0;
    if (port_num == 1)
        return &GPIO1;
    return NULL;
}

static int parse_line(char* line, char** argv, int max_args)
{
    int argc = 0;
    while (*line && argc < max_args) {
        while (*line == ' ')
            line++;
        if (*line == '\0')
            break;
        argv[argc++] = line;
        while (*line && *line != ' ')
            line++;
        if (*line == ' ')
            *line++ = '\0';
    }
    return argc;
}

static void handle_gpio(int argc, char** argv)
{
    if (argc < 4) {
        uart_send(ERR_INVALID_ARGS);
        return;
    }

    int port_num = atoi(argv[2]);
    int pin = atoi(argv[3]);

    volatile struct _gpio* port = get_port(port_num);
    if (!port || pin > MAX_PIN) {
        uart_send(ERR_INVALID_PIN);
        return;
    }

    char* action = argv[1];

    if (string_compare(action, "O") == 0) {
        GPIO_Output(port, pin);

    } else if (string_compare(action, "I") == 0) {
        GPIO_Input(port, pin, GPIO_PULL_DISABLED);

    } else if (string_compare(action, "W") == 0) {
        if (argc < 5) {
            uart_send(ERR_INVALID_ARGS);
            return;
        }
        int val = atoi(argv[4]);
        GPIO_Write(port, pin, val ? GPIO_PIN_HIGH : GPIO_PIN_LOW);

    } else if (string_compare(action, "R") == 0) {
        uint32_t val = GPIO_Read(port, pin);
        uart_send("OK %lu\n", val);
        return;

    } else {
        uart_send(ERR_UNKNOWN_CMD);
        return;
    }

    uart_send("OK\n");
}

static void handle_display(int argc, char** argv)
{
    char* action = argv[1];

    if (string_compare(action, "I") == 0) {
        display_init(60);

    } else if (string_compare(action, "W") == 0) {
        if (argc != 7) {
            uart_send(ERR_INVALID_ARGS);
            return;
        }
        uint8_t rows[5];
        for (int i = 0; i < 5; i++)
            rows[i] = (uint8_t)binary_str_to_int(argv[2 + i]);
        display(rows);

    } else if (string_compare(action, "C") == 0) {
        display_clear();

    } else if (string_compare(action, "P") == 0) {
        if (argc != 5) {
            uart_send(ERR_INVALID_ARGS);
            return;
        }
        display_set_pixel(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));

    } else {
        uart_send(ERR_UNKNOWN_CMD);
        return;
    }

    uart_send("OK\n");
}

static void handle_clear(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    uart_send("\033[2J\033[H");
}

static void handle_reset(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    baremetal_reset();
}

typedef void (*handler_fn)(int argc, char** argv);

typedef struct {
    const char* name;
    handler_fn handler;
} command_t;

static const command_t commands[] = {
    { "GPIO", handle_gpio },
    { "DISPLAY", handle_display },
    { "CLEAR", handle_clear },
    { "RESET", handle_reset },
    // { "GPIOTE",  handle_gpiote  },
};

#define CMD_COUNT (sizeof(commands) / sizeof(commands[0]))

void router_process(char* line)
{
    char* argv[MAX_ARGS];
    int argc = parse_line(line, argv, MAX_ARGS);

    if (argc == 0)
        return;

    for (int i = 0; i < CMD_COUNT; i++) {
        if (string_compare(argv[0], commands[i].name) == 0) {
            commands[i].handler(argc, argv);
            return;
        }
    }

    uart_send(ERR_UNKNOWN_CMD);
}
