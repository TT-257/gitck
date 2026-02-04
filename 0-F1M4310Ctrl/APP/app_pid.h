#ifndef __APP_PID_H__
#define __APP_PID_H__

#include "main.h"

#define EPSILON 0.0001

typedef struct
{
	float Kp;
	float Ki;
	float Kd;
	
	float Target;
	float Actual;
	float Out;
	
	float Error0;
	float Error1;
	float ErrorInt;
	
	float OutLimit;
}PID;

extern PID motor1pid;
extern PID motor1pidLocation;

void PID_Calc(PID* pid);
void pid_init(void);
void pid2_init(void);



#endif /* __APP_PID_H__ */
