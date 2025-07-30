#ifndef __POWER_CRL_H_
#define __POWER_CRL_H_

#include "main.h"
#include "GUI.h"
#include "adc.h"

extern volatile uint8_t zero_flag;
extern volatile uint8_t RX1_485_statu;
extern volatile uint8_t power_ch2_statu;
extern volatile uint8_t power_ch3_statu;
extern volatile uint8_t power_ch4_statu;
extern volatile uint16_t power_phase_delay;
extern volatile uint16_t signal_flag; 

void power_crl(uint16_t power_num);
void buzzer_crl( void );
void led_crl( void );
void signal_scan( void );

#endif
