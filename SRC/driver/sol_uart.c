
#include "sol_uart.h"
#include "stm32f4xx.h" 
#include "arm_math.h"
static void rev_pack_extract(void);

static send_pack_fetch 	_send_pack_fetch = NULL;

static send_pack_Fetch 	_send_pack_Fetch =NULL;

static check_pack_fetch _check_pack_fetch =NULL;
//static receive_pack_fetch  _receive_pack_fetch=NULL;



PackProtocol receive_pack;
PackInterface pack_interface;

uint8_t temp_data = 0;  

uint32_t erro_pack_sum = 0;




const uint16_t crc_temp[256] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};





static uint16_t protocol_uart_crc16( uint8_t *ptr, uint16_t len )
{
    uint16_t crc;
    uint8_t da;
    crc = 0;
    while ( len-- != 0 )
    {
        da = ( uint8_t )(crc >> 8);
        crc <<= 8;
        crc ^= crc_temp[da ^ *ptr];
        ptr++;
    }
    return ( crc );
}


static uint8_t crc_reiceive_pack_check(PackProtocol *receive_pack)
{
    uint16_t cal_headcrc = protocol_uart_crc16((uint8_t *)receive_pack, receive_pack->pack_len + 5);
		uint16_t rev_rc = ((uint16_t)receive_pack->pack_crc_h << 8) + receive_pack->pack_crc_l;
    if( rev_rc != cal_headcrc )
				return 0;
		else
				return 1;
}







static uint8_t _serial_hand_handle_t(uint8_t data)
{
    static uint8_t pos_rev = 0;
    static uint8_t buffer_index = 0;
    static uint8_t data_len = 0;
    temp_data = data;
    //drv_put_byte(System_Com, data );
    switch(pos_rev)
    {
        case 0:                                         //0xAA
          if(data == UART_MSG_PACKAGE_STARTCHAR) pos_rev ++;
          break;
        case 1:                                         //0x40
          if(data == MOTOR_DRIVER_DEVICE_TYPE)  pos_rev ++;
          else pos_rev = 0;
          break;
        case 2:                                         //0x01
          if(data == MOTOR_DIRVER_ADDRESS)
          {
              pos_rev ++;
              buffer_index = 0;
          }
          else pos_rev = 0;
          break;
        case 3:
          receive_pack.pack_cmd = data;
          pos_rev ++;
          break;
        case 4:
          receive_pack.pack_len = data;
          data_len =  data;
          pos_rev ++;
          break;
        case 5:
          if(0 == data_len) //caution: Jump to case 6 directly when the len==0 means that no data saved to buffer
            pos_rev ++;
          else
          {
            receive_pack.pack_data[buffer_index++] = data;
            data_len--;
            break;
          }
        case 6:
          receive_pack.pack_crc_l = data;
          pos_rev ++;
          break;
        case 7:
          receive_pack.pack_crc_h = data;
          pos_rev ++;
          break;
        case 8:
          if(data == UART_MSG_PACKAGE_ENDCHAR && crc_reiceive_pack_check(&receive_pack)) //&& crc_reiceive_pack_check(&receive_pack))
          {
            pos_rev = 0;
            return receive_pack.pack_cmd;               //return the right pack style;
          }
          else
          {
            pos_rev = 0;
						buffer_index = 0;
            erro_pack_sum++;            //bebug by cj;
            return Error_Pack;                                //erro package detected
          }
         // break;
        default:
          pos_rev = 0;
          break;
    }
    return pos_rev;                                     //return the error steps of pack where error ocoured;
    
}




