#ifndef __BSP_USART_H__
#define __BSP_USART_H__

#include "main.h"

extern UART_HandleTypeDef huart1;

void MX_USART1_UART_Init(void);
void sent_vofa(float f1,float f2,float f3,float f4);

#endif /* __BSP_USART_H__ */
