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

#include "esp_log.h"
#include "Wsc.h"
#include "esp_websocket_client.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "driver/wifi.h"


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

void chanelWithCar(void* parameter) {
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

#define TAG "WebSocket"
void websocket_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_websocket_event_data_t *data = (esp_websocket_event_data_t *)event_data;
    switch (event_id) {
    case WEBSOCKET_EVENT_CONNECTED:
        ESP_LOGI(TAG, "WEBSOCKET_EVENT_CONNECTED");
        break;
    case WEBSOCKET_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "WEBSOCKET_EVENT_DISCONNECTED");
        break;
    case WEBSOCKET_EVENT_DATA:
        ESP_LOGI(TAG, "WEBSOCKET_EVENT_DATA");
        ESP_LOGI(TAG, "Received opcode=%d", data->op_code);
        ESP_LOGW(TAG, "Received=%.*s", data->data_len, (char *)data->data_ptr);
        ESP_LOGW(TAG, "Total payload length=%d, data_len=%d, current payload offset=%d\r\n", data->payload_len, data->data_len, data->payload_offset);

        // xTimerReset(shutdown_signal_timer, portMAX_DELAY);
        break;
    case WEBSOCKET_EVENT_ERROR:
        ESP_LOGI(TAG, "WEBSOCKET_EVENT_ERROR");
        break;
    }

}

void connectWsServer(const char* wsUrl) {
    esp_websocket_client_config_t websocket_cfg = {};
    websocket_cfg.uri = wsUrl;
    esp_websocket_client_handle_t client = esp_websocket_client_init(&websocket_cfg);
    esp_websocket_register_events(client, WEBSOCKET_EVENT_ANY, websocket_event_handler, (void *)client);
    esp_websocket_client_start(client);

}

void testWebSocket(void* parameter) {

    esp_netif_t *pEsp_wifi_netif;

    esp_netif_init();

    /*创建wifi sta模式的默认netif, 返回一个指针*/
    pEsp_wifi_netif = esp_netif_create_default_wifi_sta();

    /* (非必须) 这行代码是修改ESP32的主机名, 即WiFi设备名, 连接其他WiFi时显示的名称 */
    // esp_netif_set_hostname(esp_wifi_netif, "Augtons");// 不建议使用汉字

    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_init_config);

    /* 第二步, WiFi配置 */
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "Stark",   // wifi名（ssid）
            .password = "fengzhiping,1101",// wifi密码
        }
    };
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_set_mode(WIFI_MODE_STA);  // 设置工作模式

    /* 第三步, 启动WiFi */
    esp_wifi_start();


    connectWsServer("ws://localhost:3000/ws");

}
void app_main(void)
{
    // init_uart();
    xTaskCreate(&testWebSocket, "testWebSocket", CONFIG_FREERTOS_TIMER_TASK_STACK_DEPTH, NULL, 5, NULL);
    // connectWsServer("ws://localhost:3000/ws");
}
