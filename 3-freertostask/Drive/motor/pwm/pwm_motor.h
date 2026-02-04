#ifndef __PWM_MOTOR_H
#define __PWM_MOTOR_H

#include "main.h"

void pwm_motor_init(void);
void pwm_motor_ctrl_send(int16_t ccr);


#endif
