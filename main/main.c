/* GPIO Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#include "driver/uart.h"

#define TXD_PIN 1 //串口1发送管脚
#define RXD_PIN 2 //串口1接收管脚


static const int RX_BUF_SIZE = 1024;  //串口接收缓冲区大小
 
 
void init_uart(void)        
{
    const uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
 
 
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

#define SOFT_VER  "Ver: 4.7.5\n"
static void send_ver(void)
{
    const char *data = SOFT_VER;
    const int len = strlen(data);
    int err = uart_write_bytes(UART_NUM_1, data, len);
    printf("err=%s", SOFT_VER);
    uart_flush(UART_NUM_1);
}

void app_main(void)
{
    init_uart();
    while(1) {
        send_ver();
        // printf("send\n");
        // Read data from UART.
        // const uart_port_t uart_num = UART_NUM_1;
        // uint8_t data[128];
        // int length = 0;
        // ESP_ERROR_CHECK(uart_get_buffered_data_len(uart_num, (size_t*)&length));
        // length = uart_read_bytes(uart_num, data, length, 100);
        // if (length >0) {
        //     data[length] = 0;
        //     printf("l=%d, %s\n", length, data);
        
        // }
        // vTaskDelay(10);
    }
    printf("end\n");
    
}
