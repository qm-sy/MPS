#include "GUI.h"
#include "pic.h"

GUI_INFO gui_info;
GUI_FLICKER gui_flicker;

void GUI_Info_Init( void )
{
    gui_info.connect_flag           = UNCONNECT;
    gui_info.power_switch           = true;
    
    gui_info.fan_level              = LEVEL_3;
    gui_info.led_switch             = LED_ON; 
    gui_info.ac220_switch           = AC220V_OFF;
    gui_info.bake_fan_level         = LEVEL_3;
    gui_info.bake_power_level       = 50;
    gui_info.sync_switch1            = SYNC_ON;
    gui_info.temp1_alarm_value      = 50;
    gui_info.temp2_alarm_value      = 50;
    gui_info.temp3_alarm_value      = 50;
    gui_info.temp1_value            = 25;
    gui_info.temp2_value            = 25;
    gui_info.temp3_value            = 25;
    gui_info.envir_temp             = 25;
    gui_info.envir_humidity         = 65;
    gui_info.cc6903_24V            = 1.2f;
    gui_info.cc6903_36V            = 3.4f;
    gui_info.cc6903_42V            = 5.6f;

    gui_flicker.start_flag          = 0;
    gui_flicker.clear_flag          = 0;

    gui_flicker.selection           = 0;
    gui_flicker.enable_flag         = BEAT_OFF; 
}


