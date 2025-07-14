#include "POWER_CRL.h"
#include "tim.h"

volatile uint8_t zero_flag = 0;
volatile uint8_t power_ch1_statu = 0;
volatile uint8_t power_ch2_statu = 0;
volatile uint8_t power_ch3_statu = 0;
volatile uint8_t power_ch4_statu = 0;
volatile uint16_t power_phase_delay = 0;
volatile uint16_t signal_flag = 0;
/**

  
 * @brief	外部中断6回调函数 用于接收数据
 * 
 * @param   huart：标识触发回调函数的UART外设                     
 * 
  @return  void
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if( GPIO_Pin == ZERO_Pin )
    {
        zero_flag = 1;
    }
	
}

void power_crl(uint16_t power_num)
{
    if( power_num == 0 )
    {
        HAL_TIM_Base_Stop_IT(&htim7);			      //TIM7失能
    }else
    {
        HAL_TIM_Base_Start_IT(&htim7);			      //TIM7失能
        power_phase_delay = (100 - power_num) * 83 / 10;
    }  
    
}

void led_crl( void )
{
    if(gui_info.led_switch == 0)
    {
        HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
    }else
    {
        HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
    }
}

void signal_scan( void )
{
    uint16_t signal[5];
    static uint8_t statu_now = 0;
    static uint8_t statu_flag = 0;

    Get_ADC12bitResult(signal);
    signal_flag = signal[0];

    if( statu_now == statu_flag )
    {
        if( signal_flag > 2000 )
        {
            gui_info.sync_switch2 = 0;
        }else
        {
            gui_info.sync_switch2 = 1;
        }
        if( gui_info.sync_switch1 == 1 ) 
        {
            delay_ms(30);
            write_slave_06(SYNC_ADDR,gui_info.sync_switch2,gui_info.sync_switch1);

            statu_now = 1- statu_now;
        }
    } 
    if( signal_flag > 2000 )
    {
        statu_flag = 1;
    }else
    {
        statu_flag = 0;
    }
    delay_ms(10);
}
