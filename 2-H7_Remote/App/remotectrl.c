#include "remotectrl.h"

void Remote_GetData(volatile const uint8_t* DoubleBuffer,Remote_DataTypeDef* RemData)
{
	RemData->ch1 = (DoubleBuffer[0] | (DoubleBuffer[1] << 8)) & 0x07ff;
	RemData->ch2 = ((DoubleBuffer[1] >> 3) | (DoubleBuffer[2] << 5)) & 0x07ff;
	RemData->ch3 = ((DoubleBuffer[2] >> 6) | (DoubleBuffer[3] << 2) | (DoubleBuffer[4] << 10)) & 0x7ff;
	RemData->ch4 = ((DoubleBuffer[4] >> 1) | (DoubleBuffer[5] << 7)) & 0x07ff;
	
	RemData->s1 = (DoubleBuffer[5] >> 4) & 0x0003;
	RemData->s2 = (DoubleBuffer[5] >> 6) & 0x0003;
	
	RemData->ch1 -= Median;
	RemData->ch2 -= Median;
	RemData->ch3 -= Median;
	RemData->ch4 -= Median;
}