static void _serial_hand_rx_t(uint8_t data)
{
		pack_interface.pack_state = _serial_hand_handle_t(data);
  
		switch(pack_interface.pack_state)
		{
		case UARTCMD_MOVE_Flag:			//receive velocity order
			pack_interface.pack_line_state[Receive_Pack] = LINE_READY;
			//rev_pack_extract();         //接收到slam的信息，然后把信息发送到底盘，控制底盘行走。发送到底盘的是两个电机的速度。
			//if(_receive_pack_fetch)
			//	_receive_pack_fetch();
			break;
		case UARTCMD_NOW_Flag:			//receive odom	request
			pack_interface.pack_line_state[Now_Pack] = LINE_READY;
		//	if(_send_pack_fetch)
		//			_send_pack_fetch();				//call the motion_ctrl.c`s app_robot_send_odmetry
	                            //接收到slam那块板子上的信息，然后发送里程计的数据到slam
			break;
		case UARTCMD_OLD_Flag:
			pack_interface.pack_line_state[Old_Pack] = LINE_READY;
			//if(_send_pack_Fetch)
			//		_send_pack_Fetch();	
			break;
		case UARTCMD_CHK_Flag:
			pack_interface.pack_line_state[Chk_Pack] = LINE_READY;
		//	if(_check_pack_fetch)
		//			_check_pack_fetch();
		case Error_Pack:
			break;
		default:
			return;                             //debug by cj
  //  break;
	
  }
  //printf("The sate now is %d\n", pack_interface.pack_state);
  //drv_put_byte(Follow_Com, pack_interface.pack_state );
}
/*
static void rev_pack_extract(void)
	{

		static unsigned char check_sum = 0;	
		int i=0;
	
		for(i=0;i<11;i++)
		{
				check_sum ^= pack_interface.pack_protocol->pack_data[i];//如果数据不对则不赋值
		
		}
		if(check_sum==pack_interface.pack_protocol->pack_data[11])
		{
				for(i=0;i<6;i++)
				{
						Motor_Angle_receive[i] =  pack_interface.pack_protocol->pack_data[i];
						//check_sum ^=  Motor_Angle_receive[i];
				}
				for(i=6;i<11;i++)
				{
						Pressure_On_receive[i-6] =  pack_interface.pack_protocol->pack_data[i];
						//check_sum ^=  Pressure_On_receive[i];
				}
	
				Receive_Flag=0x01;//接收标志位置1，指导手指运动
		}
		else
		{
				Receive_Flag=0x00;
		}
	
		
}
*/

/*
void NOW_send_callback(send_pack_fetch _send_pack_temp )
{
		_send_pack_fetch = _send_pack_temp;
}


void OLD_send_callback(send_pack_Fetch _send_pack_Temp)
{
		_send_pack_Fetch=_send_pack_Temp;
}

void CHK_send_callback(check_pack_fetch _check_pack_Temp)
{
		_check_pack_fetch=_check_pack_Temp;
}
*/

//void Parameter_receive_callback(receive_pack_fetch _receive_pack_Temp)
//{
	//_receive_pack_fetch=_receive_pack_Temp;
//}

void protocol_uart_send_msg( UART_CMD_Type Cmd, uint8_t *Dat, uint8_t Len )
{
    uint8_t   SendBuffer[100];
    uint8_t   i = 0, j;
    uint16_t  Crc16Value;

    SendBuffer[i++] = UART_MSG_PACKAGE_STARTCHAR;
    SendBuffer[i++] = MOTOR_DRIVER_DEVICE_TYPE;
    SendBuffer[i++] = MOTOR_DIRVER_ADDRESS;
    SendBuffer[i++] = Cmd;
    SendBuffer[i++] = Len;
    for( j = 0; j < Len; j++ )
    {
        SendBuffer[i++] = Dat[j];
    }
    Crc16Value = protocol_uart_crc16(SendBuffer, i);
    SendBuffer[i++] = ( uint8_t )Crc16Value;
    SendBuffer[i++] = ( uint8_t )(Crc16Value >> 8);
    SendBuffer[i++] = UART_MSG_PACKAGE_ENDCHAR;

    //drv_uart_put_multi_bytes_blocking(SendBuffer, i);
		
    drv_put_bytes(SendBuffer, i);
}


uint8_t init_hand_com(void)
{
    //drv_follow_init(115200);
    //drv_vehicle_init(115200);
    //serial_vehicle_backcall(_serial_vehicle_rx_t);
	  pack_interface.pack_protocol = &receive_pack;
	
    uart_hand_backcall(_serial_hand_rx_t);
	  return 1;
}  


PackInterface *get_pack_interface_handle(void)
{
	return &pack_interface;
}


