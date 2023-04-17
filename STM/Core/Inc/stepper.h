/*
 * stepper.h
 *
 *  Created on: Apr 5, 2023
 *      Author: Tomek
 */

#ifndef INC_STEPPER_H_
#define INC_STEPPER_H_

#include "main.h"

#define STEPPER_MAX_FREQ	(MICRO_STEP * 1000)
#define STEPPER_MIN_FREQ	1

#define STEPPER_MAX_SPEED	200

#define STEP_PER_REV		200
#define MICRO_STEP			32

typedef enum {
	STOP = 0,
	ANGLE = 1,
	CONTINOUS = 2
}stepper_mode_t;

typedef enum {
	CCW = 0,
	CW = 1
}direction_t;

typedef struct {
	TIM_HandleTypeDef *htim;
	uint32_t channel;

}stepper_timer_t;

typedef struct {
	GPIO_TypeDef *STEP_port;
	GPIO_TypeDef *DIR_port;
	GPIO_TypeDef *ENABLE_port;
}stepper_ports_t;

typedef struct {
	uint16_t STEP_pin;
	uint16_t DIR_pin;
	uint16_t ENABLE_pin;
}stepper_pins_t;

typedef struct {
	stepper_timer_t timer;
	stepper_timer_t slave_timer;

	stepper_mode_t mode;

	stepper_ports_t ports;
	stepper_pins_t pins;

	uint32_t steps_to_move;
}stepper_t;

void stepper_init(stepper_t *stepper, TIM_HandleTypeDef *timer, uint32_t channel, TIM_HandleTypeDef *slave_timer,
		GPIO_TypeDef *STEP_port, GPIO_TypeDef *DIR_port, GPIO_TypeDef *ENABLE_port,
		uint16_t STEP_pin, uint16_t DIR_pin, uint16_t ENABLE_pin);
void stepper_set_continous_mode(stepper_t *stepper, direction_t dir, uint32_t speed);
void stepper_stop(stepper_t *stepper);
void stepper_set_direction(stepper_t *stepper, direction_t dir);
void stepper_set_speed(stepper_t *stepper, uint32_t speed);
void stepper_start_angle_mode(stepper_t *stepper, direction_t dir, uint32_t speed, uint32_t angle);
void stepper_set_enable_HIGH(stepper_t *stepper);
void stepper_set_enable_LOW(stepper_t *stepper);
#endif /* INC_STEPPER_H_ */
