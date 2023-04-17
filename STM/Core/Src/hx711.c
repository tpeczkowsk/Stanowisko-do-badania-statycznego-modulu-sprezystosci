/*
 * hx711.c
 *
 *  Created on: 5 kwi 2023
 *      Author: Tomek
 */

#include "hx711.h"
//#include "main.h"

static void hx711_wait_until_ready(hx711_t *loadcell);
static void hx711_reset(hx711_t *loadcell);

void hx711_init(hx711_t *loadcell, GPIO_TypeDef* CLK_port, GPIO_TypeDef* DT_port,
				uint16_t CLK_pin, uint16_t DT_pin, int32_t offset, float coefficent){

	loadcell->CLK_port = CLK_port;
	loadcell->DT_port = DT_port;
	loadcell->CLK_pin = CLK_pin;
	loadcell->DT_pin = DT_pin;
	loadcell->offset = offset;
	loadcell->coefficent = coefficent;
	// CLK pin to low
	hx711_reset(loadcell);
	HAL_Delay(1);
	hx711_tare(loadcell);
}

uint32_t hx711_get_raw_value(hx711_t *loadcell){
	uint32_t data = 0;
	hx711_wait_until_ready(loadcell);
	__disable_irq();
	for (int i = 0; i < 24; ++i) {
		data <<= 1;
		HAL_GPIO_WritePin(loadcell->CLK_port, loadcell->CLK_pin, GPIO_PIN_SET);
		data += HAL_GPIO_ReadPin(loadcell->DT_port, loadcell->DT_pin);
		HAL_GPIO_WritePin(loadcell->CLK_port, loadcell->CLK_pin, GPIO_PIN_RESET);
	}
	// set channel and gain
	HAL_GPIO_WritePin(loadcell->CLK_port, loadcell->CLK_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(loadcell->CLK_port, loadcell->CLK_pin, GPIO_PIN_RESET);
	__enable_irq();

	data ^= 0x800000;
	return data;
}

static void hx711_wait_until_ready(hx711_t *loadcell){

	while(HAL_GPIO_ReadPin(loadcell->DT_port, loadcell->DT_pin) == GPIO_PIN_SET){}
}

static void hx711_reset(hx711_t *loadcell){
	HAL_GPIO_WritePin(loadcell->CLK_port, loadcell->CLK_pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(loadcell->CLK_port, loadcell->CLK_pin, GPIO_PIN_RESET);
}

uint32_t hx711_get_raw_value_average(hx711_t *loadcell, uint8_t no_samples){
	uint64_t sum = 0;
	for (int i = 0; i < no_samples; ++i) {
		sum += hx711_get_raw_value(loadcell);
	}
	return sum / no_samples;
}

void hx711_tare(hx711_t *loadcell){
	uint32_t offset;
	offset = hx711_get_raw_value_average(loadcell, 10);
	loadcell->offset = offset;
}

int32_t hx711_get_value(hx711_t *loadcell){
	return hx711_get_raw_value(loadcell) - loadcell->offset;
	// add coeff
}

int32_t hx711_get_value_average(hx711_t *loadcell, uint8_t no_samples){
	return hx711_get_raw_value_average(loadcell, no_samples) - loadcell->offset;
	// add coeff
}

void hx711_calibrate(hx711_t *loadcell){

}
