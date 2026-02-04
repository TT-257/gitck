#ifndef __REMOTECTRL_H__
#define __REMOTECTRL_H__

#include "main.h"

#define Median 1024

typedef struct{
	int ch1;
	int ch2;
	int ch3;
	int ch4;
	
	uint8_t s1;
	uint8_t s2;		
}Remote_DataTypeDef;

void Remote_GetData(volatile const uint8_t* DoubleBuffer,Remote_DataTypeDef* RemData);

#endif /* __REMOTECTRL_H__ */
