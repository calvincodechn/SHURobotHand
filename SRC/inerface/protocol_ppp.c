#include "inter_sensors.h"
#include "drv_timer.h"
#include "drv_adc.h"
#include "hand_define.h"

#include "protocol_ppp.h"

#include "drv_sysclk.h"
#include "transfer_protocol_uart.h"



static uint16_t  piece_together(uint8_t Parameter1,uint8_t 	Parameter2);
HandValue_Rec Hand_value_rec;
Hand_Value Hand_value;
Angle_Value_Start angle_value_start;
SENSOR_INTERFACE *Angle_sensor_value;
SENSOR_INTERFACE *Pressure_sensor_value;
SENSOR_INTERFACE *Current_sensor_value;
/*
static void Hand_Heart_Check(void)
{
		static unsigned long speed_record_time = 0;
		if(millis() - speed_record_time > 50)
		{

				PackData _send;
				int i=0;
				static unsigned char check_sum = 0;
				PackInterface *pack_interface = get_pack_interface_handle();
				speed_record_time = millis();		//
				if(pack_interface->pack_line_state[Heart_Getflag] == LINE_READY)
				{
					uint8_t len = pack_interface->pack_Protocol->pack_len;
					uint8_t *temp = (uint8_t *)&_send;
					for(i=0 ; i < len; i++)
					{
						 _send.data[i] = pack_interface->pack_Protocol->pack_data[i];

					}	
					pack_interface->pack_line_state[Heart_Getflag] = LINE_IDLE;

					protocol_uart_send_pc_msg( temp, len, pack_interface->pack_state);
					
				}
		}
}*/

static void Hand_Heart_Check(void)
{
				PackData _send;
				int i=0;
				static unsigned char check_sum = 0;
				Uarts_Data  *uarts_data= get_Uarts_data_handle();
				uint8_t len = 12;
					
				uint8_t *temp = (uint8_t *)&_send;
				for(i=0 ; i < len; i++)
				{
						_send.data[i] = uarts_data->Uarts_value[i+2];
				}	
				protocol_uart_send_pc_msg( temp, len, UARTCMD_HEART_BIT);		
}

static void Angle_sensor_Getvalue(FINGEL_SELECT Fingersel)
{
		uint8_t i_temp;
		Angle_sensor_value = get_sensor_interface_handle();
		
		Hand_value.Hand_Angle_value[Fingersel][Hand_value_new] = Angle_sensor_value->servo_angle_value[Fingersel];
		Hand_value.Hand_Angle_value[Fingersel][Hand_value_average] = (Hand_value.Hand_Angle_value[Fingersel][Hand_value_new] + Hand_value.Hand_Angle_value[Fingersel][Hand_value_old1]+Hand_value.Hand_Angle_value[Fingersel][Hand_value_old2]+Hand_value.Hand_Angle_value[Fingersel][Hand_value_old3])>>2;    
		Hand_value.Hand_Angle_value[Fingersel][Hand_value_old3] = Hand_value.Hand_Angle_value[Fingersel][Hand_value_old2];
		Hand_value.Hand_Angle_value[Fingersel][Hand_value_old2] = Hand_value.Hand_Angle_value[Fingersel][Hand_value_old1];
		Hand_value.Hand_Angle_value[Fingersel][Hand_value_old1] = Hand_value.Hand_Angle_value[Fingersel][Hand_value_new];
		if(Hand_value.Hand_Angle_value[Fingersel][Hand_value_average] > 0xFA0)
		{
				Hand_value.Hand_Angle_value[Fingersel][Hand_value_average] = 0xFA0;
		}
		Hand_value.Hand_Angle_value[Fingersel][Hand_value_average] = Hand_value.Hand_Angle_value[Fingersel][Hand_value_average] - angle_value_start.Angle_Value_start[Fingersel];
		if(Hand_value.Hand_Angle_value[Fingersel][Hand_value_average] < 0)
		{
				Hand_value.Hand_Angle_value[Fingersel][Hand_value_average] = Hand_value.Hand_Angle_value[Fingersel][Hand_value_average] + 0xFA0;
		}
}

