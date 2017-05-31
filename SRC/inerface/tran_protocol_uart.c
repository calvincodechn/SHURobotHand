#include "transfer_protocol_uart.h"



static void rev_pack_extract(void);

PackProtocol receive_Pack;
PackInterface pack_interface;

static uint8_t _serial_hand_handle_t(uint8_t data)
{
		static uint8_t pos_rev = 0;
    static uint8_t buffer_index = 0;
    static uint8_t data_len = 0;
		static uint8_t check_sum = 0;		
		uint8_t return_status = 0;
    //drv_put_byte(System_Com, data );
    switch(pos_rev)
    {
        case 0:                                         //0xAA
          if(data == UART_MSG_PACKAGE_STARTCHAR) pos_rev ++;
          break;
        case 1:                                         //0x01
          if(data == HAND_DIRVER_ADR)	pos_rev ++;
          else goto pack_error;
          break;
        case 2:				//cmd
          receive_Pack.pack_cmd = data;
          pos_rev ++;
          break;
        case 3:
					if(data > 50)	
						goto pack_error;			//limit the len of data
          receive_Pack.pack_len = data;
          data_len =  data;
          pos_rev ++;
          break;
        case 4:
          if(0 == data_len) //caution: Jump to case 6 directly when the len==0 means that no data saved to buffer
            pos_rev ++;
          else
          {
            receive_Pack.pack_data[buffer_index++] = data;
            data_len--;
            break;
          }
        case 5:
         // receive_Pack.pack_cheak = data;
					if(data == check_sum)
					{
						pos_rev ++;
						//check_sum = 0;
						break;
					}
					else goto pack_error;
       
        case 6:
          if(data == UART_MSG_PACKAGE_ENDCHAR )//&& crc_reiceive_pack_check(&receive_Pack))
          {
            pos_rev = 0;
						buffer_index = 0;
						check_sum = 0;
						//USART_SendData(Uart1_BASE,0xFF);
            return receive_Pack.pack_cmd;              //return the right pack style;
          }
          else goto pack_error;
          //break;
        default:
          pos_rev = 0;
          break;
    }
	  check_sum ^= data;
		
	  return pos_rev;
	
pack_error:
		return_status = pos_rev + 0xD0;
		pos_rev = 0;
		check_sum = 0;
		buffer_index = 0;
				return return_status;
}



static void _serial_hand_rx_t(uint8_t data)
{
		pack_interface.pack_state = _serial_hand_handle_t(data);
  
		switch(pack_interface.pack_state)
		{
				case UARTCMD_HEART_BIT:			//判断是否能够连接，将发送下来的数据再返回发送回去
						if(pack_interface.pack_line_state[Heart_Getflag] != LINE_READY)
								pack_interface.pack_line_state[Heart_Getflag] = LINE_READY;
						break;
				case UARTCMD_Get_Pressure:			//获取当前手指的压力值
						pack_interface.pack_line_state[Rressure_Getflag] = LINE_READY;
						break;
				case UARTCMD_Get_Current:			//获取当前电机的电流值
						pack_interface.pack_line_state[Curent_Getflag] = LINE_READY;
						break;
				case UARTCMD_Get_Angle:        //获取当前手指的角度值
						pack_interface.pack_line_state[Angle_Getflag] = LINE_READY;
						break;
				case UARTCMD_Set_Pressure:      //设定手指的压力值
						pack_interface.pack_line_state[Rressure_Pack] = LINE_READY;
						break;
				case UARTCMD_Set_Current:        //设定电机的电流
						pack_interface.pack_line_state[Curent_Pack] = LINE_READY;
						break;
				case UARTCMD_Set_Angle:           //设定电机转动的角度
						pack_interface.pack_line_state[Angle_Pack] = LINE_READY;
						break;
				case UARTCMD_CHK_Flag:             //无实际意义
						pack_interface.pack_line_state[Chk_Pack] = LINE_READY;
						break;
				default:
						return;   
						break;
		}
 
}


void protocol_uart_send_pc_msg( uint8_t *Dat, uint8_t Len, UART_CMD_Type cmd)//发送相同的格式数据到PC端
{
    uint8_t   SendBuffer[100];
    uint8_t   i = 0, j;
    uint16_t  Crc16Value;
		static uint8_t check_sum = 0;	
    SendBuffer[i++] = UART_MSG_PACKAGE_STARTCHAR;
    SendBuffer[i++] = HAND_DIRVER_ADR;
    SendBuffer[i++] = cmd;
    SendBuffer[i++] = Len;
    for( j = 0; j < Len; j++ )
    {
        SendBuffer[i++] = Dat[j];
    }
  
		for(j=0;j<i;j++)
		{
				check_sum=check_sum^SendBuffer[j];
		
		}
		SendBuffer[i++] = check_sum;
    SendBuffer[i++] = UART_MSG_PACKAGE_ENDCHAR;
		
    //drv_uart_put_multi_bytes_blocking(SendBuffer, i);
		check_sum = 0;
    drv_put_bytes(UartSysterm, SendBuffer, i);
}








uint8_t init_hand_com(void)//初始化函数
{
   
	  pack_interface.pack_Protocol = &receive_Pack;
		pack_interface.pack_line_state[Heart_Getflag] = LINE_IDLE;
		pack_interface.pack_line_state[Rressure_Getflag] = LINE_IDLE;
		pack_interface.pack_line_state[Curent_Getflag] = LINE_IDLE;
		pack_interface.pack_line_state[Angle_Getflag] = LINE_IDLE;
		pack_interface.pack_line_state[Rressure_Pack] = LINE_IDLE;
		pack_interface.pack_line_state[Curent_Pack] = LINE_IDLE;
		pack_interface.pack_line_state[Angle_Pack] = LINE_IDLE;
	
    uart_hand_backcall(_serial_hand_rx_t);
	  return 1;
}  

PackInterface *get_pack_interface_handle(void)
{
	  return &pack_interface;
}


