#include "test_button.h"
#include "animations.h"
#include "buttons.h"
#include "display.h"
#include "uart.h"

typedef enum {
    STATE_IDLE,
    STATE_BUTTON_A,
    STATE_BUTTON_B
} app_state_t;

static volatile app_state_t current_state = STATE_IDLE;

void GPIOTE_Channel0_Callback(void)
{
    if (button_a_is_pressed()) {
        uart_send("Button A pressed\r\n");
        current_state = STATE_BUTTON_A;
    } else {
        uart_send("Button A released\r\n");
        current_state = STATE_IDLE;
    }
}

void GPIOTE_Channel1_Callback(void)
{
    if (button_b_is_pressed()) {
        uart_send("Button B pressed\r\n");
        current_state = STATE_BUTTON_B;
    } else {
        uart_send("Button B released\r\n");
        current_state = STATE_IDLE;
    }
}

static void print_test_banner(void)
{
    uart_send("\r\n");
    uart_send("================================\r\n");
    uart_send("  micro:bit v2 - Button Test   \r\n");
    uart_send("================================\r\n");
    uart_send("Button A: Heart (big)\r\n");
    uart_send("Button B: Heart (small)\r\n");
    uart_send("Ready!\r\n\r\n");
}

void test_buttons(void)
{
    uart_init(UART_BAUDRATE_115200);
    buttons_init_gpiote(GPIOET_CHANNEL_0, GPIOET_CHANNEL_1);

    display_init(60);

    print_test_banner();

    for (;;) {
        switch (current_state) {
        case STATE_IDLE:
            display(SMILE);
            break;

        case STATE_BUTTON_A:
            display(HEART_BIG);
            break;

        case STATE_BUTTON_B:
            display(HEART_SMALL);
            break;

        default:
            current_state = STATE_IDLE;
            break;
        }
    }
}
