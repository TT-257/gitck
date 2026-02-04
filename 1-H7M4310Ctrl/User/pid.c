/**
  ****************************(C) COPYRIGHT 2016 DJI****************************
  * @file       pid.c/h
  * @brief      pid实现函数，包括初始化，PID计算函数，
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. 完成
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2016 DJI****************************
  */

#include "pid.h"
//#include "user_sys_config.h"

#define DEBUG

void LimitMax(fp32 *val, fp32 max)
{
    if (*val > max)
        *val = max;
    else if (*val < -max)
        *val = -max;
}

void PID_init(PidTypedef *pid, uint8_t mode, const fp32 PID[3], fp32 max_out, fp32 max_iout)
{
	if (pid == NULL || PID == NULL)
	{
		return;
	}
	#ifdef DEBUG
		pid->Kp = PID[0];
		pid->Ki = PID[1];
		pid->Kd = PID[2];
	#endif
	if(pid->Initlized != true)
	{
		pid->pid_mode = mode;
		pid->Kp = PID[0];
		pid->Ki = PID[1];
		pid->Kd = PID[2];
		pid->max_out = max_out;
		pid->max_iout = max_iout;
		pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0.0f;
		pid->error[0] = pid->error[1] = pid->error[2] = pid->Pout = pid->Iout = pid->Dout = pid->out = 0.0f;
		pid->Initlized = true;
	}
}


fp32 PID_Calc(PidTypedef *pid, fp32 fdb, fp32 ref)
{
    if (pid == NULL)
    {
        return 0.0f;
    }

    pid->error[2] = pid->error[1];
    pid->error[1] = pid->error[0];
    pid->set = ref;
    pid->fdb = fdb;
    pid->error[0] = ref - fdb;
	if(pid->Initlized == true)
	{
		switch(pid->pid_mode)
		{
			case PID_POSITION:
			{
				pid->Pout = pid->Kp * pid->error[0];
				pid->Iout += pid->Ki * pid->error[0];
				pid->Dbuf[2] = pid->Dbuf[1];
				pid->Dbuf[1] = pid->Dbuf[0];
				pid->Dbuf[0] = (pid->error[0] - pid->error[1]);
				pid->Dout = pid->Kd * pid->Dbuf[0];
				LimitMax(&pid->Iout, pid->max_iout);
				pid->out = pid->Pout + pid->Iout + pid->Dout;
				LimitMax(&pid->out, pid->max_out);
			}break;
			
			case PID_DELTA:
			{
				pid->Pout = pid->Kp * (pid->error[0] - pid->error[1]);
				pid->Iout = pid->Ki * pid->error[0];
				pid->Dbuf[2] = pid->Dbuf[1];
				pid->Dbuf[1] = pid->Dbuf[0];
				pid->Dbuf[0] = (pid->error[0] - 2.0f * pid->error[1] + pid->error[2]);
				pid->Dout = pid->Kd * pid->Dbuf[0];
				pid->out += pid->Pout + pid->Iout + pid->Dout;
				LimitMax(&pid->out, pid->max_out);
			}break;
		}
	}
	else
	{
	    pid->error[0] = pid->error[1] = pid->error[2] = 0.0f;
		pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0.0f;
		pid->out = pid->Pout = pid->Iout = pid->Dout = 0.0f;
		pid->fdb = pid->set = 0.0f;	
	}
    return pid->out;
}

void PID_clear(PidTypedef *pid)
{
    if (pid == NULL)
    {
        return;
    }
	pid->Initlized = false;
    pid->error[0] = pid->error[1] = pid->error[2] = 0.0f;
    pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0.0f;
    pid->out = pid->Pout = pid->Iout = pid->Dout = 0.0f;
    pid->fdb = pid->set = 0.0f;
}
