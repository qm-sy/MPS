#include "modbus_rtu.h"

MODBIS_INFO modbus;
SLAVE_04 slave_04;
SLAVE_06 slave_06;

/**
 * @brief	modbus_rtu  无奇偶校验
 *
 * @param   void
 *
 * @return  void 
**/
void Modbus_Event( void )
{
    uint16_t crc,rccrc;

    /*1.接收完毕                                           */
    if( rs485.RX2_rev_end_Flag == 1 )
    {
        /*2.清空接收完毕标志位                              */    
        rs485.RX2_rev_end_Flag = 0;

        /*3.CRC校验                                         */
        crc = MODBUS_CRC16(rs485.RX2_buf, rs485.RX2_rev_cnt-2);
        rccrc = (rs485.RX2_buf[rs485.RX2_rev_cnt-2]<<8) | (rs485.RX2_buf[rs485.RX2_rev_cnt-1]);

        /*4.清空接收计数                                    */
        rs485.RX2_rev_cnt = 0; 

        /*5.CRC校验通过，进行地址域校验                      */
        if( crc == rccrc )
        {
            /*6.地址域校验通过，进入相应功能函数进行处理      */
            if( rs485.RX2_buf[0] == MY_ADDR )
            {
                switch ( rs485.RX2_buf[1] )
                {
                    case 0x03:          Modbus_Fun3();          break;

                    case 0x04:          Modbus_Fun4();          break;

                    case 0x06:          Modbus_Fun6();          break;  

                    case 0x10:          Modbus_Fun16();         break;

                    default:                                    break;
                }
            }
        }
    }
}

/**
 * @brief	读输出寄存器  03
 *
 * @param   void
 *
 * @return  void 
**/
void Modbus_Fun3( void )
{
    uint16_t i;

    modbus.send_value_addr  = 3;                 //DATA1 H 位置
    modbus.byte_cnt   = (rs485.RX2_buf[4]<<8 | rs485.RX2_buf[5]) *2;
    modbus.start_addr = rs485.RX2_buf[2]<<8 | rs485.RX2_buf[3];

    rs485.TX2_buf[0]  = MY_ADDR;                //Addr
    rs485.TX2_buf[1]  = 0x03;                   //Fun
    rs485.TX2_buf[2]  = modbus.byte_cnt;        //Byte Count

    for( i = modbus.start_addr; i < modbus.start_addr + modbus.byte_cnt/2; i++ )
    {
        /*    每次循环前初始化byte_info                       */
        modbus.byte_info_H = modbus.byte_info_L = 0X00;
        switch (i)
        {
            /*  40001   LED开关状态查询                   */
            case 0:
                modbus.byte_info_H = 0X00;
                modbus.byte_info_L = slave_06.led_switch;                              //LED开关状态
                
                break;

            /*  40002  两路PWM 开关状态及风速查询                       */
            case 1:

                modbus.byte_info_H  = 0X00;
                modbus.byte_info_L  = slave_06.fan_level;   //PWM7风速
                break;

            /*  40003  220V 开关状态及功率查询               */
            case 2:
                modbus.byte_info_H = 0X00;
                modbus.byte_info_L = slave_06.ac220_switch;

                break;

            /*  40004  烘干 220V功率和风扇档位 查询                       */
            case 3:
                modbus.byte_info_H = slave_06.bake_power_level;  //220V 功率      
                modbus.byte_info_L = slave_06.bake_fan_level;         

                break;

            /*  40005 NTC1 NTC2 alarm value查询                            */
            case 4:
                modbus.byte_info_H = slave_06.temp2_alarm_value;   
                modbus.byte_info_L = slave_06.temp1_alarm_value;    

                break;

            /*  40006 NTC3 alarm value查询                            */
            case 5:
                modbus.byte_info_H = 0x00;   
                modbus.byte_info_L = slave_06.temp3_alarm_value;    

                break;

            /*  40007  同步开关设置                         */
            case 6:
                modbus.byte_info_H = 0x00;   
                modbus.byte_info_L = slave_06.sync_switch1;    

                break;

            default:
                break;
        }
        rs485.TX2_buf[modbus.send_value_addr++] = modbus.byte_info_H;
        rs485.TX2_buf[modbus.send_value_addr++] = modbus.byte_info_L;
    }
    slave_to_master(0x03,3 + modbus.byte_cnt);
}


