#include "delay.h"
#include "display.h"
#include "gpio.h"
#include "timer.h"
#include "uart.h"
#include "utils.h"

void test_delay(uint32_t interval_us)
{
    char buffer[12];
    uint32_t time_counter = 0;

    uart_init(UART_BAUDRATE_115200);

    GPIO_Output(&GPIO0, LED_ROW3);
    GPIO_Output(&GPIO0, LED_COL3);

    TIMER_Init(&TIMER0, TIMER_MODE_TIMER, TIMER_BITMODE_32BIT, TIMER_PRESCALER_1MHz, TIMER_CHANNEL_0, 0);

    TIMER_Start(&TIMER0);

    delay(interval_us);

    time_counter = TIMER_GetCounter(&TIMER0, 0);
    TIMER_Stop(&TIMER0);

    int2string(time_counter, buffer);

    uart_send("--- Test Delay ---\r\n");
    uart_send("Expected (us): ");

    char buffer_exp[12];
    int2string(interval_us, buffer_exp);
    uart_send(buffer_exp);

    uart_send("\r\nReal (us):     ");
    uart_send(buffer);
    uart_send("\r\n------------------\r\n");

    GPIO_Write(&GPIO0, LED_ROW3, 1);
    GPIO_Write(&GPIO0, LED_COL3, 0);
}
