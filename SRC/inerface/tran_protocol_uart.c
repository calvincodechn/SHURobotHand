#include "transfer_protocol_uart.h"
#include "drv_sysclk.h"
#include "drv_timer.h" 

static void rev_pack_extract(void);

PackProtocol receive_Pack;
PackInterface pack_interface;
static uint8_t _serial_hand_Protocol_analysis(uint8_t *Uarts_value,char len);
static UART_callback UART_callback_Heart_bit = NULL;

static UART_callback UART_callback_Get_pressure = NULL;
static UART_callback UART_callback_Get_angle = NULL;
static UART_callback UART_callback_Get_current = NULL;

static UART_callback UART_callback_Set_pressure = NULL;
static UART_callback UART_callback_Set_angle = NULL;
static UART_callback UART_callback_Set_current = NULL;

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

PackInterface *get_pack_interface_handle(void)
{
	  return &pack_interface;
}


void _serial_hand_handle(void)
{
		uint8_t cmd;
		static unsigned long speed_record_time = 0;
		if(millis() - speed_record_time > 50)
		{
				Uarts_Data  *uarts_data= get_Uarts_data_handle();
				speed_record_time = millis();
				if(uarts_data->DMA_Transfer_finish == LINE_DMA_READY )
				{
						uarts_data->DMA_Transfer_finish = LINE_DMA_IDLE;
						cmd =_serial_hand_Protocol_analysis(uarts_data->Uarts_value,18);
						switch (cmd)
						{
								case UARTCMD_HEART_BIT:
									 UART_callback_Heart_bit();
									 break;
								case UARTCMD_Get_Pressure:			//获取当前手指的压力值
									 UART_callback_Get_pressure();
									 break;
								case UARTCMD_Get_Current:			//获取当前电机的电流值
						       UART_callback_Get_current();
									 break;
								case UARTCMD_Get_Angle:        //获取当前手指的角度值
						       UART_callback_Get_angle();
									 break;
								case UARTCMD_Set_Pressure:      //设定手指的压力值
						       UART_callback_Set_pressure();
									 break;
								case UARTCMD_Set_Current:        //设定电机的电流
					         UART_callback_Set_current();
					       	 break;
								case UARTCMD_Set_Angle:           //设定电机转动的角度
					         UART_callback_Set_angle();
					         break;
						  	case UARTCMD_CHK_Flag:             //无实际意义						
						       break;
								default:
									 break;
						
						}
				}
					
		}
}

void UART_Heart_callback(UART_callback UART_callback_Heart)
{
    UART_callback_Heart_bit = UART_callback_Heart;
}

void UART_Get_pressure_callback(UART_callback UART_callback_get_pressure)
{
    UART_callback_Get_pressure = UART_callback_get_pressure;
}

void UART_Get_angle_callback(UART_callback UART_callback_get_angle)
{
    UART_callback_Get_angle = UART_callback_get_angle;
}

void UART_Get_current_callback(UART_callback UART_callback_get_current)
{
    UART_callback_Get_current = UART_callback_get_current;
}

void UART_Set_pressure_callback(UART_callback UART_callback_set_pressure)
{
    UART_callback_Set_pressure = UART_callback_set_pressure;
}

void UART_Set_angle_callback(UART_callback UART_callback_set_angle)
{
    UART_callback_Set_angle = UART_callback_set_angle;
}

void UART_Set_current_callback(UART_callback UART_callback_set_current)
{
    UART_callback_Set_current = UART_callback_set_current;
}


static uint8_t _serial_hand_Protocol_analysis(uint8_t *Uarts_value,char len)
{
		static uint8_t cmd = 0;
		static uint8_t Package_Count = 0;
		static uint8_t check_sum = 0;
		while(1)
		{
				if( Package_Count == 0 && Uarts_value[Package_Count] == UART_MSG_PACKAGE_STARTCHAR)
						 Package_Count++;
				else if(Package_Count == 1)
				{
						 cmd = Uarts_value[Package_Count];
						 Package_Count++;
						
				}
				else if ( Package_Count > 1 && Package_Count < 14 )			 
				{
						
						 Package_Count ++;
	      }
				else if(Package_Count == 14 && Uarts_value[Package_Count] == check_sum)
				{
						 Package_Count ++;
				}
				else if(Package_Count == 15 && Uarts_value[Package_Count] == UART_MSG_PACKAGE_ENDCHAR)
				{
						 Package_Count =0;
						 check_sum = 0;
						 return cmd;				
				}
				else 
				{
						 Package_Count =0;
						 check_sum = 0;
						 return 0;
				}
				check_sum ^=  Uarts_value[Package_Count-1];
		}



}








