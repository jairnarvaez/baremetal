#include "command_router.h"
#include "gpio.h"
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
        uart_send("ERR 02\n");
        return;
    }

    int port_num = atoi(argv[2]);
    int pin = atoi(argv[3]);

    volatile struct _gpio* port = get_port(port_num);
    if (!port || pin > MAX_PIN) {
        uart_send("ERR 05\n");
        return;
    }

    char* action = argv[1];

    if (string_compare(action, "O") == 0) {
        GPIO_Output(port, pin);
        uart_send("OK\n");

    } else if (string_compare(action, "I") == 0) {
        GPIO_Input(port, pin, GPIO_PULL_DISABLED);
        uart_send("OK\n");

    } else if (string_compare(action, "W") == 0) {
        if (argc < 5) {
            uart_send("ERR 02\n");
            return;
        }
        int val = atoi(argv[4]);
        GPIO_Write(port, pin, val ? GPIO_PIN_HIGH : GPIO_PIN_LOW);
        uart_send("OK\n");

    } else if (string_compare(action, "R") == 0) {
        uint32_t val = GPIO_Read(port, pin);
        uart_send("OK %lu\n", val);

    } else {
        uart_send("ERR 01\n");
    }
}

typedef void (*handler_fn)(int argc, char** argv);

typedef struct {
    const char* name;
    handler_fn handler;
} command_t;

static const command_t commands[] = {
    { "GPIO", handle_gpio },
    // { "I2C",    handle_i2c    },
    // { "GPIOTE", handle_gpiote },
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

    uart_send("ERR 01\n");
}
