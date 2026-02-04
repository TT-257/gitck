#include "bsp_can.h"

CAN_HandleTypeDef hcan;

void MX_CAN_Init(void)
{
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 4;
  hcan.Init.Mode = CAN_MODE_NORMAL;//CAN_MODE_NORMAL/CAN_MODE_LOOPBACK
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_4TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_4TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = ENABLE;
  hcan.Init.AutoWakeUp = ENABLE;
  hcan.Init.AutoRetransmission = ENABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
//    Error_Handler();
  }
}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN GPIO Configuration
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX
    */
    GPIO_InitStruct.Pin = CAN_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(CAN_RX_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = CAN_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(CAN_TX_GPIO_Port, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN GPIO Configuration
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX
    */
    HAL_GPIO_DeInit(GPIOA, CAN_RX_Pin|CAN_TX_Pin);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
  }
}

/*CAN的过滤器配置*/
void CAN_Filter_Config_Default(void)
{
    CAN_FilterTypeDef sFilterConfig;
    
    /* 初始化过滤器配置 */
    sFilterConfig.FilterIdHigh = 0x0000;          // ID高16位
    sFilterConfig.FilterIdLow = 0x0000;           // ID低16位
    sFilterConfig.FilterMaskIdHigh = 0x0000;      // 屏蔽位高16位（0表示不关心）
    sFilterConfig.FilterMaskIdLow = 0x0000;       // 屏蔽位低16位（0表示不关心）
    sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;  // 分配到FIFO0
    sFilterConfig.FilterBank = 0;                 // 使用过滤器组0
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;       // 屏蔽模式
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;      // 32位模式
    sFilterConfig.FilterActivation = ENABLE;      // 使能过滤器
    sFilterConfig.SlaveStartFilterBank = 14;      // 对于双CAN设备，从14开始分配给CAN2
    
    /* 配置CAN过滤器 */
    if(HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
	{
//		Eror_Handler();
	}
}

void CAN_Init(void)
{
	MX_CAN_Init();
	CAN_Filter_Config_Default();
	HAL_CAN_Start(&hcan);
	HAL_CAN_ActivateNotification(&hcan,CAN_IT_RX_FIFO0_MSG_PENDING);	
}

void CAN_TxMsg(uint32_t StdId,uint8_t *pData,uint8_t Len)
{
	CAN_TxHeaderTypeDef tx_header;
	uint32_t txMailbox;//保留邮箱编号
	
	tx_header.StdId = StdId;
	tx_header.IDE = CAN_ID_STD;//标准id
	tx_header.RTR = CAN_RTR_DATA;//数据帧
	tx_header.DLC = Len;
	
	if(HAL_CAN_AddTxMessage(&hcan,&tx_header,pData,&txMailbox) != HAL_OK)
	{
		printf("CAN发送数据失败!\r\n");
		printf("报文ID:0x%x!\r\n",tx_header.StdId);
	}
}
