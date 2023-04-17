/*
 * uart_logger.c
 *
 *  Created on: Apr 4, 2023
 *      Author: Tomek
 */

#include "uart_logger.h"
#include "stdio.h"
#include "stm32h7xx_hal.h"

int __io_putchar(int ch){
  HAL_UART_Transmit(&LOGGER_UART, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}

void uart_log(char * message){
	char log[300];
	sprintf(log, "%s\r\n", message);
	printf(log);
}