static void Pressure_sensor_Getvalue(FINGEL_SELECT Fingersel)
{
		uint8_t i_temp;
		Pressure_sensor_value = get_sensor_interface_handle();
		Hand_value.Hand_Pressure_value[Fingersel][Hand_value_new] = Pressure_sensor_value->finger_pressure_value[Fingersel];
		
		Hand_value.Hand_Pressure_value[Fingersel][Hand_value_average] = (Hand_value.Hand_Pressure_value[Fingersel][Hand_value_new] + Hand_value.Hand_Pressure_value[Fingersel][Hand_value_old1]+Hand_value.Hand_Pressure_value[Fingersel][Hand_value_old2]+Hand_value.Hand_Pressure_value[Fingersel][Hand_value_old3] )>>2;    
		Hand_value.Hand_Pressure_value[Fingersel][Hand_value_old3] = Hand_value.Hand_Pressure_value[Fingersel][Hand_value_old2];
		Hand_value.Hand_Pressure_value[Fingersel][Hand_value_old2] = Hand_value.Hand_Pressure_value[Fingersel][Hand_value_old1];
		Hand_value.Hand_Pressure_value[Fingersel][Hand_value_old1] = Hand_value.Hand_Pressure_value[Fingersel][Hand_value_new];
	  
		
}

static void Current_sensor_Getvalue(FINGEL_SELECT Fingersel)
{
		uint8_t i_temp;
		Current_sensor_value = get_sensor_interface_handle();
		Hand_value.Hand_Current_value[Fingersel][Hand_value_new] = Current_sensor_value->servo_current_value[Fingersel];
		
		Hand_value.Hand_Current_value[Fingersel][Hand_value_average] = (Hand_value.Hand_Current_value[Fingersel][Hand_value_new] + Hand_value.Hand_Current_value[Fingersel][Hand_value_old1]+Hand_value.Hand_Current_value[Fingersel][Hand_value_old2]+Hand_value.Hand_Current_value[Fingersel][Hand_value_old3] )>>2;    
		Hand_value.Hand_Current_value[Fingersel][Hand_value_old3] = Hand_value.Hand_Current_value[Fingersel][Hand_value_old2];
		Hand_value.Hand_Current_value[Fingersel][Hand_value_old2] = Hand_value.Hand_Current_value[Fingersel][Hand_value_old1];
		Hand_value.Hand_Current_value[Fingersel][Hand_value_old1] = Hand_value.Hand_Current_value[Fingersel][Hand_value_new];
	  	
}

void get_hand_sensor_value(void)  //每隔50ms更新一次传感器数据
{
		FINGEL_SELECT Fingersel;
		static unsigned long speed_record_time = 0;
		if(millis() - speed_record_time > 50)
		{
				speed_record_time = millis();	
				for(Fingersel = FINGER_SEL_THUMB; Fingersel < FINGER_SEL_ROOT; Fingersel ++)
				{
						Angle_sensor_Getvalue(Fingersel);
						Pressure_sensor_Getvalue(Fingersel);
						Current_sensor_Getvalue(Fingersel);
				}
				Angle_sensor_Getvalue(FINGER_SEL_ROOT);
				Current_sensor_Getvalue(FINGER_SEL_ROOT);
		}
}