/**
 * @brief	读输入寄存器  04
 *
 * @param   void
 *
 * @return  void 
**/
void Modbus_Fun4( void )
{
    uint16_t i;

    modbus.send_value_addr  = 3;                //DATA1 H 位置
    modbus.byte_cnt   = (rs485.RX2_buf[4]<<8 | rs485.RX2_buf[5]) *2;
    modbus.start_addr = rs485.RX2_buf[2]<<8 | rs485.RX2_buf[3];

    rs485.TX2_buf[0]  = MY_ADDR;                //Addr
    rs485.TX2_buf[1]  = 0x04;                   //Fun
    rs485.TX2_buf[2]  = modbus.byte_cnt;        //Byte Count

    for( i = modbus.start_addr; i < modbus.start_addr + modbus.byte_cnt/2; i++ )
    {
        /*    每次循环前初始化byte_info                       */
        modbus.byte_info_H = modbus.byte_info_L = 0X00;
        switch (i)
        {   
            /*  30001 NTC1 NTC2温度查询                     */
            case 0:
                modbus.byte_info_L = slave_04.temp1_value;
                modbus.byte_info_H = slave_04.temp2_value;     

                break;

            /*  30002 NTC3 NTC4温度查询                     */    
            case 1:
                modbus.byte_info_L = slave_04.temp3_value;
                
                break;

            /*    30003 2路IR查询                         */
            case 2:    
                modbus.byte_info_H = slave_04.envir_humidity;
                modbus.byte_info_L = slave_04.envir_temp;

                break;

            /*    30004 I_OUT1电流查询              */
            case 3:    
                modbus.byte_info_H = slave_04.cc6903_24V >> 8;     
                modbus.byte_info_L = slave_04.cc6903_24V;   

                break;

            /*    30005 I_OUT3 电流查询                     */
            case 4:    
                modbus.byte_info_H = slave_04.cc6903_36V >> 8;     
                modbus.byte_info_L = slave_04.cc6903_36V;    

                break;

            /*    30006 I_OUT3 电流查询                     */
            case 5:    
                modbus.byte_info_H = slave_04.cc6903_42V >> 8;     
                modbus.byte_info_L = slave_04.cc6903_42V;    

                break;

            default:

                break;
        }
        rs485.TX2_buf[modbus.send_value_addr++] = modbus.byte_info_H;
        rs485.TX2_buf[modbus.send_value_addr++] = modbus.byte_info_L;
    }
    slave_to_master(0x04,3 + modbus.byte_cnt);
}

/**
 * @brief	写单个输出寄存器  06
 *
 * @param   void
 *
 * @return  void 
**/
void Modbus_Fun6( void )
{
    switch (rs485.RX2_buf[3])
    {
        /*  40001  24V LED开关状态设置                  */
        case 0:
            slave_06.led_switch = rs485.RX2_buf[5];

            led_ctrl(slave_06.led_switch);

            break;
        
        /*  40002  两路PWM 开关状态及风速设置           */
        case 1:
            slave_06.fan_level = rs485.RX2_buf[5];

            fan_ctrl(slave_06.fan_level);

            break;

        /*  40003  220V 开关设置                        */
        case 2:
            slave_06.ac220_switch = rs485.RX2_buf[5];
            ac_ctrl.enable         = rs485.RX2_buf[5];

            ac220_ctrl(slave_06.ac220_switch);

            break;

        /*  40004  烘干功率及风扇档位 设置              */
        case 3:
            slave_06.bake_power_level = rs485.RX2_buf[4];
            slave_06.bake_fan_level   = rs485.RX2_buf[5];
            
            bake_power_ctrl(slave_06.bake_power_level);
            bake_fan_ctrl(slave_06.bake_fan_level);

            break;

        /*  40005  NTC1 NTC2 alarm value 设置           */
        case 4:
            slave_06.temp2_alarm_value = rs485.RX2_buf[4];
            slave_06.temp1_alarm_value = rs485.RX2_buf[5];

            break;
        
        /*  40006  NTC3 alarm value 设置                */
        case 5:
            slave_06.temp3_alarm_value = rs485.RX2_buf[5];

            break;

        /*  40007  同步开关设置                         */
        case 6:
            slave_06.sync_switch2 = rs485.RX2_buf[4];
            slave_06.sync_switch1 = rs485.RX2_buf[5];

            break;

        /*  40008  总开关设置                           */
        case 7:
            slave_06.power_switch = rs485.RX2_buf[5];
            power_switch_ctrl(slave_06.power_switch);

            break;

        default:
            break;   
    }
    
    slave_to_master(0x06,8);

    eeprom_data_record();
}

