#include "pwm_motor.h"

extern TIM_HandleTypeDef htim1;

void pwm_motor_init(void)
{
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	pwm_motor_ctrl_send(0);
}

void pwm_motor_ctrl_send(int16_t ccr)
{
	if(ccr >= 0 && ccr <= 100)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
		
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,ccr);	
	}else if(ccr < 0 && ccr >= -100)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);	
		
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,-ccr);			
	}else
	{
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,0);			
	}
}
