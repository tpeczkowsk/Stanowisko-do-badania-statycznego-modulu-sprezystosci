/*
 * sharp_ir_sensor.c
 *
 *  Created on: Apr 4, 2023
 *      Author: Tomek
 */
#include "sharp_ir_sensor.h"
#include "adc.h"
#include "tim.h"
#include "stm32h7xx_hal.h"

static uint32_t distance_raw[NB_OF_CONV] = {0};

void sharp_ir_sensor_start(void){
	HAL_ADCEx_Calibration_Start(&SENSOR_ADC, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
	HAL_Delay(10);
	HAL_ADC_Start_DMA(&SENSOR_ADC, distance_raw, NB_OF_CONV);
	HAL_TIM_Base_Start(&htim3);
}

uint16_t sharp_ir_sensor_get_raw_value(void){
	uint32_t sum = 0;
	for (uint16_t i = 0; i < NB_OF_CONV; ++i) {
		sum += distance_raw[i];
	}
	return sum / NB_OF_CONV;
}
