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
#include "..\\include\Wsc.h"
#include "..\\include\Wifi.h"

#include <stdio.h>
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gattc_api.h"

#include "esp_gatt_defs.h"
#include "esp_bt_main.h"
// #include "esp_gatt_common_api.h"
#include "iBeacon.h"

#define TAG "BLE_BEACON"


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

iBeacon* BEACONS = NULL;

void intArrayToHexString(int arr[], int size, char* str) {
    int i, offset = 0;
    for (i = 0; i < size; i++) {
        offset += sprintf(str + offset, "0x%04x ", arr[i]);
    }
}

void initWebSocket(void* parameter) {

    initWifi();
    connectWsServer("ws://cmdmac.top:3000/mobile");
    while (!isWsConnected()) {        
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    while(isWsConnected()) {
        if (BEACONS != NULL) {
            for(int i = 0; i < 3; i++) {
                char str[128] = {0};
                char s[32] = {0};
                intArrayToHexString(BEACONS[i].addr, sizeof(BEACONS[i]), str);
                sprintf(str, "mac=%s, rssi=%d", s, BEACONS[i].rssi);
                printf("send\r\n");
                wsSendText(str, strlen(str));
            }
            
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void onIBeaconsFound(iBeacon* devices, int size) {
    // for (int i = 0; i < size; i++) {
    //     iBeacon beacon = devices[i];
    //     ESP_LOGI("MAIN", "ibeacons len = %d", i);
    //     esp_log_buffer_hex("IBEACON_DEMO: Device address:", beacon.addr, ESP_BD_ADDR_LEN);
    //     // esp_log_buffer_hex("IBEACON_DEMO: Proximity UUID:", beacon.data->ibeacon_vendor.proximity_uuid, ESP_UUID_LEN_128);
    //     ESP_LOGI("MAIN", "Major:  (%d)", beacon.major);
    //     ESP_LOGI("MAIN", "Minor:  (%d)", beacon.minor);
    //     // ESP_LOGI("MAIN", "Measured power (RSSI at a 1m distance):%d dbm", beacon.data->ibeacon_vendor.measured_power);
    //     ESP_LOGI("MAIN", "RSSI of packet:%d dbm", beacon.rssi);
    // }
    BEACONS = devices;
    iBeacon* closestDevices[3] = {0};
    
}


void app_main(void)
{
    iBeaconScanCallback callback = onIBeaconsFound;
    startScanIbeacon(callback);
    initWebSocket(NULL);
}
