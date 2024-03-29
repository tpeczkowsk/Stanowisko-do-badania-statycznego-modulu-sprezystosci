/*
 * uart_logger.h
 *
 *  Created on: Apr 4, 2023
 *      Author: Tomek
 */

#ifndef INC_UART_HANDLER_H_
#define INC_UART_HANDLER_H_

#include "usart.h"

#define SENDER_UART huart3

void uart_handler_ping_pong(void);
void uart_handler_start_receiving(void);
void uart_handler_transmit(uint8_t *data_ptr, uint16_t size);
uint8_t uart_handler_get_data_received(void);
void uart_handler_set_data_received(uint8_t value);

#endif /* INC_UART_HANDLER_H_ */
