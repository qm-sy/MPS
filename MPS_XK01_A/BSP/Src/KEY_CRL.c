#include "KEY_CRL.h"

KEY key;

void key_init( void )
{

}

void jump_to_init( void )
{
    /*       无图标跳动     */
    gui_flicker.enable_flag = BEAT_OFF;

    /*       可继续查询从机状态     */
    modbus.modbus_04_scan_allow = 1;

    /*       复位图标跳动起始点     */
    gui_flicker.selection = 0;

}

void key_scan( void )
{
    uint8_t key_value;

    /*       50ms检测一次     */
    if(key.key_scan_flag == 1)
	{
		key_value = (B1_VAL) | (B2_VAL<<1) | (B3_VAL<<2) | (B4_VAL<<3) | (B5_VAL<<4);
	}
    switch (key_value)
    {
        case KEY1:      KEY1_press();       break;

        case KEY2:      KEY2_press();       break;

        case KEY3:      KEY3_press();       break;

        case KEY4:      KEY4_press();       break;

        case KEY5:      KEY5_press();       break;

        default:                            break;
    }
}

void KEY1_press()
{
    uint8_t key_value = KEY1;
    uint8_t key1_delay_cnt = 100;

    if( gui_info.power_switch == true )
    {
        buzzer_open();
        delay_ms(10);   
        /*               KEY1长按                 */
        while((key_value == KEY1)&&(key1_delay_cnt!=0))        
        {
            key_value = (B1_VAL) | (B2_VAL<<1) | (B3_VAL<<2) | (B4_VAL<<3) | (B5_VAL<<4);
            key1_delay_cnt--;
            delay_ms(10); 
        }
        if( key1_delay_cnt == 0 )                   //长按1s后跳转报警温度调节界面
        {
            gui_flicker.enable_flag = true;
            gui_flicker.selection = TEMP1_ALARM;              
            
            buzzer_close();
            delay_ms(500);
        }else/*         KEY1短按                  */
        {
            gui_info.ac220_switch = 1 - gui_info.ac220_switch;                 //调节通道
            ac220_dis();    
            write_slave_06(AC220_ADDR,0X00,gui_info.ac220_switch);
        }
        buzzer_close();
        key.key_scan_flag = 0;
    }
}

void KEY2_press()
{
    if( gui_info.power_switch == true )
    {
        buzzer_open();
        delay_ms(10);
        if( gui_flicker.enable_flag == 0 )
        {
            if( gui_info.fan_level > 0 )
            {
                gui_info.fan_level -= 1;
                write_slave_06(FAN_ADDR,0x00,gui_info.fan_level);
            }
            fan_dis();
        }else
        {
            switch(gui_flicker.selection)
            {
                case LED_ICON:
                    gui_info.led_switch = LED_OFF;
                    write_slave_06(LED_ADDR,0X00,gui_info.led_switch);
                    led_dis();

                    break;

                case FAN_ICON:
                    if( gui_info.fan_level > 0)
                    {
                        gui_info.fan_level -= 1;
                        write_slave_06(FAN_ADDR,0X00,gui_info.fan_level);
                    }
                    fan_dis();
                    break;

                case BAKE_POWER:
                    if( gui_info.bake_power_level > 20 )
                    {
                        gui_info.bake_power_level -= 5;
                        write_slave_06(BAKE_ADDR,gui_info.bake_power_level,gui_info.bake_fan_level);
                    }
                    bake_dis();

                    break;

                case BAKE_FAN:
                    if( gui_info.bake_fan_level > 0 )
                    {
                        gui_info.bake_fan_level -= 1;
                        write_slave_06(BAKE_ADDR,gui_info.bake_power_level,gui_info.bake_fan_level);
                    }
                    bake_dis();

                    break;  

                case TEMP1_ALARM:
                    if( gui_info.temp1_alarm_value > 20 )
                    {
                        gui_info.temp1_alarm_value -= 1;
                        write_slave_06(TEMP12_ALARM_ADDR,gui_info.temp2_alarm_value,gui_info.temp1_alarm_value);
                    }
                    temp_alarm_dis();

                    break;

                case TEMP2_ALARM:
                    if( gui_info.temp2_alarm_value > 20 )
                    {
                        gui_info.temp2_alarm_value -= 1;
                        write_slave_06(TEMP12_ALARM_ADDR,gui_info.temp2_alarm_value,gui_info.temp1_alarm_value);
                    }
                    temp_alarm_dis();

                    break;

                case TEMP3_ALARM:
                    if( gui_info.temp3_alarm_value > 20 )
                    {
                        gui_info.temp3_alarm_value -= 1;
                        write_slave_06(TEMP3_ALARM_ADDR,0X00,gui_info.temp3_alarm_value);
                    }
                    temp_alarm_dis();

                    break;

                default:
                    break;
            }
        }
        buzzer_close();
        key.key_scan_flag = 0;
    }
}

