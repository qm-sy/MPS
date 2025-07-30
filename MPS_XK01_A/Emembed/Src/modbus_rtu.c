#include "modbus_rtu.h"

MODBUS_INFO modbus;

/**
 * @brief	发送1帧数据
 * 
 * @param   buf：待发送数组首地址           
 * @param   len：数组长度           
 * 
  @return  void
 */
void modbus_send_data( uint8_t *buf , uint8_t len )
{
    HAL_UART_Transmit(&huart1,(uint8_t*)buf,len,1000);
    
    while (__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC) != SET);
}

/**
 * @brief	modbus接收函数，接收并判断Function后转到相应Function函数进行处理
 * 
 * @param   buf：待发送数组首地址           
 * @param   len：数组长度           
 * 
  @return  void
 */
void Modbus_Event( void )
{
    uint16_t crc,rccrc;

    /*1.接收完毕                                           */
    if( rs485.reflag == 1 )
    { 
        rs485.reflag = 0;

        /*2.CRC校验                                         */
        crc = MODBUS_CRC16(rs485.rcvbuf, rs485.recount-2);
        rccrc = (rs485.rcvbuf[rs485.recount-2]<<8) | (rs485.rcvbuf[rs485.recount-1]);
        if ( crc == rccrc)
        {
            if( rs485.rcvbuf[0] == SLAVE_ADDR )
            {
                switch ( rs485.rcvbuf[1] )
                {         
                    case 0x03:		Modbus_Fun3();		break;
                    case 0x04:		Modbus_Fun4();      break;            

                    default:						    break;
                }
                    /* RX允许继续接收，开启超时接收计时               */
                    rs485.recount = 0;
            }
        }
    }
}

void Modbus_Fun3( void )
{
   uint8_t start_addr_03 = 3;              //Slave reply  DATA1_H address

   for( uint16_t i = 0; i < 7; i++)
   {
       switch (i)
       {
        case 0:
            gui_info.led_switch = rs485.rcvbuf[start_addr_03 + 1];
            
            break;

        case 1:
            gui_info.fan_level = rs485.rcvbuf[start_addr_03 + 1];   

            break;

        case 2:
            gui_info.ac220_switch = rs485.rcvbuf[start_addr_03 + 1];
            
            break; 

        case 3:
            gui_info.bake_power_level = rs485.rcvbuf[start_addr_03];
            gui_info.bake_fan_level   = rs485.rcvbuf[start_addr_03 + 1];

            break;

        case 4:
            gui_info.temp2_alarm_value = rs485.rcvbuf[start_addr_03];
            gui_info.temp1_alarm_value = rs485.rcvbuf[start_addr_03 + 1];

            break;   

        case 5:
            gui_info.temp3_alarm_value = rs485.rcvbuf[start_addr_03 + 1];

            break;   

        case 6:
            gui_info.sync_switch1 = rs485.rcvbuf[start_addr_03 + 1];

            break;  

        default:
            break;
       }
       start_addr_03 += 2;
   }

   gui_info.connect_flag = true;
}

void Modbus_Fun4( void )
{
    uint8_t start_addr_04 = 3;              //Slave reply  DATA1_H address
    float current_24V,current_36V,current_42V = 0;

    for( uint16_t i = 0; i < 6; i++)
    {
        switch (i)
        {
            case 0:
                gui_info.temp2_value = rs485.rcvbuf[start_addr_04]; 
                gui_info.temp1_value = rs485.rcvbuf[start_addr_04 + 1]; 

                break;

            case 1:
                gui_info.temp3_value = rs485.rcvbuf[start_addr_04 + 1]; 

                break;

            case 2:
                gui_info.envir_humidity = rs485.rcvbuf[start_addr_04]; 
                gui_info.envir_temp     = rs485.rcvbuf[start_addr_04 + 1];     

                break; 

            
            case 3:
                current_24V = (rs485.rcvbuf[start_addr_04]<<8) | rs485.rcvbuf[start_addr_04 + 1];  
                if(current_24V <= 1360)
                {
                    current_24V = 1360;
                }
                gui_info.cc6903_24V = ((current_24V - 1360) / 36.0);
                if( gui_info.cc6903_24V > 0 )
                {
                    gui_info.cc6903_24V = gui_info.cc6903_24V + 0.4;
                }
                //printf("The 24V current is %.1f \r\n",gui_info.cc6903_24V);

                break;

            case 4:
                current_36V = (rs485.rcvbuf[start_addr_04]<<8) | rs485.rcvbuf[start_addr_04 + 1];      
                if(current_36V <= 1360)
                {
                    current_36V = 1360;
                }
                gui_info.cc6903_36V = ((current_36V - 1360) / 26.0);
                //printf("The 36V current is %.1f \r\n",gui_info.cc6903_36V);

                break;

            case 5:
                current_42V = (rs485.rcvbuf[start_addr_04]<<8) | rs485.rcvbuf[start_addr_04 + 1];  
                if(current_42V <= 1360)
                {
                    current_42V = 1360;
                }    
                gui_info.cc6903_42V = ((current_42V - 1360) / 36.0);
                if( gui_info.cc6903_42V > 0 )
                {
                    gui_info.cc6903_42V = gui_info.cc6903_42V + 0.4;
                }
                //printf("The 42V current is %.1f \r\n",gui_info.cc6903_42V);

                break;

            default:
                break;
        }
        start_addr_04 += 2;
    }
}

