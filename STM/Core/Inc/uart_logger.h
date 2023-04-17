/*
 * uart_logger.h
 *
 *  Created on: Apr 4, 2023
 *      Author: Tomek
 */

#ifndef INC_UART_LOGGER_H_
#define INC_UART_LOGGER_H_

#include "usart.h"

#define LOGGER_UART huart3

void uart_log(char * message);

#endif /* INC_UART_LOGGER_H_ */
