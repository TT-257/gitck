#include "ibus.h"
#include <stdio.h>

/*数据解析*/
int ibus_parse( ibus_msg *msg, uint8_t *src ){

	uint16_t	calc_sum = 0;
	uint16_t 	ori_sum;
	uint8_t  	i;

	if( src[0] != 0x20 )
		return -1;
	if( src[1] != 0x40 )
		return -1;

	for( i = 0; i < 30; i++ )
		calc_sum += src[i];
	calc_sum ^= 0xFFFF;
	ori_sum = src[30] + (src[31]<<8);

	// printf( "calc_sum: %04x %04x\n", (unsigned)calc_sum, (unsigned)ori_sum );
	if( calc_sum != ori_sum )
		return -1;

#define IBUS_PARSE_CH1_14(x) 	msg->ch[x] = src[(x)*2+2] + ((src[(x)*2+3]&0xF)<<8)

	IBUS_PARSE_CH1_14(0);
	IBUS_PARSE_CH1_14(1);
	IBUS_PARSE_CH1_14(2);
	IBUS_PARSE_CH1_14(3);
	IBUS_PARSE_CH1_14(4);
	IBUS_PARSE_CH1_14(5);
	IBUS_PARSE_CH1_14(6);
	IBUS_PARSE_CH1_14(7);
	IBUS_PARSE_CH1_14(8);
	IBUS_PARSE_CH1_14(9);
	IBUS_PARSE_CH1_14(10);
	IBUS_PARSE_CH1_14(11);
	IBUS_PARSE_CH1_14(12);
	IBUS_PARSE_CH1_14(13);

#define IBUS_PARSE_CH15_18(x)		msg->ch[x] = ((src[(x-14)*6+3+0]&0xF0)>>4) | (src[(x-14)*6+3+2]&0xF0) | ((src[(x-14)*6+3+4]&0xF0)<<4)

	IBUS_PARSE_CH15_18(14);
	IBUS_PARSE_CH15_18(15);
	IBUS_PARSE_CH15_18(16);
	IBUS_PARSE_CH15_18(17);

	return 0;
}

void ibus_show( ibus_msg *msg ){
	uint8_t	i;
	for( i = 0; i < 14; i++ ){
		printf( "ch%d(%hu)  ", i, (int)msg->ch[i] );
	}
	printf( "\r\n" );
}