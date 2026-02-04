#include "tim_encoder.h"

extern TIM_HandleTypeDef htim2;

void tim_encoder_init(void)
{
	HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_ALL);
	__HAL_TIM_SET_COUNTER(&htim2,32000);		
}
