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

void sharp_ir_sensor_start(void);
uint32_t sharp_ir_sensor_get_raw(void);

#endif /* INC_SHARP_IR_SENSOR_H_ */
