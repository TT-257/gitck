#ifndef __BSP_CAN_H__
#define __BSP_CAN_H__

#include "main.h"

#define CAN_RX_Pin GPIO_PIN_11
#define CAN_RX_GPIO_Port GPIOA
#define CAN_TX_Pin GPIO_PIN_12
#define CAN_TX_GPIO_Port GPIOA

extern CAN_HandleTypeDef hcan;

void CAN_Init(void);
void CAN_TxMsg(uint32_t StdId,uint8_t *pData,uint8_t Len);

#endif /* __BSP_CAN_H__ */
