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

static uint32_t distance_raw = 0;
static uint32_t distance_mm = 0;

void sharp_ir_sensor_start(void){
	HAL_ADCEx_Calibration_Start(&SENSOR_ADC, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
	HAL_ADC_Start_DMA(&SENSOR_ADC, &distance_raw, 1);
	HAL_TIM_Base_Start(&htim3);
}

uint32_t sharp_ir_sensor_get_raw(void){
	return distance_raw;
}
