/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
extern TIM_HandleTypeDef htim2;

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ledandmotorTask */
osThreadId_t ledandmotorTaskHandle;
const osThreadAttr_t ledandmotorTask_attributes = {
  .name = "ledandmotorTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for usart_senttask */
osThreadId_t usart_senttaskHandle;
const osThreadAttr_t usart_senttask_attributes = {
  .name = "usart_senttask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for data_collecttas */
osThreadId_t data_collecttasHandle;
const osThreadAttr_t data_collecttas_attributes = {
  .name = "data_collecttas",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for encoderqueue */
osMessageQueueId_t encoderqueueHandle;
const osMessageQueueAttr_t encoderqueue_attributes = {
  .name = "encoderqueue"
};
/* Definitions for encoderqueue2 */
osMessageQueueId_t encoderqueue2Handle;
const osMessageQueueAttr_t encoderqueue2_attributes = {
  .name = "encoderqueue2"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void ledandmotor(void *argument);
void usart_sent(void *argument);
void data_collect(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of encoderqueue */
  encoderqueueHandle = osMessageQueueNew (5, sizeof(int16_t), &encoderqueue_attributes);

  /* creation of encoderqueue2 */
  encoderqueue2Handle = osMessageQueueNew (5, sizeof(int16_t), &encoderqueue2_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of ledandmotorTask */
  ledandmotorTaskHandle = osThreadNew(ledandmotor, NULL, &ledandmotorTask_attributes);

  /* creation of usart_senttask */
  usart_senttaskHandle = osThreadNew(usart_sent, NULL, &usart_senttask_attributes);

  /* creation of data_collecttas */
  data_collecttasHandle = osThreadNew(data_collect, NULL, &data_collecttas_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_ledandmotor */
/**
* @brief Function implementing the ledandmotorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ledandmotor */
void ledandmotor(void *argument)
{
  /* USER CODE BEGIN ledandmotor */
	int16_t spectrl = 0;//电机PWM控制值
  
  /* Infinite loop */
  for(;;)
  {
		static uint8_t cnt = 0;	//方波测试
		
		if(cnt == 0)
		{
		// 发送正向速度PWM信号
		pwm_motor_ctrl_send(spectrl);
		cnt = 1;	
		}else if(cnt == 1)
		{
		// 发送反向速度PWM信号
		pwm_motor_ctrl_send(-spectrl);
		cnt = 0;	
		}
		
		// 翻转LED引脚状态，实现LED闪烁
		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_12);//LED闪烁
    
    osDelay(1000);
  }
  /* USER CODE END ledandmotor */
}

/* USER CODE BEGIN Header_usart_sent */
/**
* @brief Function implementing the usart_senttask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_usart_sent */
void usart_sent(void *argument)
{
  /* USER CODE BEGIN usart_sent */
  /* Infinite loop */
  for(;;)
  {
		// 存储从消息队列获取的速度值
		static int16_t spe = 0;
		// 存储从消息队列获取的位置值
		static int16_t loc = 0;
		
		// 从消息队列encoderqueueHandle获取速度数据，无限期等待
		osMessageQueueGet(encoderqueueHandle,&spe,NULL,osWaitForever);
		
		// 从消息队列encoderqueue2Handle获取位置数据，无限期等待
		osMessageQueueGet(encoderqueue2Handle,&loc,NULL,osWaitForever);
		
		// 通过串口打印速度值
		printf("%d,%d\n",spe,loc);
    
    // 任务延迟1毫秒
    osDelay(1);
  }
  /* USER CODE END usart_sent */
}

/* USER CODE BEGIN Header_data_collect */
/**
* @brief Function implementing the data_collecttas thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_data_collect */
void data_collect(void *argument)
{
  /* USER CODE BEGIN data_collect */
  /* Infinite loop */
  for(;;)
  {		
		// 静态变量：spe存储编码器速度，loc存储累计位置
		static int16_t spe;
		static int16_t loc;
		
		// 读取TIM2计数器的当前值（编码器计数值）
		spe = __HAL_TIM_GET_COUNTER(&htim2);
		// 将TIM2计数器重置为中间值32000（用于有符号速度计算）
		__HAL_TIM_SET_COUNTER(&htim2,32000);	
		// 计算相对于中间值的偏移量，得到有符号速度
		spe -= 32000;
		// 累计位置（积分速度得到位置）
		loc += spe;

		// 将速度值放入消息队列，等待时间为0，优先级为1
		osMessageQueuePut(encoderqueueHandle,&spe,0,1);
		
		// 将位置值放入消息队列，等待时间为0，优先级为1
		osMessageQueuePut(encoderqueue2Handle,&loc,0,1);
		
    // 任务延迟30毫秒
    osDelay(30);
  }
  /* USER CODE END data_collect */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