void KEY3_press()
{
    uint8_t key_value = KEY3;
    uint8_t key3_delay_cnt = 100;

    buzzer_open();
    delay_ms(10);

    /*               KEY1长按                 */
    if( gui_info.power_switch == true )
    {
        while((key_value == KEY3)&&(key3_delay_cnt!=0))        
        {
            key_value = (B1_VAL) | (B2_VAL<<1) | (B3_VAL<<2) | (B4_VAL<<3) | (B5_VAL<<4);
            key3_delay_cnt--;
            delay_ms(10); 
        }
        if( key3_delay_cnt == 0 )                   //长按1s后跳转报警温度调节界面
        {
            gui_info.power_switch = false;
            LCD_Clear(BLACK);        
            write_slave_06(POWER_SWITCH_ADDR,0x00,POWER_OFF);

            buzzer_close();
            delay_ms(500);
        }else
        {
            switch(gui_flicker.selection)
            {
                case LED_ICON:
                    led_dis();
                    gui_flicker.selection = FAN_ICON;

                    break;

                case FAN_ICON:
                    fan_dis();
                    gui_flicker.selection = BAKE_POWER;

                    break;

                case BAKE_POWER:
                    bake_dis();
                    gui_flicker.selection = BAKE_FAN;

                    break;

                case BAKE_FAN:
                    bake_dis();
                    jump_to_init();

                    break;

                case TEMP1_ALARM:
                    temp_alarm_dis();
                    gui_flicker.selection = TEMP2_ALARM;

                    break;

                case TEMP2_ALARM:
                    temp_alarm_dis();
                    gui_flicker.selection = TEMP3_ALARM;

                    break;

                case TEMP3_ALARM:
                    temp_alarm_dis();
                    jump_to_init();

                    break;

                default:
                    gui_flicker.selection++;
                    gui_flicker.enable_flag = true;

                    break;
            }
        }
    }else
    {
        gui_info.power_switch = true;
        write_slave_06(POWER_SWITCH_ADDR,0x00,POWER_ON);
        GUI_Icon_Init();
    }
    
    buzzer_close();
    key.key_scan_flag = 0;
}

void KEY4_press()
{
    if( gui_info.power_switch == true )
    {
        buzzer_open();
        delay_ms(10);
        if( gui_flicker.enable_flag == 0 )
        {
            if( gui_info.fan_level < 6 )
            {
                gui_info.fan_level += 1;
                write_slave_06(FAN_ADDR,0x00,gui_info.fan_level);
            }
            fan_dis();
        }else
        {
            switch(gui_flicker.selection)
            {
                case LED_ICON:
                    gui_info.led_switch = LED_ON;
                    write_slave_06(LED_ADDR,0X00,gui_info.led_switch);
                    led_dis();

                    break;

                case FAN_ICON:
                    if( gui_info.fan_level < 6)
                    {
                        gui_info.fan_level += 1;
                        write_slave_06(FAN_ADDR,0X00,gui_info.fan_level);
                    }
                    fan_dis();
                    break;

                case BAKE_POWER:
                    if( gui_info.bake_power_level < 100 )
                    {
                        gui_info.bake_power_level += 5;
                        write_slave_06(BAKE_ADDR,gui_info.bake_power_level,gui_info.bake_fan_level);
                    }
                    bake_dis();
                    break;

                case BAKE_FAN:
                    if( gui_info.bake_fan_level < 6 )
                    {
                        gui_info.bake_fan_level += 1;
                        write_slave_06(BAKE_ADDR,gui_info.bake_power_level,gui_info.bake_fan_level);
                    }
                    bake_dis();
                    break;

                case TEMP1_ALARM:
                    if( gui_info.temp1_alarm_value < 120 )
                    {
                        gui_info.temp1_alarm_value += 1;
                        write_slave_06(TEMP12_ALARM_ADDR,gui_info.temp2_alarm_value,gui_info.temp1_alarm_value);
                    }
                    temp_alarm_dis();

                    break;

                case TEMP2_ALARM:
                    if( gui_info.temp2_alarm_value < 120 )
                    {
                        gui_info.temp2_alarm_value += 1;
                        write_slave_06(TEMP12_ALARM_ADDR,gui_info.temp2_alarm_value,gui_info.temp1_alarm_value);
                    }
                    temp_alarm_dis();

                    break;

                case TEMP3_ALARM:
                    if( gui_info.temp3_alarm_value < 120 )
                    {
                        gui_info.temp3_alarm_value += 1;
                        write_slave_06(TEMP3_ALARM_ADDR,0X00,gui_info.temp3_alarm_value);
                    }
                    temp_alarm_dis();

                    break;

                default:
                    break;
            }
        }
        buzzer_close();
        key.key_scan_flag = 0;
    }
 }

void KEY5_press()
{
    uint8_t key_value = KEY5;
    uint8_t key5_delay_cnt = 100;

    if( gui_info.power_switch == true )
    {
        buzzer_open();
        delay_ms(10);

        while((key_value == KEY5)&&(key5_delay_cnt!=0)) 
        {
            key_value = (B1_VAL) | (B2_VAL<<1) | (B3_VAL<<2) | (B4_VAL<<3) | (B5_VAL<<4);
            key5_delay_cnt--;
            delay_ms(10); 
        }
        if( key5_delay_cnt == 0 )
        {
            gui_info.sync_switch1 = 1 - gui_info.sync_switch1;
            if( gui_info.sync_switch1 == 0 )
            {
                write_slave_06(SYNC_ADDR,0,gui_info.sync_switch1);
            }else
            {
                write_slave_06(SYNC_ADDR,gui_info.sync_switch2,gui_info.sync_switch1);
            }  
            sync_dis();
            
            buzzer_close();
            delay_ms(500);
        }else
        {
            GUI_Icon_Init();
            jump_to_init();
        }

        buzzer_close();
        key.key_scan_flag = 0;
    }
}

void buzzer_open( void )
{
    HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN_SET);
}

void buzzer_close( void )
{
    delay_ms(150);
    HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN_RESET);
}
