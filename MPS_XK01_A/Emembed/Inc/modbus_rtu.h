#ifndef __MODBUS_RTU_H_
#define __MODBUS_RTU_H_

#include "usart.h"
#include "main.h"
#include "PWM_CRL.h"
#include "communication.h"
#include "delay.h"
#include "GUI.h"

#define SLAVE_ADDR      0x35

#define FUN_03      0X03
#define FUN_04      0X04
#define FUN_06      0X06
#define FUN_16      0X10

#define START_REG_H_03      0
#define START_REG_L_03      0
#define REG_NUM_H_03        0
#define REG_NUM_L_03        7

#define START_REG_H_04      0
#define START_REG_L_04      0
#define REG_NUM_H_04        0
#define REG_NUM_L_04        6

#define LED_ADDR            0X00
#define FAN_ADDR            0X01
#define AC220_ADDR          0X02
#define BAKE_ADDR           0X03
#define TEMP12_ALARM_ADDR   0X04
#define TEMP3_ALARM_ADDR    0X05
#define SYNC_ADDR           0X06
#define POWER_SWITCH_ADDR   0x07


typedef struct  
{
    uint8_t send_buf[48];
    uint8_t rcv_buf[48];

    uint8_t fan_info;               //40001
    uint8_t led_info;               //40002
    uint8_t channel_info;             //40003
    uint8_t power_info;       //40004
    uint8_t sync_info;
    uint8_t mode_num;
    uint8_t temp;       //40004
    uint8_t alarm_temp;       //40005

    uint8_t envir_temp;
    uint8_t envir_humidity;

    uint8_t signal_in_flag;

    uint8_t modbus_04_scan_flag;
    uint8_t modbus_04_scan_allow;
}MODBUS_INFO;

extern MODBUS_INFO modbus;

void modbus_send_data( uint8_t *buf , uint8_t len );

void Modbus_Event( void );
void Modbus_Fun3( void );
void Modbus_Fun4( void );

void read_slave_03( void );
void read_slave_04( void );
void write_slave_06( uint16_t reg_addr, uint8_t reg_val_H, uint8_t reg_val_L);
void write_slave_16( void );

uint16_t MODBUS_CRC16(uint8_t *buf, uint8_t length);

#endif
