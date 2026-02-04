#include "app_pid.h"
#include <math.h>
#include <string.h>

PID motor1pid;
PID motor1pidLocation;

float pid_array[3]={0,0,0};

void pid_init(void)
{
	memset(&motor1pid,0,sizeof(motor1pid));
	
	motor1pid.Kp = 0.11f;
	motor1pid.Ki = 0.0008f;
	motor1pid.Kd = 0.0f;
	motor1pid.OutLimit = 10.0f;
	
	motor1pidLocation.OutLimit = 25.0f;
	
//	motor1pid.
//	motor1pid
//	motor1pid
//	motor1pid
	
}

void pid2_init(void)
{
//	motor1pid.Kp = pid_array[0];
//	motor1pid.Ki = pid_array[1];
//	motor1pid.Kd = pid_array[2];
	         
	motor1pidLocation.Kp = pid_array[0];
	motor1pidLocation.Ki = pid_array[1];
	motor1pidLocation.Kd = pid_array[2];
	
}
	
	
void PID_Calc(PID* pid)
{
//	pid->Actual = (float)Speed;
	
	pid->Error1 = pid->Error0;
	pid->Error0 = pid->Target - pid->Actual;
	
	if (fabs(pid->Ki) > EPSILON)
	{
		pid->ErrorInt += pid->Error0;
	}
	else
	{
		pid->ErrorInt = 0;
	}
	
///*积分限幅*/
//if(ErrorInt > OutLimit){ErrorInt = OutLimit;}
//if(ErrorInt < OutLimit){ErrorInt = -OutLimit;}

	pid->Out = pid->Kp * pid->Error0 + pid->Ki * pid->ErrorInt + pid->Kd * (pid->Error0 - pid->Error1);
	
	if(pid->Out >= pid->OutLimit){pid->Out = pid->OutLimit;}
	if(pid->Out <= -pid->OutLimit){pid->Out = -pid->OutLimit;}
}
