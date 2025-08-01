#ifndef __CC6903_H_
#define __CC6903_H_

#include "adc.h"
#include "stdio.h"
#include "modbus_rtu.h"

#define I_OUT1      10
#define I_OUT2      11
#define I_OUT3      12

typedef struct 
{
    uint16_t  current1;
    uint16_t  current2;
    uint16_t  current3;          
}CURRENT;

extern CURRENT current;

void current_init( void );
uint16_t get_current( uint8_t channel_x );

#endif