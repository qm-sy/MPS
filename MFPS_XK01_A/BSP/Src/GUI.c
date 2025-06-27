#include "GUI.h"
#include "pic.h"

GUI_INFO gui_info;
GUI_BEAT gui_beat;

void gui_init( void )
{
    gui_info.fan_level              = LEVEL_3;
    gui_info.led_switch             = LED_ON; 
    gui_info.ac220_switch           = AC220V_OFF;
    gui_info.ac220_info             = 0;            //000
    gui_info.bake_fan_level         = LEVEL_3;
    gui_info.bake_power_level       = 50;
    gui_info.sync_switch            = SYNC_ON;
    gui_info.alarm_temp1_value      = 50;
    gui_info.alarm_temp2_value      = 50;
    gui_info.alarm_temp3_value      = 50;
    gui_info.temp1_value            = 25;
    gui_info.temp2_value            = 25;
    gui_info.temp3_value            = 25;
    gui_info.envir_temp             = 25;
    gui_info.envir_humidity         = 65;
    gui_info.current_24V            = 1.2f;
    gui_info.current_36V            = 3.4f;
    gui_info.current_42V            = 5.6f;

    gui_beat.beat_start_flag        = 0;
    gui_beat.beat_clear             = 0;

    gui_beat.beat_select            = 0;
    gui_beat.beat_switch            = BEAT_OFF; 
    
    gui_info.connect_on_flag        = UNCONNECT;
}

static void alarm_temp_beat( void )
{
    switch (gui_beat.beat_select)
    {
        case ALARM_1:
            if( gui_beat.beat_clear == 1 )
            {
                LCD_Fill(31,65,67,89,BACK_COLOR);
            }else
            {
                LCD_ShowNum(31,65,gui_info.alarm_temp1_value,3,24,POINT_COLOR,BACK_COLOR);
            }
            
            break;
        
        case ALARM_2:
            if( gui_beat.beat_clear == 1 )
            {
                LCD_Fill(105,65,141,89,BACK_COLOR);
            }else
            {
                LCD_ShowNum(105,65,gui_info.alarm_temp2_value,3,24,POINT_COLOR,BACK_COLOR);
            }
            
            break;

        case ALARM_3:
            if( gui_beat.beat_clear == 1 )
            {
                LCD_Fill(179,65,215,89,BACK_COLOR);
            }else
            {
                LCD_ShowNum(179,65,gui_info.alarm_temp3_value,3,24,POINT_COLOR,BACK_COLOR);
            }
            
            break;

        default:
            break;
    }
}

static void bake_beat( void )
{
    switch (gui_beat.beat_select)
    {
        case BAKE_POWER:
            if( gui_beat.beat_clear == 1 )
            {
                LCD_Fill(272,162,296,178,BACK_COLOR);
            }else
            {
                LCD_ShowNum(272,162,gui_info.bake_power_level,3,16,POINT_COLOR,BACK_COLOR);
            }
            
            break;
        
        case BAKE_FAN:
            if( gui_beat.beat_clear == 1 )
            {
                LCD_Fill(280,190,288,206,BACK_COLOR);
            }else
            {
                LCD_ShowNum(280,190,gui_info.bake_fan_level,1,16,POINT_COLOR,BACK_COLOR);
            }
            
            break;

        default:
            break;
    }
}

void connect_dis( void )
{
    if( gui_info.connect_on_flag == CONNECT )
    {
        LCD_Show_Image_Internal_Flash(15,3,29,30,gImage_connect_on,1740);
    }else
    {
        LCD_Show_Image_Internal_Flash(15,3,30,30,gImage_connect_off,1800);
    }
}

void sync_dis( void )
{
    if( gui_info.sync_switch == 1 )
    {
        LCD_Show_Image_Internal_Flash(45,3,30,30,gImage_sync_on,1800);
    }else
    {
        LCD_Show_Image_Internal_Flash(45,3,30,30,gImage_gImage_sync_off,1800);
    }
}

