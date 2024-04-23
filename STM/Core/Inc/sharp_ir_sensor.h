/*
 * sharp_ir_sensor.h
 *
 *  Created on: Apr 4, 2023
 *      Author: Tomek
 */

#ifndef INC_SHARP_IR_SENSOR_H_
#define INC_SHARP_IR_SENSOR_H_

#include "stdint.h"

#define SENSOR_ADC hadc1

#define NB_OF_CONV 20

void sharp_ir_sensor_start(void);
uint16_t sharp_ir_sensor_get_raw_value(void);

#endif /* INC_SHARP_IR_SENSOR_H_ */
