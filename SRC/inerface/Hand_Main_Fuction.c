#include "inter_sensors.h"
#include "drv_timer.h"
#include "drv_adc.h"
#include "hand_define.h"

#include "Hand_Main_Fuction.h"

#include "drv_sysclk.h"
#include "transfer_protocol_uart.h"



static uint16_t  piece_together(uint8_t Parameter1,uint8_t 	Parameter2);
HandValue_Rec Hand_value_rec;
Hand_Value Hand_value;
Angle_Value_Start angle_value_start;
SENSOR_INTERFACE *Angle_sensor_value;
SENSOR_INTERFACE *Pressure_sensor_value;
SENSOR_INTERFACE *Current_sensor_value;

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

static void get_hand_sensor_value(void)  //ÿ��50ms����һ�δ���������
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
		static unsigned long speed_record_time = 0;
		if(millis() - speed_record_time > 50)
		{

				PackData _send;
				int i=0;
				uint8_t f_select = 0;
				static unsigned char check_sum = 0;
				PackInterface *pack_interface = get_pack_interface_handle();
				speed_record_time = millis();		//
				if(pack_interface->pack_line_state[Rressure_Getflag] == LINE_READY)
				{
					//uint8_t len = pack_interface->pack_Protocol->pack_len;
					uint8_t *temp = (uint8_t *)&_send;
					
					for(i=0 ; i < 12; i = i+2)
					{
						  _send.data[i] = Hand_value.Hand_Pressure_value[f_select][Hand_value_average]>>8;  //����ָѹ����ƽ��ֵת��Ϊ2��8λ���ݴ洢�������У������͵�PC��
							_send.data[i+1] = (uint8_t)Hand_value.Hand_Pressure_value[f_select][Hand_value_average]; 
							f_select = f_select+1;
					}
					
					protocol_uart_send_pc_msg( temp, 10, pack_interface->pack_state);  //���ú�����������
					pack_interface->pack_line_state[Rressure_Getflag] = LINE_IDLE;
				}
		}
}
static void Current_Value_Get(void)
{
		static unsigned long speed_record_time = 0;
		if(millis() - speed_record_time > 50)
		{

				PackData _send;
				int i=0;
				uint8_t f_select = 0;
				static unsigned char check_sum = 0;
				PackInterface *pack_interface = get_pack_interface_handle();
				speed_record_time = millis();		//
				if(pack_interface->pack_line_state[Curent_Getflag] == LINE_READY)
				{
					//uint8_t len = pack_interface->pack_Protocol->pack_len;
					uint8_t *temp = (uint8_t *)&_send;
					for(i=0 ; i < 12; i = i+2)
					{
						  _send.data[i] = Hand_value.Hand_Current_value[f_select][Hand_value_average]>>8;  //�����������ƽ��ֵת��Ϊ2��8λ���ݴ洢�������У������͵�PC��
							_send.data[i+1] = (uint8_t)Hand_value.Hand_Current_value[f_select][Hand_value_average]; 
							f_select = f_select+1;
					}
					protocol_uart_send_pc_msg( temp, 12, pack_interface->pack_state);
					pack_interface->pack_line_state[Curent_Getflag] = LINE_IDLE;
				}
		}
}

static void Angle_Value_Get(void)
{
		static unsigned long speed_record_time = 0;
		if(millis() - speed_record_time > 50)
		{

				PackData _send;
				uint8_t f_select = 0;
				int i=0;
				
				PackInterface *pack_interface = get_pack_interface_handle();
				speed_record_time = millis();		//
				if(pack_interface->pack_line_state[Angle_Getflag] == LINE_READY)
				{
					//uint8_t len = pack_interface->pack_Protocol->pack_len;
					uint8_t *temp = (uint8_t *)&_send;
					
					for(i=0 ; i < 12; i = i+2)
					{
						  _send.data[i] = Hand_value.Hand_Angle_value[f_select][Hand_value_average]>>8;  //����ָ�Ƕȵ�ƽ��ֵת��Ϊ2��8λ���ݴ洢�������У������͵�PC��
							_send.data[i+1] = (uint8_t)Hand_value.Hand_Angle_value[f_select][Hand_value_average]; 
						  //_send.data[i] = Hand_value_rec.Angle_Value_rec[f_select] >>8;
							//_send.data[i+1] = (uint8_t)Hand_value_rec.Angle_Value_rec[f_select]; 
							f_select = f_select+1;
					}
					
					
					protocol_uart_send_pc_msg( temp, 12, pack_interface->pack_state);
					pack_interface->pack_line_state[Angle_Getflag] = LINE_IDLE;
				}
		}
}

