#ifndef __POWER_CRL_H_
#define __POWER_CRL_H_

#include "sys.h"
#include "gpio.h"
#include "modbus_rtu.h"

#define LED_ON    1
#define LED_OFF   0

#define POWER_ON  1
#define POWER_OFF 1

typedef struct 
{
    uint8_t  enable;
    uint8_t  signal_flag;         //24V输入检测
    uint8_t  connect_flag;
    uint8_t  zero_flag;          //220V输入触发标志位
    uint16_t time_delay;         //移相延时
}AC_CTRL;

extern AC_CTRL ac_ctrl;


void Power_Statu_Init( void );
void fan_ctrl( uint8_t level );
void led_ctrl( uint8_t on_off );
void ac220_ctrl( uint8_t on_off );
void bake_power_ctrl( uint8_t power_level );
void bake_fan_ctrl( uint8_t level );
void power_switch_ctrl( uint8_t power_switch );
void sync_ctrl( void );

#endif