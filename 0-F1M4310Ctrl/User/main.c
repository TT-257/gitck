#include "main.h"
#include "usart/bsp_usart.h"
#include "dma/bsp_dma.h"
#include "can/bsp_can.h"
#include "tim/bsp_tim.h"
#include "app_motorctrl.h"
#include "app_pid.h"
#include <stdio.h>

#define Length 64                   //定义串口缓冲区长度

uint8_t RXBuff[Length];             //串口接收缓冲
uint8_t rx_data[8] = {0};           //CAN接收数据缓冲区


extern PID motor1pid;

extern float pid_array[3];

void SystemClock_Config(void);

int main(void)
{
	HAL_Init();
    SystemClock_Config();
    MX_DMA_Init();
    MX_USART1_UART_Init();
	CAN_Init();
	MX_TIM2_Init();
	Motor_Init();
	pid_init();
	
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1,RXBuff,Length);//开启DMA中断
	HAL_TIM_Base_Start_IT(&htim2);						// 启动1ms定时器中断
	
	enable_motor(&motor1);
	
    while (1)
    {
//		sent_vofa(motor1pid.Target,motor1.rxpara.rVEL,motor1.rxpara.rT,motor1pid.Out);
		sent_vofa(motor1pidLocation.Target,motor1.rxpara.rVEL,motor1.rxpara.rPOS,motor1pidLocation.Out);
//		printf("running!");
//		HAL_Delay(120);
    }
}

//CAN接收回调函数
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rx_header;
    uint32_t can_id = 0;               
    
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data) == HAL_OK)
    {
        if (rx_header.IDE == CAN_ID_STD)  
        {
            can_id = rx_header.StdId;
        }
        else  
        {
            can_id = rx_header.ExtId;
        }
		
		if(can_id == motor1.MST_ID)
		{
			Get_MotorData(rx_data,&motor1);

		}
    }
}

// TIM2的回调函数
uint32_t count= 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) 
	{	
		count ++;
		if(count==3000)
		{
			count=0;
			if(motor1pidLocation.Target==0.0f)
			{
			motor1pidLocation.Target=5.0f;
			}
			else if(motor1pidLocation.Target==5.0f)
			{
			motor1pidLocation.Target=0.0f;
			}
			
		}
		pid2_init();

		motor1pidLocation.Actual = motor1.rxpara.rPOS;
		PID_Calc(&motor1pidLocation);
		motor1pid.Target = motor1pidLocation.Out;	
		motor1pid.Actual = motor1.rxpara.rVEL;
		PID_Calc(&motor1pid);
		motor1.sT = motor1pid.Out;
		Sent_MotorData(&motor1);	
		
//		printf("%.2f,%.2f,%.2f,%.2f\r\n",motor1pid.Target,motor1pid.Out,motor1.rxpara.rVEL,motor1.rxpara.rT);
		
	}   
}

// 重写fputc使用非阻塞版本
int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart1,(uint8_t*)&ch,1,HAL_MAX_DELAY);
    return ch;
}

//DMA转发数据回调函数
uint16_t temp;
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart->Instance == USART1)
	{
		printf("\r\n数据转发成功,未使用CPU\r\n");
		//回显
		HAL_UART_Transmit_DMA(&huart1,RXBuff,Size);		 
		//重新开启空闲中断
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1,RXBuff,Length);	
	}
}

/*以下内容不需要修改*/
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef clkinitstruct = {0};
  RCC_OscInitTypeDef oscinitstruct = {0};
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSEState        = RCC_HSE_ON;
  oscinitstruct.HSEPredivValue  = RCC_HSE_PREDIV_DIV1;
  oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */
