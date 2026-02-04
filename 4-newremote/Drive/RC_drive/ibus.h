#ifndef __IBUS_H
#define __IBUS_H

#include <stdint.h>
#include "main.h"

typedef struct{
	int16_t	ch[18];
}ibus_msg;

extern int ibus_parse( ibus_msg *msg, uint8_t *src );
extern void ibus_show( ibus_msg *msg );

#endif