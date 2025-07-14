#include "power_crl.h"

AC_CTRL ac_ctrl;

/**
 * @brief	移相触发调用结构体初始化
 *
 * @param   
 *
 * @return  void
**/
void Power_Statu_Init( void )
{
    ac_ctrl.zero_flag  = 0;
    AC_Out1 = AC_Out2 = AC_Out3 = AC_Out4 = 1;
}

/**
 * @brief	外部中断2中断处理函数
 *
 * @param   
 *
 * @return  void
**/
void INT2_ISR( void ) interrupt 10
{
    /* 1, 检测到外部中断后，等待THL\TLI后触发TIM1中断       */
    TL1 = ac_ctrl.time_delay;				
	TH1 = ac_ctrl.time_delay >> 8;				

    ac_ctrl.zero_flag = 1;

    /* 2, 定时器1开始计时，打开TIM1中断                     */
    TR1 = 1;				     
    ET1 = 1; 
}

/**
 * @brief	Timer1中断处理函数
 *
 * @param   
 *
 * @return  void
**/
void Tim1_ISR( void ) interrupt 3   //1ms
{
    /* 1, 中断触发后，power_ch电平 由高电平变为低电平           */
    if( ac_ctrl.zero_flag == 1 )
    {
        ac_ctrl.zero_flag = 0;
        AC_Out4 = 0;

         /* 2, 设置下一次Timer1中断触发所需时间，即脉冲时间       */
        TL1 = 0xF7;				
        TH1 = 0xFF;				
    }else
    {
        /* 3, 下一次进入Timer1中断，power_ch电平 由低电平变为高电平，完成一次10us脉冲，即斩波*/
        AC_Out4 = 1;

        /* 4, 定时器1停止计时，关闭TIM1中断，等待下一次外部中断     */
        TR1 = 0;				    
        ET1 = 0; 
    }
}

/**
 * @brief	220V输出功率控制函数 
 *
 * @param   power_level：输出功率百分比  0%~100%
 *
 * @return  void
**/
void bake_power_ctrl( uint8_t power_level )
{
    ac_ctrl.time_delay = 56500 + 90 * power_level;
}

/**
 * @brief	220V输出功率控制函数 
 *
 * @param   power_level：输出功率百分比  0%~100%
 *
 * @return  void
**/
void bake_fan_ctrl( uint8_t level )
{
    PWMB_CCR8  = level * 184;
}

/**
 * @brief	24V LED开关控制函数
 *
 * @param   on_off：0：关闭 1：开启
 *
 * @return  void
**/
void led_ctrl( uint8_t on_off )
{
    if( on_off == LED_ON )
    {
        DC_24V = 0;
    }else
    {
        DC_24V = 1;
    }
}

/**
 * @brief	24V PWM风扇档位控制函数
 *
 * @param   level :风扇档位 0~6档
 *
 * @return  void
**/
void fan_ctrl( uint8_t level )
{
    PWMB_CCR7  = level * 184;
}

/**
 * @brief	24V PWM风扇档位控制函数
 *
 * @param   level :风扇档位 0~6档
 *
 * @return  void
**/
void ac220_ctrl( uint8_t on_off )
{
    if( on_off == POWER_ON )
    {
        AC_Out1 = AC_Out2 = AC_Out3 = 0;
    }else
    {
        AC_Out1 = AC_Out2 = AC_Out3 = 1;
    }
}

/**
 * @brief	电源开关（受屏幕控制） 
 *
 * @param   
 *
 * @return  void
**/
void power_switch_ctrl( uint8_t power_switch )
{
    if( power_switch == 0 )
    {
        PWMB_BKR = 0x00; 
        INTCLKO  = 0X00; 
        led_ctrl(LED_OFF);
        AC_Out1 = AC_Out2 = AC_Out3 = 1;
    }else
    {
        PWMB_BKR = 0x80;    //PWM控制
        INTCLKO |= 0X10;             //外部中断控制
        led_ctrl(slave_06.led_switch);
    }
}

/**
 * @brief	同步控制函数：无外部信号时 PWM风扇和220V输出关闭
 *
 * @param   
 *
 * @return  void
**/
void sync_ctrl( void )
{
    if( slave_06.sync_switch2 == 1 )
    { 
        INTCLKO  = 0X00; 
        AC_Out1 = AC_Out2 = AC_Out3 = 1;
    }else
    {
        INTCLKO |= 0X10;             //外部中断控制
    }
}