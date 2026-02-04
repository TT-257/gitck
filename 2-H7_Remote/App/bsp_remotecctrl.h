#ifndef __BSP_REMOTECCTRL_H__
#define __BSP_REMOTECCTRL_H__

#include "main.h"

#define DataLen 36
#define halfDataLen 18

void Remote_Init(void);
void DoubleBuffer_Init(UART_HandleTypeDef *huart,uint8_t* DstAddress,uint8_t* SecondMemAddress,uint32_t DataLength);


#endif /* __BSP_REMOTECCTRL_H__ */