void dht11_dis( void )
{
    LCD_ShowNum(215,10,gui_info.envir_temp,3,16,POINT_COLOR,BACK_COLOR);
    LCD_ShowNum(257,10,gui_info.envir_humidity,3,16,POINT_COLOR,BACK_COLOR);
}


void ac220_switch_dis( void )
{
    if( gui_info.ac220_switch == 1)
    {
        LCD_Show_Image_Internal_Flash(242,100,66,39,gImage_ac220_on,5148);
    }else
    {
        LCD_Show_Image_Internal_Flash(242,100,66,39,gImage_ac220_off,5148);
    }
}
// void ac220_temp_dis( void )
// {

// }

void led_switch_dis( void )
{
    if( gui_info.led_switch == 1)
    {
        LCD_Show_Image_Internal_Flash(32,166,32,32,gImage_led_on,2048);
    }else
    {
        LCD_Show_Image_Internal_Flash(32,166,32,32,gImage_led_off,2048);
    }
}

void alarm_temp_dis( void )
{
    LCD_ShowNum(31,65,gui_info.alarm_temp1_value,3,24,POINT_COLOR,BACK_COLOR);
    LCD_ShowNum(105,65,gui_info.alarm_temp2_value,3,24,POINT_COLOR,BACK_COLOR);
    LCD_ShowNum(179,65,gui_info.alarm_temp3_value,3,24,POINT_COLOR,BACK_COLOR);
}

void temp_dis( void )
{
    LCD_ShowNum(40,130,gui_info.temp1_value,3,16,POINT_COLOR,BACK_COLOR);
    LCD_ShowNum(114,130,gui_info.temp2_value,3,16,POINT_COLOR,BACK_COLOR);
    LCD_ShowNum(188,130,gui_info.temp3_value,3,16,POINT_COLOR,BACK_COLOR);
}

void fan_dis( void )
{
    LCD_ShowNum(135,165,gui_info.fan_level,1,32,POINT_COLOR,BACK_COLOR);
}

void current_dis( void )
{   
    LCD_ShowxFloat(62,222,gui_info.current_24V,1,12,BACK_COLOR,DARKBLUE);
    LCD_ShowxFloat(162,222,gui_info.current_36V,1,12,BACK_COLOR,DARKBLUE);
    LCD_ShowxFloat(262,222,gui_info.current_42V,1,12,BACK_COLOR,DARKBLUE);
}

void bake_dis( void )
{
    LCD_ShowNum(272,162,gui_info.bake_power_level,3,16,POINT_COLOR,BACK_COLOR);
    LCD_ShowNum(280,190,gui_info.bake_fan_level,1,16,POINT_COLOR,BACK_COLOR);
}

void icon_beat(uint8_t pic_code , uint8_t beat_switch )
{
    if( gui_beat.beat_start_flag == 1 )
    {
        delay_ms(1);
        gui_beat.beat_clear = 1 -  gui_beat.beat_clear;
        switch (pic_code)
        {
            case ALARM_1:
                alarm_temp_beat();
                break;

            case ALARM_2:
                alarm_temp_beat();
                break;

            case ALARM_3:
                alarm_temp_beat();
                break;

            case BAKE_POWER:
                bake_beat();
                break;

            case BAKE_FAN:
                bake_beat();
                break;

            default:
                break;
        }
    }

    gui_beat.beat_start_flag = 0; 
}

void refresh_icon( void )
{
    alarm_temp_dis();
    bake_dis();
}

void slave_statu_update( void )
{
    if( gui_beat.beat_switch == 0 )
    {
        if(( modbus.modbus_04_scan_flag == 1 ) && ( modbus.modbus_04_scan_allow == 1 ))
        {
            get_slave_statu_04();
            temp_dis();
            dht11_dis();
            current_dis();
            delay_ms(1);

            modbus.modbus_04_scan_flag = 0;
        }
    }else
    {
        modbus.modbus_04_scan_allow = 0;
    }   
}

