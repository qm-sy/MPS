#ifndef __GUI_H_
#define __GUI_H_

#include "sys.h"
#include "ST7789V2.h"
#include "modbus_rtu.h"
#include "KEY_CRL.h"

#define LED_ON 			1
#define LED_OFF 		0

#define AC220V_ON 	    1
#define AC220V_OFF 		0

#define POWER_ON        1
#define POWER_OFF       0

#define LEVEL_0         0
#define LEVEL_1         1
#define LEVEL_2         2
#define LEVEL_3         3
#define LEVEL_4         4
#define LEVEL_5         5
#define LEVEL_6         6

#define BEAT_ON         1
#define BEAT_OFF        0

#define SYNC_ON         1
#define SYNC_OFF        0

#define CONNECT         1
#define UNCONNECT       0

/*      需要跳动显示的内容   */
#define LED_ICON        1
#define FAN_ICON        2
#define BAKE_POWER      3
#define BAKE_FAN        4
#define TEMP1_ALARM     7
#define TEMP2_ALARM     8
#define TEMP3_ALARM     9

typedef struct  
{       
    uint8_t  fan_level;
    uint8_t  led_switch;
    uint8_t  ac220_switch;
    uint8_t  bake_fan_level;
    uint8_t  bake_power_level;
    uint8_t  sync_switch1;
    uint8_t  sync_switch2;
    uint8_t  temp1_alarm_value;
    uint8_t  temp2_alarm_value;
    uint8_t  temp3_alarm_value;
    uint8_t  temp1_value;
    uint8_t  temp2_value;
    uint8_t  temp3_value;
    uint8_t  envir_temp;
    uint8_t  envir_humidity;
    float    cc6903_24V;   
    float    cc6903_36V;   
    float    cc6903_42V;   

    bool connect_flag;
    bool power_switch;
}GUI_INFO;

typedef struct 
{
    bool    start_flag;
    bool    clear_flag;
    bool    enable_flag; 
    uint8_t selection;
}GUI_FLICKER;

extern GUI_INFO gui_info;
extern GUI_FLICKER gui_flicker;

void GUI_Info_Init( void );
void GUI_Icon_Init( void );

void connect_dis( void );
void sync_dis( void );
void dht11_dis( void );
void ac220_dis( void );
void temp_alarm_dis( void );
void temp_dis( void );
void led_dis( void );
void fan_dis( void );
void bake_dis( void );
void cc6903_dis( void );

void Icon_Flicker( void );

void gui_info_update( void );
void gui_info_refresh( void );

#endif