/**
 * @brief	写多个输出寄存器  16
 *
 * @param   void
 *
 * @return  void 
**/
void Modbus_Fun16( void )
{
    uint16_t i;

    modbus.rcv_value_addr = 7;                  //DATA1 H位置
    modbus.byte_cnt   = rs485.RX2_buf[6];
    modbus.start_addr = rs485.RX2_buf[2]<<8 | rs485.RX2_buf[3];

    memcpy(rs485.TX2_buf,rs485.RX2_buf,6);

    for( i = modbus.start_addr; i < modbus.start_addr + modbus.byte_cnt/2; i++)
    {
        modbus.byte_info_H = rs485.RX2_buf[modbus.rcv_value_addr];
        modbus.byte_info_L = rs485.RX2_buf[modbus.rcv_value_addr + 1];
        switch (i)
        {
            /*  40001  24V LED开关状态设置                  */
            case 0:
                slave_06.led_switch = modbus.byte_info_L;

                led_ctrl(slave_06.led_switch);

                break;
            
            /*  40002  两路PWM 开关状态及风速设置           */
            case 1:
                slave_06.fan_level = modbus.byte_info_L;

                fan_ctrl(slave_06.fan_level);

                break;

            /*  40003  220V 开关设置                        */
            case 2:
                slave_06.ac220_switch = modbus.byte_info_L;

                ac220_ctrl(slave_06.ac220_switch);

                break;

            /*  40004  烘干功率及风扇档位 设置              */
            case 3:
                slave_06.bake_power_level = modbus.byte_info_H;
                slave_06.bake_fan_level   = modbus.byte_info_L;
                
                bake_power_ctrl(slave_06.bake_power_level);
                bake_fan_ctrl(slave_06.bake_fan_level);

                break;

            /*  40005  NTC1 NTC2 alarm value 设置           */
            case 4:
                slave_06.temp2_alarm_value = modbus.byte_info_H;
                slave_06.temp1_alarm_value = modbus.byte_info_L;

                break;
            
            /*  40006  NTC3 alarm value 设置                */
            case 5:
                slave_06.temp3_alarm_value = modbus.byte_info_L;

                break;

            /*  40007  同步开关设置                         */
            case 6:
                slave_06.sync_switch1 = modbus.byte_info_L;

                break;

            default:
                break;
        }
        modbus.rcv_value_addr += 2;         //从Value1_H →→ 从Value2_H
    }
    
    slave_to_master(0x10,8);  

    eeprom_data_record();                      //记录更改后的值
}

/**
 * @brief	从机回复主机
 *  
 * @param   code_num:功能码       
 * @param   length:数据长度        
 * 
  @return  crc16:crc校验的值 2byte
 */
void slave_to_master(uint8_t code_num,uint8_t length)
{
    uint16_t crc;

    switch (code_num)
    {
        case 0x03:
            crc = MODBUS_CRC16(rs485.TX2_buf,length);

            rs485.TX2_buf[length+1] = crc;                 //CRC H
            rs485.TX2_buf[length] = crc>>8;            //CRC L

            rs485.TX2_send_bytelength = length + 2;
            
            break;

        case 0x04:
            crc = MODBUS_CRC16(rs485.TX2_buf,length);

            rs485.TX2_buf[length+1] = crc;                 //CRC H
            rs485.TX2_buf[length] = crc>>8;            //CRC L

            rs485.TX2_send_bytelength = length + 2;
            
            break;    

        case 0x06:
            memcpy(rs485.TX2_buf,rs485.RX2_buf,8);

            rs485.TX2_send_bytelength = length;
            
            break;   

        case 0x10:
            memcpy(rs485.TX2_buf,rs485.RX2_buf,6);
        
            crc = MODBUS_CRC16(rs485.TX2_buf,6);

            rs485.TX2_buf[7] = crc;                 //CRC H
            rs485.TX2_buf[6] = crc>>8;              //CRC L
        
            rs485.TX2_send_bytelength = length;
            
            break;         

        default:
            break;
    }

    DR2 = 1;                                 //485可以发送
    delay_ms(2);
    S2CON |= S4TI;                              //开始发送
    delay_ms(1);
}

/**
 * @brief	crc校验函数
 * 
 * @param   buf：  Address(1 byte) +Funtion(1 byte) ）+Data(n byte)   
 * @param   length:数据长度           
 * 
  @return  crc16:crc校验的值 2byte
 */
uint16_t MODBUS_CRC16(uint8_t *buf, uint8_t length)
{
	uint8_t	i;
	uint16_t	crc16;

    /* 1, 预置16位CRC寄存器为0xffff（即全为1）                          */
	crc16 = 0xffff;	

	do
	{
        /* 2, 把8位数据与16位CRC寄存器的低位相异或，把结果放于CRC寄存器     */        
		crc16 ^= (uint16_t)*buf;		//
		for(i=0; i<8; i++)		
		{
            /* 3, 如果最低位为1，把CRC寄存器的内容右移一位(朝低位)，用0填补最高位 再异或0xA001    */
			if(crc16 & 1)
            {
                crc16 = (crc16 >> 1) ^ 0xA001;
            }
            /* 4, 如果最低位为0，把CRC寄存器的内容右移一位(朝低位)，用0填补最高位                */
            else
            {
                crc16 >>= 1;
            }		
		}
		buf++;
	}while(--length != 0);

	return	(crc16);
}
