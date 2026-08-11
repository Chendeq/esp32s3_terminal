#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "driver/uart.h"
#include "driver/gpio.h"

#define SERIAL_UART_PORT UART_NUM_1
#define SERIAL_UART_BAUDRATE 115200
#define SERIAL_UART_TXD GPIO_NUM_43
#define SERIAL_UART_RXD GPIO_NUM_44
#define SERIAL_UART_RTS UART_PIN_NO_CHANGE
#define SERIAL_UART_CTS UART_PIN_NO_CHANGE
#define SERIAL_UART_BUF_SIZE 1024

/* 串口初始化 */
bool serial_port_init(void);

/* 串口写入 */
int serial_port_write(const char *data, int len);

/* 串口读取 */
int serial_port_read(uint8_t *Rx_buf, size_t max_len, uint32_t timeout_ms);

void serial_set_rx_active(bool active);
bool serial_get_rx_active(void);


#endif
