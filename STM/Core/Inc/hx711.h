/*
 * hx711.h
 *
 *  Created on: 5 kwi 2023
 *      Author: Tomek
 */

#ifndef INC_HX711_H_
#define INC_HX711_H_

#include "main.h"

typedef struct {
	GPIO_TypeDef* CLK_port;
	GPIO_TypeDef* DT_port;
	uint16_t CLK_pin;
	uint16_t DT_pin;
	int32_t offset;
	float coefficent;
}hx711_t;

void hx711_init(hx711_t *loadcell, GPIO_TypeDef* CLK_port, GPIO_TypeDef* DT_port,
				uint16_t CLK_pin, uint16_t DT_pin, int32_t offset, float coefficent);

uint32_t hx711_get_raw_value(hx711_t *loadcell);
void hx711_tare(hx711_t *loadcell);
uint32_t hx711_get_raw_value_average(hx711_t *loadcell, uint8_t no_samples);
int32_t hx711_get_value(hx711_t *loadcell);
int32_t hx711_get_value_average(hx711_t *loadcell, uint8_t no_samples);



#endif /* INC_HX711_H_ */
