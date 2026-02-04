#include "bsp_rc.h"
#include "ibus.h"

extern UART_HandleTypeDef huart7;
uint8_t DoubleBuffer[2][DataLen];
ibus_msg Remote_Data;

uint32_t cnt = 0;
/*UART空闲中断回调函数*/
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	cnt++;
	if(((((DMA_Stream_TypeDef  *)huart->hdmarx->Instance)->CR) & DMA_SxCR_CT) == RESET )
	{
		__HAL_DMA_DISABLE(huart->hdmarx);
		
		((DMA_Stream_TypeDef  *)huart->hdmarx->Instance)->CR |= DMA_SxCR_CT;
		
		__HAL_DMA_SET_COUNTER(huart->hdmarx,DataLen);

		if(Size == halfDataLen)
		{
			ibus_parse(&Remote_Data,DoubleBuffer[0]);
		}
		
		__HAL_DMA_ENABLE(huart->hdmarx);
	}else 
	{
		__HAL_DMA_DISABLE(huart->hdmarx);
		
		((DMA_Stream_TypeDef  *)huart->hdmarx->Instance)->CR &= ~DMA_SxCR_CT;
		
		__HAL_DMA_SET_COUNTER(huart->hdmarx,DataLen);

		if(Size == halfDataLen)
		{
			ibus_parse(&Remote_Data,DoubleBuffer[0]);
		}		

		__HAL_DMA_ENABLE(huart->hdmarx);
	}	
}

/*配置DMA双缓冲区*/
void DoubleBuffer_Init(UART_HandleTypeDef *huart,uint8_t* DstAddress,uint8_t* SecondMemAddress,uint32_t DataLength)
{
	huart->ReceptionType = HAL_UART_RECEPTION_TOIDLE;
	huart->RxEventType = HAL_UART_RXEVENT_IDLE;
	
	huart->RxXferSize = DataLength;
	
	SET_BIT(huart->Instance->CR3,USART_CR3_DMAR);
	
	__HAL_UART_ENABLE_IT(huart,UART_IT_IDLE);
		
	HAL_DMAEx_MultiBufferStart(huart->hdmarx,(uint32_t)&huart->Instance->RDR,(uint32_t)DstAddress,(uint32_t)SecondMemAddress,DataLength);
}

void Remote_Init(void)
{
	DoubleBuffer_Init(&huart7,DoubleBuffer[0],DoubleBuffer[1],DataLen);
}
