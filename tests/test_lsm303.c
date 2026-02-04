#include "delay.h"
#include "lsm303.h"
#include "uart.h"
#include "utils.h"
#include <stdint.h>

int test_lsm303(void)
{
    char str_buf[16];

    lsm303_acc_init(LSM_MODE_HIGH_RESOLUTION);
    lsm_acc_data_t data;

    uart_init(UART_BAUDRATE_115200);

    uart_send("================================\r\n");
    uart_send("   micro:bit v2 - LSM303 Test    \r\n");
    uart_send("================================\r\n");

    uart_send("\rInitiating communication with the sensor\r\n");

    lsm303_tmp_init();
    int temp_sensor = 0;
    char buffer_temp_sensor[10];

    for (;;) {
        data = lsm_acc_get_acceleration();

        uart_send("X: ");
        int2string(data.acceleration_x, str_buf);
        uart_send(str_buf);

        uart_send("  Y: ");
        int2string(data.acceleration_y, str_buf);
        uart_send(str_buf);

        uart_send("  Z: ");
        int2string(data.acceleration_z, str_buf);
        uart_send(str_buf);

        uart_send("       \r\n");

        temp_sensor = lsm303_acc_get_temperature();
        int2string(temp_sensor, buffer_temp_sensor);
        uart_send("Temp: ", buffer_temp_sensor, "\r\n");
        delay(1000000);
    }

    return 0;
}
