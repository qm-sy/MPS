#include "eeprom_crl.h"

/**
 * @brief	eeprom状态判定，是否写入过
 *
 * @param   
 *
 * @return  void
**/
void eeprom_statu_judge( void )
{
    uint8_t eeprom_statu_flag;

    eeprom_statu_flag = ISP_Read(EEPROM_STATU_JUDGE);
    //printf("The value of eeprom_statu_flag is 0x%02x \r\n",(int)eeprom_statu_flag);
    if( eeprom_statu_flag == 0xFF)
    {
        slave_06.fan_level          = 0x03;          //011 pwm7默认开3档风速
        slave_06.led_switch         = 0x01;          //0000000 1  led默认开
        slave_06.ac220_switch       = 0x00;          //默认关闭220V输出
        slave_06.bake_power_level   = 0x32;          //220V_CH4默认开，50%功率
        slave_06.bake_fan_level     = 0x03;          //011 pwm8默认开3档风速  
        slave_06.temp1_alarm_value  = 0x50;          //NTC1 alarm value 默认80℃
        slave_06.temp2_alarm_value  = 0x50;          //NTC2 alarm value 默认80℃  
        slave_06.temp3_alarm_value  = 0x50;          //NTC3 alarm value 默认80℃ 
        slave_06.sync_switch1        = 0;
        eeprom_data_record(); 
    }
    eeprom_data_init();    
}

/**
 * @brief	eeprom 数据写入
 *
 * @param   
 *
 * @return  void
**/
void eeprom_data_record( void )
{
    ISP_Earse(0x0000);

    ISP_Write(LED_ADDR,slave_06.led_switch);
    ISP_Write(FAN_ADDR,slave_06.fan_level);
    ISP_Write(AC220_ADDR,slave_06.ac220_switch);
    ISP_Write(BAKE_POWER_ADDR,slave_06.bake_power_level);
    ISP_Write(BAKE_FAN_ADDR,slave_06.bake_fan_level);
    ISP_Write(TEMP_ALARM1,slave_06.temp1_alarm_value);
    ISP_Write(TEMP_ALARM2,slave_06.temp2_alarm_value);
    ISP_Write(TEMP_ALARM3,slave_06.temp3_alarm_value);
    ISP_Write(SYNC_ADDR,slave_06.sync_switch1);

    ISP_Write(EEPROM_STATU_JUDGE,0x58);
}

/**
 * @brief	eeprom 数据初始化
 *
 * @param   
 *
 * @return  void
**/
void eeprom_data_init( void )
{
    /*    LED开关状态初始化    */
    slave_06.led_switch = ISP_Read(LED_ADDR);
    led_ctrl(slave_06.led_switch);

    /*    PWM7、PWM8 风速及开关状态初始化    */
    slave_06.fan_level = ISP_Read(FAN_ADDR);
    fan_ctrl(slave_06.fan_level);

    /*    220V输出开关状态初始化    */
    slave_06.ac220_switch = ISP_Read(AC220_ADDR);
    ac220_ctrl(slave_06.ac220_switch);

    /*    220V输出开关状态初始化    */
    slave_06.bake_fan_level   = ISP_Read(BAKE_FAN_ADDR);
    slave_06.bake_power_level = ISP_Read(BAKE_POWER_ADDR);

    bake_power_ctrl(slave_06.bake_power_level);
    bake_fan_ctrl(slave_06.bake_fan_level);


    /*    三路NTC alarm value初始化    */
    slave_06.temp1_alarm_value = ISP_Read(TEMP_ALARM1);
    slave_06.temp2_alarm_value = ISP_Read(TEMP_ALARM2);
    slave_06.temp3_alarm_value = ISP_Read(TEMP_ALARM3);

    slave_06.sync_switch1       = ISP_Read(SYNC_ADDR);

    slave_06.power_switch = 1;
    slave_06.sync_switch2 = 0;
}
