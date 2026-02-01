#include "animations.h"
#include "buttons.h"
#include "display.h"
#include "uart.h"

int main(void)
{
    uart_init(UART_BAUDRATE_115200);
    buttons_init();
    display_init(60);

    uart_send("\r\n");
    uart_send("================================\r\n");
    uart_send("  micro:bit v2 - Demo Program  \r\n");
    uart_send("================================\r\n");
    uart_send("Button A: Heart (big)\r\n");
    uart_send("Button B: Heart (small)\r\n");
    uart_send("No button: Smile\r\n");
    uart_send("Ready!\r\n\r\n");

    display(SMILE);

    while (1) {
        if (button_a_is_pressed()) {
            display(HEART_BIG);
        } else if (button_b_is_pressed()) {
            display(HEART_SMALL);
        } else {
            display(SMILE);
        }
    }

    return 0;
}