void gui_icon_init( void )
{
    /*              draw line                */
    LCD_DrawLine(10,36,310,36,GRAY);
    LCD_DrawLine(35,154,285,154,BROWN);
    LCD_DrawLine(10,210,310,210,GRAY);

    /*              connect icon                */
    connect_dis();

    /*              sync icon                */
    LCD_Show_Image_Internal_Flash(45,3,30,30,gImage_gImage_sync_off,1800);

    
    /*              temp && shidu icon                */
    dht11_dis();
    PutChinese_12(13,245,"度",BACK_COLOR,POINT_COLOR);
    LCD_Show_Image_Internal_Flash(285,13,13,12,gImage_baifenbi,312);

    /*              temp icon                */
    LCD_ShowChar(56,50,'F',12,POINT_COLOR,BACK_COLOR);
    LCD_ShowChar(130,50,'M',12,POINT_COLOR,BACK_COLOR);
    LCD_ShowChar(204,50,'R',12,POINT_COLOR,BACK_COLOR);

    alarm_temp_dis();
    LCD_Show_Image_Internal_Flash(69,67,21,21,gImage_sheshidu_big,882);
    LCD_Show_Image_Internal_Flash(143,67,21,21,gImage_sheshidu_big,882);
    LCD_Show_Image_Internal_Flash(217,67,21,21,gImage_sheshidu_big,882);

    temp_dis();
    LCD_Show_Image_Internal_Flash(69,127,21,21,gImage_sheshidu_big,882);
    LCD_Show_Image_Internal_Flash(143,127,21,21,gImage_sheshidu_big,882);
    LCD_Show_Image_Internal_Flash(217,127,21,21,gImage_sheshidu_big,882);

    LCD_Show_Image_Internal_Flash(46,92,30,30,gImage_temp_stop,1800);
    LCD_Show_Image_Internal_Flash(120,92,30,30,gImage_temp_stop,1800 );
    LCD_Show_Image_Internal_Flash(194,92,30,30,gImage_temp_stop,1800);
    LCD_Show_Image_Internal_Flash(250,56,32,32,gImage_temp_set,2048);

    /*              ac 220 icon                */
    ac220_switch_dis();
    /*              led  icon                */
    led_switch_dis();

    /*              fan  icon                */
    fan_dis();
    
    LCD_Show_Image_Internal_Flash(94,166,32,32,gImage_fan,2048);

    /*              bake  icon                */
    bake_dis();

    LCD_Show_Image_Internal_Flash(180,172,36,21,gImage_bake,1512);
    PutChinese_strings(240,162,"功率",BACK_COLOR,POINT_COLOR);
    PutChinese_strings(240,190,"风速",BACK_COLOR,POINT_COLOR);

    /*             current 24 36 42                */
    current_dis();

    LCD_ShowNum(20,220,24,2,16,POINT_COLOR,BACK_COLOR);
    LCD_ShowChar(40,220,'V',16,POINT_COLOR,BACK_COLOR);
    LCD_ShowChar(50,220,':',16,POINT_COLOR,BACK_COLOR);
    LCD_ShowChar(87,220,'A',16,POINT_COLOR,BACK_COLOR);

    LCD_ShowNum(120,220,36,2,16,POINT_COLOR,BACK_COLOR); 
    LCD_ShowChar(140,220,'V',16,POINT_COLOR,BACK_COLOR);
    LCD_ShowChar(150,220,':',16,POINT_COLOR,BACK_COLOR);
    LCD_ShowChar(187,220,'A',16,POINT_COLOR,BACK_COLOR);

    LCD_ShowNum(220,220,42,2,16,POINT_COLOR,BACK_COLOR);
    LCD_ShowChar(240,220,'V',16,POINT_COLOR,BACK_COLOR);
    LCD_ShowChar(250,220,':',16,POINT_COLOR,BACK_COLOR);
    LCD_ShowChar(287,220,'A',16,POINT_COLOR,BACK_COLOR);
}


