#ifndef __APP_MOTORCTRL_H__
#define __APP_MOTORCTRL_H__

#include "main.h"

typedef struct//接收电机数据
{
	float rPOS;//位置	
	float rVEL;//速度
	float rT;  //力矩
	
	uint8_t ERR;
}MotorRxData;

typedef struct//发送电机数据
{
	MotorRxData rxpara;
	
	float sPOS;//位置	
	float sVEL;//速度
	float sT;  //力矩
	
	float PMAX;
	float VMAX;
	float TMAX;
	
	float KP_MAX;
	float KD_MAX;
	
	float kp;
	float kd;
	
	uint8_t MST_ID;
	uint8_t ID;
}MotorData;

extern MotorData motor1;

/*初始化motor1*/
void Motor_Init(void);

int float_to_uint(float x,float xMAX,uint8_t bit);
float uint_to_float(int x,float xMAX,uint8_t bit);
int float_to_uint1(float x_float, float x_min, float x_max, int bits);
float uint_to_float1(int x_int, float x_min, float x_max, int bits);

/*获取和显示电机数据*/
void Get_MotorData(uint8_t Data[],MotorData *motor);
/*使能电机*/
void enable_motor(MotorData* motor);
/*失能电机*/
void disable_motor(MotorData* motor);
/*发送控制电机的can报文*/
void Sent_MotorData(MotorData* motor);

#endif /* __APP_MOTORCTRL_H__ */