static void Angle_Value_Set(void)
{
		static unsigned long speed_record_time = 0;
		if(millis() - speed_record_time > 50)
		{

				PackData _send;
				int i=0;
				uint8_t f_select = 0;
				static unsigned char check_sum = 0;
				PackInterface *pack_interface = get_pack_interface_handle();
				speed_record_time = millis();		//
				if(pack_interface->pack_line_state[Angle_Pack] == LINE_READY)
				{
					uint8_t len = pack_interface->pack_Protocol->pack_len;
					//uint8_t *temp = (uint8_t *)&_send;
					
					for(i=0 ; i < len; i = i+2)
					{
						 Hand_value_rec.Angle_Value_rec[f_select] = piece_together(pack_interface->pack_Protocol->pack_data[i],pack_interface->pack_Protocol->pack_data[i+1]);//�����յ���2��8λ����ת��Ϊһ��16λ�ĽǶ�ֵ
						 f_select = f_select + 1;
					}
					
					//protocol_uart_send_pc_msg( temp, 6, pack_interface->pack_state);
					pack_interface->pack_line_state[Angle_Pack] = LINE_IDLE;
				}
		}
}
static void Pressure_Value_Set(void)
{
		static unsigned long speed_record_time = 0;
		if(millis() - speed_record_time > 50)
		{

				PackData _send;
				int i=0;
				uint8_t f_select = 0;
				static unsigned char check_sum = 0;
				PackInterface *pack_interface = get_pack_interface_handle();
				speed_record_time = millis();		//
				if(pack_interface->pack_line_state[Rressure_Pack] == LINE_READY)
				{
					uint8_t len = pack_interface->pack_Protocol->pack_len;
					//uint8_t *temp = (uint8_t *)&_send;
					
					for(i=0 ; i < len; i = i+2)
					{
						 Hand_value_rec.Pressure_Value_rec[f_select] = piece_together(pack_interface->pack_Protocol->pack_data[i],pack_interface->pack_Protocol->pack_data[i+1]); //�����յ���2��8λ����ת��Ϊһ��16λ��ѹ��ֵ
						 //Hand_angle_rec.Angle_Value_rec[f_select] = piece_together(pack_interface->pack_Protocol->pack_data[i],pack_interface->pack_Protocol->pack_data[i+1]);
						 f_select = f_select + 1;
					}
					
					//protocol_uart_send_pc_msg( temp, 6, pack_interface->pack_state);
					pack_interface->pack_line_state[Rressure_Pack] = LINE_IDLE;
				}
		}
}

static void Current_Value_Set(void)
{
		static unsigned long speed_record_time = 0;
		if(millis() - speed_record_time > 50)
		{

				PackData _send;
				int i=0;
				uint8_t f_select = 0;
				static unsigned char check_sum = 0;
				PackInterface *pack_interface = get_pack_interface_handle();
				speed_record_time = millis();		//
				if(pack_interface->pack_line_state[Curent_Pack] == LINE_READY)
				{
					uint8_t len = pack_interface->pack_Protocol->pack_len;
					
					
					for(i=0 ; i < len; i = i+2)
					{
						 Hand_value_rec.Current_Value_rec[f_select] = piece_together(pack_interface->pack_Protocol->pack_data[i],pack_interface->pack_Protocol->pack_data[i+1]);  //�����յ���2��8λ����ת��Ϊһ��16λ�ĵ���ֵ
						
						 f_select = f_select + 1;
					}
					
					
					pack_interface->pack_line_state[Curent_Pack] = LINE_IDLE;
				}
		}
}


static uint16_t  piece_together(uint8_t Parameter1,uint8_t 	Parameter2)//������8λ����ת��Ϊһ��16λ������
{
		uint16_t Parameter3;
	  Parameter3 = Parameter1<<8;
		Parameter3 = Parameter3 +Parameter2;
    return Parameter3;

}
	
	
	
void hand_message_loop(void)//ѭ����⵱ǰ�ı�־λ����ִ����Ӧ������
{

		Hand_Heart_Check();
		Angle_Value_Set();
		Pressure_Value_Set();
		Current_Value_Set();
		Current_Value_Get();
		Pressure_Value_Get();
		Angle_Value_Get();
		get_hand_sensor_value();  //��ȡ�Ƕȣ�ѹ��������ֵ
}
























