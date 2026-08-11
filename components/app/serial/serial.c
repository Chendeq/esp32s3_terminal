#include "serial.h"
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "driver/gpio.h"
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "esp_err.h"

static const char *TAG = "serial";

static bool s_is_serial_inited = false;
static bool s_serial_rx_active = false;

/**
 * @brief 初始化串口
 * 
 * @return true 初始化成功
 * @return false 初始化失败
 */
bool serial_port_init(void)
{
    if(s_is_serial_inited){
        return true;
    }

    const uart_config_t uart_config = {
        .baud_rate = SERIAL_UART_BAUDRATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    if(uart_param_config(SERIAL_UART_PORT, &uart_config) != ESP_OK){
        ESP_LOGE(TAG, "uart_param_config failed");
        return false;
    }

    if(uart_set_pin(SERIAL_UART_PORT, SERIAL_UART_TXD, SERIAL_UART_RXD, SERIAL_UART_RTS, SERIAL_UART_CTS) != ESP_OK){
        ESP_LOGE(TAG, "uart_set_pin failed");
        return false;
    }

    if(uart_driver_install(SERIAL_UART_PORT, SERIAL_UART_BUF_SIZE, SERIAL_UART_BUF_SIZE, 0, NULL, 0) != ESP_OK){
        ESP_LOGE(TAG, "uart_driver_install failed");
        return false;
    }

    s_is_serial_inited = true;
    return true;
}

/**
 * @brief 写入数据到串口
 * 
 * @param data 要写入的数据指针
 * @param len 要写入的数据长度
 * @return int 写入的字节数
 */
int serial_port_write(const char *data, int len)
{
    if((!s_is_serial_inited) || (data == NULL) || (len == 0)){
        return 0;
    }
    return uart_write_bytes(SERIAL_UART_PORT, data, len);
}

/**
 * @brief 从串口读取数据
 * 
 * @param Rx_buf 读取数据的缓冲区指针
 * @param max_len 读取数据的最大缓冲区长度
 * @param timeout_ms 读取超时时间（毫秒）
 * @return int 读取的字节数
 */
int serial_port_read(uint8_t *Rx_buf, size_t max_len, uint32_t timeout_ms)
{
    if((!s_is_serial_inited) || (Rx_buf == NULL) || (max_len == 0)){
        return 0;
    }
    return uart_read_bytes(SERIAL_UART_PORT, Rx_buf, (uint32_t)max_len, pdMS_TO_TICKS(timeout_ms));
}

void serial_set_rx_active(bool active)
{
    s_serial_rx_active = active;
}

bool serial_get_rx_active(void)
{
    return s_serial_rx_active;
}


