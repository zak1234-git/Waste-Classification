#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

void Serial_Init(void);
uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);


#endif
