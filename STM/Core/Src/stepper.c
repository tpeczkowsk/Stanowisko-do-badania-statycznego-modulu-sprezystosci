/*
 * stepper.c
 *
 *  Created on: Apr 5, 2023
 *      Author: Tomek
 */

#include "stepper.h"
#include "tim.h"

void stepper_init(stepper_t *stepper, TIM_HandleTypeDef *timer, uint32_t channel, TIM_HandleTypeDef *slave_timer,
		GPIO_TypeDef *STEP_port, GPIO_TypeDef *DIR_port, GPIO_TypeDef *ENABLE_port,
		uint16_t STEP_pin, uint16_t DIR_pin, uint16_t ENABLE_pin){
	stepper->timer.htim = timer;
	stepper->timer.channel = channel;
	stepper->slave_timer.htim = slave_timer;

	stepper->ports.STEP_port = STEP_port;
	stepper->ports.DIR_port = DIR_port;
	stepper->ports.ENABLE_port = ENABLE_port;

	stepper->pins.STEP_pin = STEP_pin;
	stepper->pins.DIR_pin = DIR_pin;
	stepper->pins.ENABLE_pin = ENABLE_pin;
}

void stepper_start_continous_mode(stepper_t *stepper, direction_t dir, uint32_t speed){
	stepper->mode = CONTINOUS;

	stepper_set_direction(stepper, dir);
	stepper_set_speed(stepper, speed);

	HAL_TIM_PWM_Start(stepper->timer.htim, stepper->timer.channel);

}

void stepper_stop(stepper_t *stepper){
	stepper->mode = STOP;
	__HAL_TIM_SET_COMPARE(stepper->timer.htim,stepper->timer.channel,0);
	HAL_TIM_PWM_Stop(stepper->timer.htim, stepper->timer.channel);
	HAL_TIM_Base_Stop_IT(stepper->slave_timer.htim);
}

void stepper_set_direction(stepper_t *stepper, direction_t dir){
	HAL_GPIO_WritePin(stepper->ports.DIR_port, stepper->pins.DIR_pin, dir);
}

void stepper_set_speed(stepper_t *stepper, uint32_t speed){

	uint32_t timer_count, frequency;
	 if(speed > STEPPER_MAX_SPEED){
		 speed = STEPPER_MAX_SPEED;
	 } else if(speed == 0) {
		 stepper_stop(stepper);
		 return;
	 }

	 frequency = (speed * (STEPPER_MAX_FREQ - STEPPER_MIN_FREQ)) / STEPPER_MAX_SPEED;
	 timer_count = (HAL_RCC_GetPCLK1Freq() * 2) / (stepper->timer.htim->Init.Prescaler * frequency);

	 __HAL_TIM_SET_COUNTER(stepper->timer.htim,0);
	 __HAL_TIM_SET_AUTORELOAD(stepper->timer.htim, timer_count - 1);
	 __HAL_TIM_SET_COMPARE(stepper->timer.htim, stepper->timer.channel, (timer_count / 2) - 1);
}

void stepper_start_angle_mode(stepper_t *stepper, direction_t dir, uint32_t speed, uint32_t angle){

	stepper->mode = ANGLE;
	stepper_set_direction(stepper, dir);
	stepper_set_speed(stepper, speed);

	stepper->steps_to_move = (angle * STEPS_PER_REV * MICRO_STEP) / 360;

	if(stepper->steps_to_move == 0){
		stepper_stop(stepper);
		return;
	}

	__HAL_TIM_SET_COUNTER(stepper->slave_timer.htim, 0);
	__HAL_TIM_SET_AUTORELOAD(stepper->slave_timer.htim, stepper->steps_to_move - 1);
	HAL_TIM_Base_Start_IT(stepper->slave_timer.htim);
	HAL_TIM_PWM_Start(stepper->timer.htim, stepper->timer.channel);
}

void stepper_set_enable_HIGH(stepper_t *stepper){
	HAL_GPIO_WritePin(stepper->ports.ENABLE_port, stepper->pins.ENABLE_pin, GPIO_PIN_SET);
}

void stepper_set_enable_LOW(stepper_t *stepper){
	HAL_GPIO_WritePin(stepper->ports.ENABLE_port, stepper->pins.ENABLE_pin, GPIO_PIN_RESET);
}