/**
 * @brief	读输出寄存器-03
 *
 * @param   void
 *
 * @return  void
 */
void read_slave_03( void )
{
    uint8_t send_buf[8];
    uint16_t crc;
    delay_ms(10);

    TX1_485;                        //使能发送
    delay_ms(2);                    //等待电平稳定

    send_buf[0] = SLAVE_ADDR;       //Addr
    send_buf[1] = FUN_03;           //Fun

    /*   Value_H  && Value_L    */
    send_buf[2] = START_REG_H_03;
    send_buf[3] = START_REG_L_03;
    send_buf[4] = REG_NUM_H_03;
    send_buf[5] = REG_NUM_L_03;

    /*   crc    */
    crc = MODBUS_CRC16(send_buf,6);
    send_buf[6] = crc >> 8;
    send_buf[7] = crc;

    /*   发送，后使能接收    */
    memcpy(modbus.send_buf,send_buf,8);

    modbus_send_data(modbus.send_buf,8); 
    RX1_485;
}

/**
 * @brief	读输入寄存器-04
 *
 * @param   void
 *
 * @return  void
 */
void read_slave_04( void )
{
    uint8_t send_buf[8];
    uint16_t crc;

    delay_ms(10);

    TX1_485;                        //使能发送
    delay_ms(2);                    //等待电平稳定

    send_buf[0] = SLAVE_ADDR;       //Addr
    send_buf[1] = FUN_04;           //Fun

    /*   Value_H  && Value_L    */
    send_buf[2] = START_REG_H_04;
    send_buf[3] = START_REG_L_04;
    send_buf[4] = REG_NUM_H_04;
    send_buf[5] = REG_NUM_L_04;

    /*   crc    */
    crc = MODBUS_CRC16(send_buf,6);
    send_buf[6] = crc >> 8;
    send_buf[7] = crc;

    
    /*   发送，后使能接收    */
    memcpy(modbus.send_buf,send_buf,8);

    modbus_send_data(modbus.send_buf,8); 
    RX1_485;
}

/**
 * @brief	写单个输出寄存器-06
 *
 * @param   reg_addr：要写的寄存器地址
 *          reg_val： 要写的值
 *
 * @return  void
 */
void write_slave_06( uint16_t reg_addr, uint8_t reg_val_H, uint8_t reg_val_L)
{
    uint8_t send_buf[8];
    uint16_t crc;

    delay_ms(10);

    TX1_485;                        //使能发送
    delay_ms(2);                    //等待电平稳定

    send_buf[0] = SLAVE_ADDR;       //Addr
    send_buf[1] = FUN_06;           //Fun

    /*   Value_H  && Value_L    */
    send_buf[2] = reg_addr >> 8;
    send_buf[3] = reg_addr;
    send_buf[4] = reg_val_H ;
    send_buf[5] = reg_val_L;

    /*   crc    */
    crc = MODBUS_CRC16(send_buf,6);
    send_buf[6] = crc >> 8;
    send_buf[7] = crc;

    /*   发送，后使能接收    */
    memcpy(modbus.send_buf,send_buf,8);

    modbus_send_data(modbus.send_buf,8);
    RX1_485;        
}

/**
 * @brief	写多个输出寄存器-16
 *
 * @param   void
 *
 * @return  void
 */
void write_slave_16( void )
{
    uint8_t send_buf[23];
    uint16_t crc;

    delay_ms(10);

    TX1_485;
    delay_ms(2);

    send_buf[0] = SLAVE_ADDR;       //Addr
    send_buf[1] = FUN_16;           //Fun
    send_buf[2] = 0x00;             //Start reg H
    send_buf[3] = 0x00;             //Start reg L
    send_buf[4] = 0x00;             //Num H
    send_buf[5] = 0x07;             //Num L
    send_buf[6] = 0x0e;             //Byte count

    /*   Valuex_H  && Valuex_L    */
    send_buf[7] = 0x00;
    send_buf[8] = gui_info.led_switch;

    send_buf[9] = 0x00;
    send_buf[10] = gui_info.fan_level;

    send_buf[11] = 0x00;
    send_buf[12] = gui_info.ac220_switch;

    send_buf[13] = gui_info.bake_power_level;
    send_buf[14] = gui_info.bake_fan_level;

    send_buf[15] = gui_info.temp2_alarm_value;
    send_buf[16] = gui_info.temp1_alarm_value;

    send_buf[17] = 0x00;
    send_buf[18] = gui_info.temp3_alarm_value;

    send_buf[19] = 0x00;
    send_buf[20] = gui_info.sync_switch1;

    /*   crc    */
    crc = MODBUS_CRC16(send_buf,21);
    send_buf[21] = crc>>8;
    send_buf[22] = crc;

    /*   发送，后使能接收    */
    memcpy(modbus.send_buf,send_buf,23);

    modbus_send_data(modbus.send_buf,23);
    RX1_485;
}

/**
 * @brief	crc校验函数
 * 
 * @param   buf：  Address(1 byte) +Funtion(1 byte) ）+Data(n byte)   
 * @param   length:数据长度           
 * 
  @return  crc16:crc校验的值 2byte
 */
//buf内的值为 
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
