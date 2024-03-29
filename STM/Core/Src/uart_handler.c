/*
 * uart_logger.c
 *
 *  Created on: Apr 4, 2023
 *      Author: Tomek
 */

#include <uart_handler.h>
#include "stdio.h"
#include "stm32h7xx_hal_uart.h"
#include "string.h"

volatile uint8_t uart_data_received = 0;
uint8_t receive_buffer[256] = {0};

uint8_t uart_handler_get_data_received(void){
	return uart_data_received;
}

void uart_handler_set_data_received(uint8_t value){
	uart_data_received = value;
}

void uart_handler_ping_pong(void){
	  if(memcmp(receive_buffer,"Ping",4) == 0){
		  uart_handler_transmit((uint8_t*)"Pong", 4);
	  }
	  uart_handler_start_receiving();
}

void uart_handler_start_receiving(void){
	HAL_UARTEx_ReceiveToIdle_DMA(&SENDER_UART, receive_buffer, 256);
}

void uart_handler_transmit(uint8_t *data_ptr, uint16_t size){
	HAL_UART_Transmit_DMA(&SENDER_UART, data_ptr, size);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
//	if(huart == &SENDER_UART){
		uart_data_received = 1;
//	}
}
