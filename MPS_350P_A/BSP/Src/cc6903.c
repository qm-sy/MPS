#include "cc6903.h"

CURRENT current;

void bubble_sort(uint16_t arr[], uint16_t n) 
{
    uint8_t i, j;
    uint16_t temp;
    
    for (i = 0; i < n-1; i++) {
        for (j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                // 交换元素
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

void current_init( void )
{
    slave_04.cc6903_24V = 1360;
    slave_04.cc6903_36V = 1360;
    slave_04.cc6903_42V = 1360;
}
/**
 * @brief	获取电流值
 *
 * @param   channel_x：电流通道1~3
 *
 * @return  current 
**/
uint16_t get_current( uint8_t channel_x)
{
    uint8_t i,t;
    uint16_t adc_val = 0;
    uint16_t adc_tab[24];
    /* 1, 取10次adc_val                           */
    for( t = 0; t < 24; t++ )
	{
		adc_tab[t] = Get_ADC_12bit(channel_x);
	}	       
    bubble_sort(adc_tab,24);
    for(i = 6; i < 18; i++)
    {
      adc_val += adc_tab[i];
    }
    adc_val = adc_val/12;

    return adc_val;
}