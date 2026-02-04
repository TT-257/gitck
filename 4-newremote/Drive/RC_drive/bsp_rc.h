#ifndef __BSP_RC_H__
#define __BSP_RC_H__

#include "main.h"

#define DataLen 64
#define halfDataLen 32

void Remote_Init(void);
void DoubleBuffer_Init(UART_HandleTypeDef *huart,uint8_t* DstAddress,uint8_t* SecondMemAddress,uint32_t DataLength);


#endif /* __BSP_RC_H__ */