void GUI_Icon_Init( void )
{
    /*              draw line                */
    LCD_DrawLine(10,36,310,36,GRAY);
    LCD_DrawLine(35,154,285,154,BROWN);
    LCD_DrawLine(10,210,310,210,GRAY);

    /*              connect icon                */
    connect_dis();

    /*              sync icon                */
    sync_dis();
    //signal_scan();
    
    /*              temp && shidu icon                */
    dht11_dis();
    PutChinese_12(13,245,"度",BACK_COLOR,POINT_COLOR);
    LCD_Show_Image_Internal_Flash(285,13,13,12,gImage_baifenbi,312);

    /*              temp icon                */
    LCD_ShowChar(56,50,'F',12,POINT_COLOR,BACK_COLOR);
    LCD_ShowChar(130,50,'M',12,POINT_COLOR,BACK_COLOR);
    LCD_ShowChar(204,50,'R',12,POINT_COLOR,BACK_COLOR);

    temp_alarm_dis();
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
    ac220_dis();
    /*              led  icon                */
    led_dis();

    /*              fan  icon                */
    LCD_Show_Image_Internal_Flash(94,166,32,32,gImage_fan,2048);

    fan_dis();

    /*              bake  icon                */
    bake_dis();

    LCD_Show_Image_Internal_Flash(180,172,36,21,gImage_bake,1512);
    PutChinese_strings(240,162,"功率",BACK_COLOR,POINT_COLOR);
    PutChinese_strings(240,190,"风速",BACK_COLOR,POINT_COLOR);

    /*             current 24 36 42                */
    cc6903_dis();

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

/**
 * @brief	界面信息-LED开关状态 跳动
 *
 * @param   void
 *
 * @return  void
 */
static void led_flicker( void )
{  
    if( gui_flicker.clear_flag == true )
    {
        LCD_Fill(32,166,64,198,BACK_COLOR);
    }else
    {
        led_dis();
    }
}

/**
 * @brief	界面信息-风扇档位 跳动
 *
 * @param   void
 *
 * @return  void
 */
static void fan_flicker( void )
{   
    if( gui_flicker.clear_flag == true )
    {
        LCD_Fill(135,165,151,197,BACK_COLOR);
    }else
    {
        fan_dis();
    }
}

static void bake_power_flicker( void )
{
    if( gui_flicker.clear_flag == true )
    {
        LCD_Fill(272,162,296,178,BACK_COLOR);
    }else
    {
        bake_dis();
    }
}

static void bake_fan_flicker( void )
{
    if( gui_flicker.clear_flag == true )
    {
        LCD_Fill(280,190,288,206,BACK_COLOR);
    }else
    {
        bake_dis();
    }
}

static void temp1_alarm_flicker( void )
{
    if( gui_flicker.clear_flag == true )
    {
        LCD_Fill(31,65,67,89,BACK_COLOR);
    }else
    {
        temp_alarm_dis();
    }
}
static void temp2_alarm_flicker( void )
{
    if( gui_flicker.clear_flag == true )
    {
        LCD_Fill(105,65,141,89,BACK_COLOR);
    }else
    {
        temp_alarm_dis();
    }
}

static void temp3_alarm_flicker( void )
{
    if( gui_flicker.clear_flag == true )
    {
        LCD_Fill(179,65,215,89,BACK_COLOR);
    }else
    {
        temp_alarm_dis();
    }
}


void connect_dis( void )
{
    if( gui_info.connect_flag == true )
    {
        LCD_Show_Image_Internal_Flash(15,3,29,30,gImage_connect_on,1740);
    }else
    {
        LCD_Show_Image_Internal_Flash(15,3,30,30,gImage_connect_off,1800);
    }
}

void sync_dis( void )
{
    if( gui_info.sync_switch1 == SYNC_ON )
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

void ac220_dis( void )
{
    if( gui_info.ac220_switch == 1)
    {
        LCD_Show_Image_Internal_Flash(242,100,66,39,gImage_ac220_on,5148);
    }else
    {
        LCD_Show_Image_Internal_Flash(242,100,66,39,gImage_ac220_off,5148);
    }
}

void temp_alarm_dis( void )
{
    LCD_ShowNum(31,65,gui_info.temp1_alarm_value,3,24,POINT_COLOR,BACK_COLOR);
    LCD_ShowNum(105,65,gui_info.temp2_alarm_value,3,24,POINT_COLOR,BACK_COLOR);
    LCD_ShowNum(179,65,gui_info.temp3_alarm_value,3,24,POINT_COLOR,BACK_COLOR);
}

void temp_dis( void )
{
    LCD_ShowNum(40,130,gui_info.temp1_value,3,16,POINT_COLOR,BACK_COLOR);
    LCD_ShowNum(114,130,gui_info.temp2_value,3,16,POINT_COLOR,BACK_COLOR);
    LCD_ShowNum(188,130,gui_info.temp3_value,3,16,POINT_COLOR,BACK_COLOR);
}

void led_dis( void )
{
    if( gui_info.led_switch == 1)
    {
        LCD_Show_Image_Internal_Flash(32,166,32,32,gImage_led_on,2048);
    }else
    {
        LCD_Show_Image_Internal_Flash(32,166,32,32,gImage_led_off,2048);
    }
}

void fan_dis( void )
{
    LCD_ShowNum(135,165,gui_info.fan_level,1,32,POINT_COLOR,BACK_COLOR);
}

void bake_dis( void )
{
    LCD_ShowNum(272,162,gui_info.bake_power_level,3,16,POINT_COLOR,BACK_COLOR);
    LCD_ShowNum(280,190,gui_info.bake_fan_level,1,16,POINT_COLOR,BACK_COLOR);
}

void cc6903_dis( void )
{   
    LCD_ShowxFloat(62,222,gui_info.cc6903_24V,1,12,BACK_COLOR,DARKBLUE);
    LCD_ShowxFloat(162,222,gui_info.cc6903_36V,1,12,BACK_COLOR,DARKBLUE);
    LCD_ShowxFloat(262,222,gui_info.cc6903_42V,1,12,BACK_COLOR,DARKBLUE);
}

void Icon_Flicker( void )
{
    if( gui_flicker.start_flag == 1 )
    {
        gui_flicker.clear_flag = !gui_flicker.clear_flag;

        switch (gui_flicker.selection)
        {
            case LED_ICON:      led_flicker();          break;

            case FAN_ICON:      fan_flicker();          break;

            case BAKE_POWER:    bake_power_flicker();   break;

            case BAKE_FAN:      bake_fan_flicker();     break;

            case TEMP1_ALARM:   temp1_alarm_flicker();  break;

            case TEMP2_ALARM:   temp2_alarm_flicker();  break;

            case TEMP3_ALARM:   temp3_alarm_flicker();  break;

            default:                                    break;
        }
    }

    gui_flicker.start_flag = 0; 
}

/**
 * @brief	界面信息-更新（04寄存器）
 *
 * @param   void
 *
 * @return  void
 */
void gui_info_update( void )
{
    if( gui_flicker.enable_flag == false )
    {
        if(( modbus.modbus_04_scan_flag == true ) && ( modbus.modbus_04_scan_allow == true ))
        {
            /*      1.读取从机04寄存器信息       */
            read_slave_04();
            
            /*      2.刷新屏幕       */
            gui_info_refresh();  
            modbus.modbus_04_scan_flag = 0;
        }
    }  
}

/**
 * @brief	读取从机04寄存器后刷新屏幕显示
 *
 * @param   void
 *
 * @return  void
 */
void gui_info_refresh( void )
{
    temp_dis();
    temp_alarm_dis();
    dht11_dis();
    cc6903_dis();    
}
