#include "app_motorctrl.h"
#include <string.h>
#include <stdio.h>

MotorData motor1;

/*初始化motor1*/
void Motor_Init(void)
{
	memset(&motor1,0,sizeof(motor1));
	
	/*必备*/	
	motor1.MST_ID = 0x12;
	motor1.ID = 0x02;
	motor1.PMAX = 6.28f;
	motor1.VMAX = 45.0f;
	motor1.TMAX = 18.0f;
	motor1.KD_MAX = 5.0f;
	motor1.kd = 0.0f;
	motor1.KP_MAX = 500.0f;
	motor1.kp = 0.0f;
	
	/*调试*/
//	motor1.sT = 0.05;	
//	motor1.sVEL = 3.0f;
}

int float_to_uint(float x,float xMAX,uint8_t bit)
{	
	return (int)((x+xMAX)*((1<<bit)-1)/(2*xMAX));
}

float uint_to_float(int x,float xMAX,uint8_t bit)
{	
	return (float)((x*2*xMAX-((1<<bit)-1)*xMAX) / ((1<<bit)-1));
}

/*获取电机数据*/
void Get_MotorData(uint8_t Data[],MotorData *motor)
{
	int temp = 0;
	temp = (Data[1]<<8) | Data[2];	
	motor->rxpara.rPOS = uint_to_float1(temp,-(motor->PMAX),motor->PMAX,16);
	temp = 0;
	temp = (Data[3]<<4) | (Data[4]>>4);
	motor->rxpara.rVEL = uint_to_float(temp,motor->VMAX,12);
	temp = 0;
	temp = ((Data[4]<<8) | Data[5]) & 0x00000FFF;
	motor->rxpara.rT = uint_to_float(temp,motor->TMAX,12);
		
}

/*使能电机*/
void enable_motor(MotorData* motor)
{
	uint8_t Data[8] = {0};
	Data[0] = 0xFF;
    Data[1] = 0xFF;
    Data[2] = 0xFF;
    Data[3] = 0xFF;
    Data[4] = 0xFF;
    Data[5] = 0xFF;
    Data[6] = 0xFF;
	Data[7] = 0xFC;	
	
	FDCAN_TxMsg(motor->ID,Data);
}

/*失能电机*/
void disable_motor(MotorData* motor)
{
	uint8_t Data[8] = {0};
	Data[0] = 0xFF;
    Data[1] = 0xFF;
    Data[2] = 0xFF;
    Data[3] = 0xFF;
    Data[4] = 0xFF;
    Data[5] = 0xFF;
    Data[6] = 0xFF;
	Data[7] = 0xFD;
	
	FDCAN_TxMsg(motor->ID,Data);
}

/*发送控制电机的can报文*/
void Sent_MotorData(MotorData* motor)
{
	uint8_t Data[8] = {0};
	uint16_t temp = 0;
	
	temp = float_to_uint(motor->sPOS,motor->PMAX,16);	
	Data[0] = temp >> 8;
	Data[1] = temp;
	
	temp = float_to_uint(motor->sVEL,motor->VMAX,12);
	Data[2] = temp >> 4;
	Data[3] = temp << 4;
	
	temp = float_to_uint1(motor->kp,0,motor->KP_MAX,12);
	Data[3] |= (temp >> 8)&0x000F;
	Data[4]	= temp;
	
	temp = float_to_uint1(motor->kd,0,motor->KD_MAX,12);
	Data[5] = temp >> 4;
	Data[6]	= temp << 4;

	temp = float_to_uint(motor->sT,motor->TMAX,12);
	Data[6] |= ((temp >> 8) & 0x000F);
	Data[7]	= temp;

	/*监听报文内容*/
//	for(uint8_t i = 0;i < 8;i ++)
//	{
//		printf("%x\r\n",Data[i]);
//	}
//	printf("\r\n");

	FDCAN_TxMsg(motor->ID,Data);
}

int float_to_uint1(float x_float, float x_min, float x_max, int bits)
{
    /* Converts a float to an unsigned int, given range and number of bits */
    float span = x_max - x_min;
    float offset = x_min;
    return (int) ((x_float-offset)*((float)((1<<bits)-1))/span);
}

float uint_to_float1(int x_int, float x_min, float x_max, int bits)
{
    /* converts unsigned int to float, given range and number of bits */
    float span = x_max - x_min;
    float offset = x_min;
    return ((float)x_int)*span/((float)((1<<bits)-1)) + offset;
}