#include "motors.h"
#include "tim.h"


motor_st motor1 = {.dir_pin = MOTOR1_DIR_Pin,
	    .pul_pin = MOTOR1_PUL_Pin,
	    .en_pin = MOTOR1_EN_Pin,
		.en_port = MOTOR1_EN_GPIO_Port,
		.dir_port = MOTOR1_DIR_GPIO_Port,
		.pul_port = MOTOR1_PUL_GPIO_Port,
		.timer = &htim1,
		.speed = 0,
		.cmd_steps = 0,
		.steps_left = 0
};

motor_st motor2 = {.dir_pin = MOTOR2_DIR_Pin,
	    .pul_pin = MOTOR2_PUL_Pin,
	    .en_pin = MOTOR2_EN_Pin,
		.en_port = MOTOR2_EN_GPIO_Port,
		.dir_port = MOTOR2_DIR_GPIO_Port,
		.pul_port = MOTOR2_PUL_GPIO_Port,
		.timer = &htim2,
		.speed = 0,
		.cmd_steps = 0,
		.steps_left = 0
};

motor_st motor3 = {.dir_pin = MOTOR3_DIR_Pin,
	    .pul_pin = MOTOR3_PUL_Pin,
	    .en_pin = MOTOR3_EN_Pin,
		.en_port = MOTOR3_EN_GPIO_Port,
		.dir_port = MOTOR3_DIR_GPIO_Port,
		.pul_port = MOTOR3_PUL_GPIO_Port,
		.timer = &htim3,
		.speed = 0,
		.cmd_steps = 0,
		.steps_left = 0
};

motor_st motor4 = {.dir_pin = MOTOR4_DIR_Pin,
	    .pul_pin = MOTOR4_PUL_Pin,
	    .en_pin = MOTOR4_EN_Pin,
		.en_port = MOTOR4_EN_GPIO_Port,
		.dir_port = MOTOR4_DIR_GPIO_Port,
		.pul_port = MOTOR4_PUL_GPIO_Port,
		.timer = &htim4,
		.speed = 0,
		.cmd_steps = 0,
		.steps_left = 0
};

motor_st motor5 = {.dir_pin = MOTOR5_DIR_Pin,
	    .pul_pin = MOTOR5_PUL_Pin,
	    .en_pin = MOTOR5_EN_Pin,
		.en_port = MOTOR5_EN_GPIO_Port,
		.dir_port = MOTOR5_DIR_GPIO_Port,
		.pul_port = MOTOR5_PUL_GPIO_Port,
		.timer = &htim8,
		.speed = 0,
		.cmd_steps = 0,
		.steps_left = 0
};




void static MOTOR_set_one(motor_st* motor, int8_t speed, uint8_t steps){
	motor->steps_left = steps*MOTOR_STEPS_PER_REVOLUTION+1;

	HAL_GPIO_WritePin(motor->dir_port, motor->dir_pin, speed > 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
	speed = speed > 0 ? speed : -speed;
	motor->timer->Instance->PSC = 0x8000 >> speed; 
	for (uint8_t i = 0; i < 254; i++){
		asm("nop");
	}
	if (motor->steps_left){
		HAL_GPIO_WritePin(motor->en_port, motor->en_pin, GPIO_PIN_SET);
		HAL_TIM_PWM_Start_IT(motor->timer, TIM_CHANNEL_1);
	}
}

void static MOTOR_update(motor_st* motor){
	if (motor->pulse_finished){
		if (motor->steps_left){
			motor->steps_left--;
			motor->timer->Instance->CCR1 = MOTOR_CCR1_START_VALUE;
		} else{
			motor->timer->Instance->CCR1 = 0;
			HAL_TIM_PWM_Stop_IT(motor->timer, TIM_CHANNEL_1);
		}
		motor->pulse_finished = false;
	}
}

void MOTOR_main(){
	MOTOR_update(&motor1);
	MOTOR_update(&motor2);
	MOTOR_update(&motor3);
	MOTOR_update(&motor4);
	MOTOR_update(&motor5);
}

void MOTOR_init(){
	HAL_TIM_PWM_Start_IT(motor1.timer, TIM_CHANNEL_1);
}

void MOTOR_set_all(usart_buffer_st* buf){
	MOTOR_set_one(&motor1, buf->m1_speed, buf->m1_steps);
	MOTOR_set_one(&motor2, buf->m2_speed, buf->m2_steps);
	MOTOR_set_one(&motor3, buf->m3_speed, buf->m3_steps);
	MOTOR_set_one(&motor4, buf->m4_speed, buf->m4_steps);
	MOTOR_set_one(&motor5, buf->m5_speed, buf->m5_steps);
}

void MOTOR_interrupt(TIM_HandleTypeDef* htim){
	if (htim == motor1.timer){
		motor1.pulse_finished = true;
	}
	if (htim == motor2.timer){
		motor2.pulse_finished = true;
	}
	if (htim == motor3.timer){
		motor3.pulse_finished = true;
	}
	if (htim == motor4.timer){
		motor4.pulse_finished = true;
	}
	if (htim == motor5.timer){
		motor5.pulse_finished = true;
	}

}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef* htim){
	MOTOR_interrupt(htim);
}