static void Pressure_Value_Get(void)
{
		
				PackData _send;
				int i=0;
				uint8_t f_select = 0;
				Uarts_Data  *uarts_data= get_Uarts_data_handle();
				uint8_t *temp = (uint8_t *)&_send;
					
				for(i=0 ; i < 10; i = i+2)
				{
						_send.data[i] = Hand_value.Hand_Pressure_value[f_select][Hand_value_average]>>8;  //将手指压力的平均值转化为2个8位数据存储在数组中，并发送到PC端
						_send.data[i+1] = (uint8_t)Hand_value.Hand_Pressure_value[f_select][Hand_value_average]; 
				  	f_select = f_select+1;
				}
				_send.data[i]   = 0;
				_send.data[i+1] = 0;
				protocol_uart_send_pc_msg( temp, 12, UARTCMD_Get_Pressure);  //调用函数发送数据
}
static void Current_Value_Get(void)
{
				PackData _send;
				int i=0;
				uint8_t f_select = 0;
				uint8_t *temp = (uint8_t *)&_send;
				for(i=0 ; i < 12; i = i+2)
				{
						_send.data[i] = Hand_value.Hand_Current_value[f_select][Hand_value_average]>>8;  //将电机电流的平均值转化为2个8位数据存储在数组中，并发送到PC端
						_send.data[i+1] = (uint8_t)Hand_value.Hand_Current_value[f_select][Hand_value_average]; 
						f_select = f_select+1;
				}
				protocol_uart_send_pc_msg( temp, 12, UARTCMD_Get_Current);
}

static void Angle_Value_Get(void)
{
				PackData _send;
				uint8_t f_select = 0;
				int i=0;							
				uint8_t *temp = (uint8_t *)&_send;					
				for(i=0 ; i < 12; i = i+2)
				{
					  _send.data[i] = Hand_value.Hand_Angle_value[f_select][Hand_value_average]>>8;  //将手指角度的平均值转化为2个8位数据存储在数组中，并发送到PC端
						_send.data[i+1] = (uint8_t)Hand_value.Hand_Angle_value[f_select][Hand_value_average]; 
						f_select = f_select+1;
				}
				protocol_uart_send_pc_msg( temp, 12, UARTCMD_Get_Angle);
}

static void Angle_Value_Set(void)
{
				PackData _send;
				int i=0;
				uint8_t f_select = 0;
				static unsigned char check_sum = 0;
				Uarts_Data  *uarts_data= get_Uarts_data_handle();				
				uint8_t len = 12;		
				for(i=0 ; i < len; i = i+2)
				{
					 Hand_value_rec.Angle_Value_rec[f_select] = piece_together(uarts_data->Uarts_value[i+2],uarts_data->Uarts_value[i+3]);//将接收到的2个8位数据转化为一个16位的角度值
					 f_select = f_select + 1;
				}	
}
static void Pressure_Value_Set(void)
{
				PackData _send;
				int i=0;
				uint8_t f_select = 0;
				static unsigned char check_sum = 0;
				uint8_t len = 10;
				Uarts_Data  *uarts_data= get_Uarts_data_handle();			
					
				for(i=0 ; i < len; i = i+2)
				{
					 Hand_value_rec.Pressure_Value_rec[f_select] = piece_together(uarts_data->Uarts_value[i+2],uarts_data->Uarts_value[i+3]); //将接收到的2个8位数据转化为一个16位的压力值
					 f_select = f_select + 1;
				}					
}

static void Current_Value_Set(void)
{
				PackData _send;
				int i=0;
				uint8_t f_select = 0;
				static unsigned char check_sum = 0;
				Uarts_Data  *uarts_data= get_Uarts_data_handle();	

				uint8_t len = 12;
				for(i=0 ; i < len; i = i+2)
				{
					 Hand_value_rec.Current_Value_rec[f_select] = piece_together(uarts_data->Uarts_value[i+2],uarts_data->Uarts_value[i+3]);  //将接收到的2个8位数据转化为一个16位的电流值
						
					 f_select = f_select + 1;
				}		
}


static uint16_t  piece_together(uint8_t Parameter1,uint8_t 	Parameter2)//将两个8位数据转化为一个16位的数据
{
		uint16_t Parameter3;
	  Parameter3 = Parameter1<<8;
		Parameter3 = Parameter3 +Parameter2;
    return Parameter3;

}
	

void Hand_callback_init(void)
{
		UART_Heart_callback(Hand_Heart_Check);
		UART_Get_pressure_callback(Pressure_Value_Get);
		UART_Get_angle_callback(Angle_Value_Get);
		UART_Get_current_callback(Current_Value_Get);
		UART_Set_pressure_callback(Pressure_Value_Set);
		UART_Set_angle_callback(Angle_Value_Set);
		UART_Set_current_callback(Current_Value_Set);
}























