#ifndef __EEPROM_CRL_H_
#define __EEPROM_CRL_H_

#include "eeprom.h"
#include "sys.h"
#include "power_crl.h"
#include "ntc.h"
#include "stdio.h"
#include "modbus_rtu.h"


#define LED_ADDR              0x0000
#define FAN_ADDR              0x0001
#define AC220_ADDR            0x0002
#define BAKE_POWER_ADDR       0x0003
#define BAKE_FAN_ADDR         0x0004
#define TEMP_ALARM1           0X0005
#define TEMP_ALARM2           0X0006
#define TEMP_ALARM3           0X0007
#define SYNC_ADDR             0X0008
#define EEPROM_STATU_JUDGE    0X0010         

void eeprom_statu_judge( void );
void eeprom_data_record( void );
void eeprom_data_init( void );

#endif