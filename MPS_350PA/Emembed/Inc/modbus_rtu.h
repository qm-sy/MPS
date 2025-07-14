#ifndef __MODBUS_RTU_H_
#define __MODBUS_RTU_H_

#include "communication.h"
#include "ntc.h"
#include "gpio.h"
#include "power_crl.h"
#include "cc6903.h"
#include "eeprom_crl.h"
#include "dht11.h"

#define MY_ADDR     0x35

typedef struct 
{
    uint8_t  byte_info_H;                //DATA_H
    uint8_t  byte_info_L;                //DATA_L
    uint8_t  byte_cnt;                   //BYTE NUM*2
    uint8_t  send_value_addr;            //DATA1 H 位置
    uint8_t  rcv_value_addr;             //DATA1 H 位置
    uint16_t start_addr;                 //查询起始位置
}MODBIS_INFO;

typedef struct 
{    
    uint8_t  temp1_value;
    uint8_t  temp2_value;
    uint8_t  temp3_value;
    uint8_t  envir_temp;
    uint8_t  envir_humidity;
    uint16_t  cc6903_24V;   
    uint16_t  cc6903_36V;   
    uint16_t  cc6903_42V;  
}SLAVE_04;

typedef struct 
{    
    uint8_t  led_switch;   
    uint8_t  fan_level;             
    uint8_t  ac220_switch;
    uint8_t  bake_fan_level;
    uint8_t  bake_power_level;
    uint8_t  sync_switch2;             //同步标志位
    uint8_t  sync_switch1;             //同步标志位
    uint8_t  temp1_alarm_value;
    uint8_t  temp2_alarm_value;
    uint8_t  temp3_alarm_value;

    uint8_t  power_switch;
}SLAVE_06;

extern MODBIS_INFO modbus;
extern SLAVE_04 slave_04;
extern SLAVE_06 slave_06;

void Modbus_Event( void );
void Modbus_Fun3( void );
void Modbus_Fun4( void );
void Modbus_Fun6( void );
void Modbus_Fun16( void );
void slave_to_master(uint8_t code_num,uint8_t length);
uint16_t MODBUS_CRC16(uint8_t *buf, uint8_t length);
void slave_scan( void );

#endif