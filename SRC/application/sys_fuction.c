#include "inter_sensors.h"
#include "drv_timer.h"
#include "drv_adc.h"
#include "stdlib.h"

#include "uart.h"
#include "sol_uart.h"
#include "sys_fuction.h"


SENSOR_INTERFACE* angle_sensor;
SENSOR_INTERFACE* current_sensor;
SENSOR_INTERFACE* pressure_sensor;
//系统1ms时钟处理函数
//#define Motor_Angle_Start 0;
#define Controlled_Min_Vlaue 0;


static void hand_send_now_parameter(void);
static void hand_send_old_parameter(void);
static void hand_send_chk_parameter(void);

unsigned char New_Reiceive_Flag = 0x00;
unsigned char sys_main_start = 0x00;
//static void app_robot_send_now(void);
//static void app_robot_receive_parameter(void );
//static void app_robot_send_old(void);
//static void app_robot_send_chk(void);

signed int Pwm_duty[6] = {0};

unsigned char Receive_Flag = 0;

int ad_angle[7];
int ad_current[5];
int ad_pressure[5];

signed int AD_Angle_old3[6] = {0};   //角度
signed int AD_Angle_old2[6] = {0};
signed int AD_Angle_old1[6] = {0};
signed int AD_Angle_new[6] =  {0};
signed int AD_Angle_average[6]={0}; 

signed int PID_Angle_sum[6] = {0};
signed int PID_Angle_e[6]={0};


unsigned char New_Message_Flag = 1;
unsigned char Job_Finshed[5] = {0};
unsigned char Pressure_On_receive[5];
unsigned char Motor_Angle_receive[6];
unsigned char Current_Over_Flag[5] ;
unsigned char Demo_Flag = 1;
unsigned int Motor_Angle_Start[6]={0};
signed int Motor_Angle_Protect[5]={0};

unsigned int AD_Current_Old3[5] ={0};//电流矩阵
unsigned int AD_Current_Old2[5] ={0};
unsigned int AD_Current_Old1[5] = {0};
//unsigned int AD_Current_Old[5] = {0};
unsigned int AD_Current_New[5] = {0};
unsigned int AD_Current_Average[5] = {0};
unsigned int Motor_Current_Protect[5]= {200,200,200,200,200};
unsigned int Current_Count[5] = {0};



unsigned char AD_Matrix_Old3[5] = {0};    //压力矩阵
unsigned char AD_Matrix_Old2[5] = {0};
unsigned char AD_Matrix_Old1[5] = {0};
unsigned char AD_Matrix_New[5] = {0};
unsigned char AD_Matrix_Average[5] = {0};
unsigned int AD_Pressure_Average[5] = {0};
//unsigned int Pressure_On_Receive[5] = {100,0,0,0,0};


signed int PID_Pressure_e[5] = {0};
signed int PID_Pressure_d[5] = {0};
signed int PID_Pressure_sum[5] = {0};


unsigned char ucControl_Order = 0;
//unsigned char New_Reiceive_Flag = 0x01;

unsigned char Heart_Beat = 0;
unsigned char Time_Count= 0;

/*unsigned char ppucPostion[][11] =
	{
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00 },		//0x01
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00 },		//0x01
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00 },	//0x01
		 
		{ 0x01, 0x01, 0x01, 0x01, 0x01,  0x01,   0x00, 0x00, 0x00, 0x00, 0x00 },		//0x02????  
		{ 0x77, 0x01, 0xBB, 0xBB, 0xBB,  0x44,   0x00, 0x00, 0x00, 0x00, 0x00 },		//0x03????  1
		{ 0x77, 0x01, 0x01, 0xBB, 0xBB,  0x55,   0x00, 0x00, 0x00, 0x00, 0x00 },		//0x04????  2 = V
		{ 0x99, 0xBB, 0x01, 0x01, 0x01,  0x55,   0x00, 0x00, 0x00, 0x00, 0x00 },		//0x05????  3 = ok
		{ 0x77, 0x01, 0x01, 0x01, 0x01,  0x01,   0x00, 0x00, 0x00, 0x00, 0x00 },		//0x06????	4
		{ 0x01, 0x01, 0x01, 0x01, 0x01,  0x01,   0x00, 0x00, 0x00, 0x00, 0x00 },		//0x07????	5 = fist = 0
		{ 0x01, 0xBB, 0xBB, 0xBB, 0x01,  0x01,   0x00, 0x00, 0x00, 0x00, 0x00 },		//0x08????	6
		{ 0x99, 0xBB, 0xBB, 0x01, 0x01,  0x88,   0x00, 0x00, 0x00, 0x00, 0x00 },		//0x09????	7
		{ 0x01, 0x01, 0xBB, 0xBB, 0xBB,  0x01,   0x00, 0x00, 0x00, 0x00, 0x00 },		//0x0A????	8
		{ 0x77, 0xBB, 0xBB, 0xBB, 0xBB,  0x33,   0x00, 0x00, 0x00, 0x00, 0x00 },		//0x0B????	10=0
		{ 0x77, 0x01, 0x01, 0x01, 0x01,  0x01,   0x00, 0x00, 0x00, 0x00, 0x00 },		//0x0C????	Thumb
		{ 0x01, 0xBB, 0x01, 0x01, 0x01,  0x01,   0x00, 0x00, 0x00, 0x00, 0x00 },		//0x0C????	Finger
		{ 0x01, 0x01, 0xBB, 0x01, 0x01,  0x01,   0x00, 0x00, 0x00, 0x00, 0x00 },		//0x0C????	Midddle
		{ 0x01, 0x01, 0x01, 0xBB, 0x01,  0x01,   0x00, 0x00, 0x00, 0x00, 0x00 },		//0x0C????	Index
		{ 0x01, 0x01, 0x01, 0x01, 0xBB,  0x01,   0x00, 0x00, 0x00, 0x00, 0x00 },		//0x0C????	Little
	};

*/

void Hand_measurement_init(void)
{
		Motor_Angle_receive[0] = 0x00;
		Motor_Angle_receive[1] = 0x00;
		Motor_Angle_receive[2] = 0x00;
		Motor_Angle_receive[3] = 0x00;
		Motor_Angle_receive[4] = 0x00;
		Motor_Angle_receive[5] = 0x00;
		Pressure_On_receive[0] = 0x00;
		Pressure_On_receive[1] = 0x00;
		Pressure_On_receive[2] = 0x00;
		Pressure_On_receive[3] = 0x00;
		Pressure_On_receive[4] = 0x00;
		
	
		Motor_Angle_Start[0] = 0;
		Motor_Angle_Start[1] = 0;
		Motor_Angle_Start[2] = 0;
		Motor_Angle_Start[3] = 0;
		Motor_Angle_Start[4] = 0;
		Motor_Angle_Start[5] = 0;
	
//		NOW_send_callback(app_robot_send_now);
//		OLD_send_callback(app_robot_send_old);
//		CHK_send_callback(app_robot_send_chk);
	//	Parameter_receive_callback(app_robot_receive_parameter);
		
	
}

void Angle_Get(unsigned char Fingersel)
{
	//int ad_angle[6];
	u16 i_temp;
	
  angle_sensor=get_sensor_interface_handle();
	angle_sensor->start_angle_sample();
	
	for(i_temp =0;i_temp < 6; i_temp++)
		{
			ad_angle[i_temp] = angle_sensor->servo_angle_value[i_temp];
		}
	AD_Angle_new[Fingersel]=ad_angle[Fingersel];
	AD_Angle_average[Fingersel] = (AD_Angle_new[Fingersel] + AD_Angle_old1[Fingersel]+ AD_Angle_old2[Fingersel] + AD_Angle_old3[Fingersel] ) >> 2;
  AD_Angle_old3[Fingersel] = AD_Angle_old2[Fingersel];
  AD_Angle_old2[Fingersel] = AD_Angle_old1[Fingersel];
	AD_Angle_old1[Fingersel] = AD_Angle_new[Fingersel];
	if(AD_Angle_average[Fingersel] > 4000)
	{
		AD_Angle_average[Fingersel] = 4000;
	}
	AD_Angle_average[Fingersel] = AD_Angle_average[Fingersel] - Motor_Angle_Start[Fingersel];
	if(AD_Angle_average[Fingersel] < 0)
		AD_Angle_average[Fingersel] += 4000;
	//AD_Angle_Average = AD_Angle_Average - Motor_Angle_Start;
}

void Angle_Control(unsigned char Fingersel)
{
  static const  float ucKp_A_min[6] = { 3.5, 3, 3, 3, 3,3};
	static const  float ucKp_A_max[6] = { 5, 5, 5, 5, 5,5};
	static const float ucKi_A_min[6] = { 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};
	static const float ucKi_A_max[6] = { 1, 1, 1, 1, 1, 1};
  static float ucKp_A[6]={0};
	static float ucKi_A[6]={0};
  
	//u16 Motor_Angle_receive[6]={1024,0,0,0,0,0};
	if(New_Reiceive_Flag == 1)
	{
			ucKp_A[Fingersel]= ucKp_A_min[Fingersel] + (((Motor_Angle_receive[Fingersel]<<4) * (ucKp_A_max[Fingersel] - ucKp_A_min[Fingersel]))/256);
			ucKi_A[Fingersel]= ucKi_A_min[Fingersel] + (((Motor_Angle_receive[Fingersel]<<4) * (ucKi_A_max[Fingersel] - ucKi_A_min[Fingersel]))/255);
			New_Reiceive_Flag=0x00;
	}
	
	PID_Angle_e[Fingersel] = (Motor_Angle_receive[Fingersel]<<8);
	PID_Angle_e[Fingersel] = PID_Angle_e[Fingersel] + Controlled_Min_Vlaue;
	PID_Angle_e[Fingersel] = PID_Angle_e[Fingersel] - AD_Angle_average[Fingersel];

	Pwm_duty[Fingersel]= PID_Angle_e[Fingersel] * ucKp_A[Fingersel];
  if(abs(PID_Angle_e[Fingersel]) < 600 )
	{
	  PID_Angle_sum[Fingersel] += ( PID_Angle_e[Fingersel]  >> 6); 
	}
	if(PID_Angle_sum[Fingersel] > 2500)
		PID_Angle_sum[Fingersel] = 2500;
	else if(PID_Angle_sum[Fingersel] <  -2500)
		PID_Angle_sum[Fingersel] = -2500;
	Pwm_duty[Fingersel]=Pwm_duty[Fingersel] +(PID_Angle_sum[Fingersel] * ucKi_A[Fingersel]);
	
	if( Pwm_duty[Fingersel] > 600 )       //direction determined by Pwm_Duty
	{
		Set_motor_direction(Fingersel, SERVO_DIR_UP);
		if(Pwm_duty[Fingersel] > 2500)
			Pwm_duty[Fingersel] = 2500;
	  Timer_pwm_fresh(Fingersel,350);
		//Pwm_duty[Fingersel]=450-Pwm_duty[Fingersel]/5.5;//调整PWM波
    //Timer_pwm_fresh(RING,Pwm_duty[Fingersel]);//刷新PWM波的占空比
	}
	else if( Pwm_duty[Fingersel] < -480 )		 
	{
		Set_motor_direction(Fingersel, SERVO_DIR_DOWN);
		if(Pwm_duty[Fingersel] < -1500)
			Pwm_duty[Fingersel] = -1500;
	  Pwm_duty[Fingersel] = abs(Pwm_duty[Fingersel]);//对Pwm_Duty取绝对值
	  //Pwm_duty[Fingersel]=450-Pwm_duty[Fingersel]/5.5;//调整PWM波
    //Timer_pwm_fresh(Fingersel,Pwm_duty[Fingersel]);//刷新PWM波的占空比
		Timer_pwm_fresh(Fingersel,350);
  }
	else 
	{
		Set_motor_direction(Fingersel, SERVO_DIR_STOP);//设置电机运行的方向
	  Pwm_duty[Fingersel]=455-Pwm_duty[Fingersel]/5.5;//调整PWM波
    Timer_pwm_fresh(Fingersel,Pwm_duty[Fingersel]);
	}
  
}


void Current_Get(unsigned char Fingersel)
{
	u16 i_temp;
	current_sensor= get_sensor_interface_handle();
	current_sensor->start_current_sample();
	for(i_temp=0;i_temp<6;i_temp++)
	{
	   ad_current[i_temp]=current_sensor->servo_current_value[i_temp];
	}
	AD_Current_New[Fingersel]  = ad_current[Fingersel];
	AD_Current_Old3[Fingersel] = AD_Current_Old2[Fingersel];
  AD_Current_Old2[Fingersel] = AD_Current_Old1[Fingersel];
	AD_Current_Old1[Fingersel] = AD_Current_New[Fingersel];
  AD_Current_Average[Fingersel]=(AD_Current_New[Fingersel] + AD_Current_Old1[Fingersel]+ AD_Current_Old2[Fingersel] + AD_Current_Old3[Fingersel])>>2 ;

}
void Current_Protect(void)
{
	unsigned char Fingersel = 0;
	for(Fingersel = THUMB; Fingersel < LITTLE+1; Fingersel ++)
	{
		if(AD_Current_Average[Fingersel] > 200 )
		{
	   	Current_Count[Fingersel] ++;
			if(Current_Count[Fingersel] > 75 )	 //20ms * 75 = 1.5S
				{
					Current_Over_Flag[Fingersel] = 1;
					Current_Count[Fingersel] = 0;
				}
		}
		else
		{
			Current_Count[Fingersel] = 0;
		}
	}
	Heart_Beat ++;
	if(Heart_Beat > 50)
	{
		//USCIA0_SendChar(0x1F);
		Heart_Beat = 0;
		Time_Count ++;				// used by DEMO
	}
	return;

}

void Pressure_Get(unsigned char Fingersel)
{
	u16 i_temp;
	pressure_sensor = get_sensor_interface_handle();
  pressure_sensor->start_pressure_sample();
	for(i_temp=0;i_temp<5;i_temp++)
	{
	   ad_pressure[i_temp]=pressure_sensor->finger_pressure_value[i_temp];
		
	}
	AD_Matrix_New[Fingersel]  = ad_pressure[Fingersel]>>4;
	//AD_Matrix_Old1[Fingersel] = AD_Matrix_New[Fingersel];
	//AD_Matrix_Old2[Fingersel] = AD_Matrix_Old1[Fingersel];
	AD_Matrix_Old3[Fingersel] = AD_Matrix_Old2[Fingersel];
  AD_Matrix_Old2[Fingersel] = AD_Matrix_Old1[Fingersel];
	AD_Matrix_Old1[Fingersel] = AD_Matrix_New[Fingersel];
  AD_Pressure_Average[Fingersel]=(AD_Matrix_New[Fingersel]+AD_Matrix_Old1[Fingersel]+AD_Matrix_Old2[Fingersel]+AD_Matrix_Old3[Fingersel])>>2;

	//AD_Pressure_Average[Fingersel]=AD_Matrix_Average[Fingersel]>>2;


}

void Pressure_Control(unsigned char Fingersel)
{
		static signed char ucKp_P = 18;
	  static float ucKi_P = 0.5;
	
		if(Pressure_On_receive[Fingersel] > 210)
		   Pressure_On_receive[Fingersel] = 210;
	  else if(Pressure_On_receive[Fingersel] < 40)
		   Pressure_On_receive[Fingersel] = 40; 
		
    PID_Pressure_e[Fingersel] = Pressure_On_receive[Fingersel] - AD_Pressure_Average[Fingersel];
		
	  Pwm_duty[Fingersel] = ucKp_P * PID_Pressure_e[Fingersel];
    
		PID_Pressure_sum[Fingersel] += (PID_Pressure_e[Fingersel] / 2);
		
	  if(PID_Pressure_sum[Fingersel] > 2500)
		   PID_Pressure_sum[Fingersel] = 2500;
	  else if(PID_Pressure_sum[Fingersel] < -2500)
		   PID_Pressure_sum[Fingersel] = -2500;
		
    Pwm_duty[Fingersel] += ucKi_P * PID_Pressure_sum[Fingersel];
		
		if( Pwm_duty[Fingersel] > 800 )       //direction determined by Pwm_Duty
	   {
			  Set_motor_direction(MIDDLE, SERVO_DIR_UP);
			  if(Pwm_duty[Fingersel] > 2500)
					Pwm_duty[Fingersel] = 2500;
			  //Pwm_duty[Fingersel]=450-Pwm_duty[Fingersel]/5.5;//调整PWM波
        Timer_pwm_fresh(MIDDLE,300);//刷新PWM波的占空比
	   }
	   else if( Pwm_duty[Fingersel] <  -800 )
	   {
		    Set_motor_direction(MIDDLE, SERVO_DIR_DOWN);
		    if(Pwm_duty[Fingersel] < -1500)
			     Pwm_duty[Fingersel] = -1500;
		    Pwm_duty[Fingersel] = abs(Pwm_duty[Fingersel]);
        Pwm_duty[Fingersel]=450-Pwm_duty[Fingersel]/5.5;//调整PWM波
        Timer_pwm_fresh(MIDDLE,Pwm_duty[Fingersel]);//刷新PWM波的占空比
	   }
	   else
	   {
		    Set_motor_direction(MIDDLE, SERVO_DIR_STOP);
	   }
		  
	   //Pwm_duty[Fingersel] = abs(Pwm_duty[Fingersel]); 
}

void Hand_Parameter_collect(void)
{
		unsigned char Fingersel;
	//if(!WaitForReceive)               //当接收到正确的指令时，才开启角度获取
	//{
		for(Fingersel= THUMB; Fingersel < LITTLE +1; Fingersel ++)//循环采集角度，压力和电流值
		{
			Angle_Get(Fingersel);					//角度值
			Pressure_Get(Fingersel);      //压力值
			//Current_Get(Fingersel);       //电流值 
		}
		Angle_Get( LITTLE +1);         //舵机角度值
	
}

/*void Specical_Function(unsigned char Fingersel)
{
	static unsigned char reposition_count = 0;
	unsigned char fingers;
	if(ucControl_Order < 0x11 )
		{
			Demonstration((unsigned int )&ucControl_Order);
		}
	else if( ucControl_Order == 0x11 && Motor_Angle_receive[Fingersel] != 0 )		//
		{
			//Calculate_Mag_Flag[Fingersel] = 1;
			Pressure_On_Receive[Fingersel] = 158;		//??????
			Motor_Angle_receive[Fingersel] = 0;
			//Pressure_Control(Fingersel);
		}
	else if( ucControl_Order == 0x12 )			//用于纠正角度传感器
		{
			for(fingers = THUMB; fingers < LITTLE +1; fingers ++)
			{
				Motor_Angle_Start[fingers]  = 0;
				Pressure_On_Receive[fingers] = 0;
				Motor_Angle_receive[fingers] = 0;
			}
			reposition_count ++;
			if(reposition_count > 100)
			{
				for(fingers = THUMB; fingers < LITTLE +1; fingers ++)
				{
					Motor_Angle_Start[fingers]  = AD_Angle_average[fingers];
				}

				reposition_count = 0;
				ucControl_Order = 0;
			}
		}

}
*/
/* void Demonstration(unsigned int puiActionSel)
 {
	const unsigned char Position_num = sizeof(ppucPostion) / sizeof(*ppucPostion);
	static unsigned char  uchFinger = 0;
	static unsigned char  uchSetFinish = 0;
	if(ucControl_Order == 0x01 &&  Demo_Flag == 1)					//
	{
		//if((Time_Count % 3) == 0 &&  uchSetFinish == 0)							//evry action should wait for 3 seconds
		//{
			Set_Reiceive_Array(*(ppucPostion + uchFinger));
			uchSetFinish = 1;
			uchFinger ++;
			if(uchFinger >= Position_num)
			{
				Demo_Flag = 0;							//???????,???????????
				uchFinger = 0;
			}
		//}

		//else if((Time_Count % 3) != 0)
		//{
			//uchSetFinish = 0;
		//}

	}
	else if( puiActionSel != 0x01 && Demo_Flag == 1 )			//???????????
	{
		if(puiActionSel <= Position_num + 1)
		{
			Set_Reiceive_Array(*(ppucPostion + (puiActionSel) - 1 ));
			Demo_Flag = 0;
		}

	}
 
 }
 void Set_Reiceive_Array(unsigned char* pucArray )
 {
	unsigned char i_finger = 0;
	for(i_finger = THUMB; i_finger < LITTLE + 1; i_finger ++)
	{
		*( Motor_Angle_receive + i_finger) = *(pucArray + i_finger) << 4;
	}

	*( Motor_Angle_receive + i_finger) = *(pucArray + i_finger);

	for(i_finger = THUMB; i_finger < LITTLE + 1; i_finger ++)
	{
		*(Pressure_On_Receive + i_finger) = *(pucArray + i_finger + 6);
	}
 
 }



*/
/*
static void app_robot_send_now(void)
{
		PackData _send;
		int i=0;
		static unsigned char check_sum = 0;	
		uint8_t *temp = (uint8_t *)&_send;
		//_send.data[0] = (int32_t)speed_odometry.total_distance[0];
		//_send.data[1] = (int32_t)speed_odometry.total_distance[1];
		for(i=0;i<6;i++)
		{
				_send.data[i] = (uint8_t)(AD_Angle_average[i]>>4);
				check_sum ^= _send.data[i];
		}
	
		for(i=6;i<11;i++)
		{
				_send.data[i] = (uint8_t)(AD_Pressure_Average[i-6]);
				check_sum ^= _send.data[i];
		}
		_send.data[11] = check_sum;
	
		protocol_uart_send_msg(UARTCMD_NOW_Flag, temp, 12);
		//f_put(_send.data[2]);
}
*/
/*
static void app_robot_send_old(void)
{
		PackData _send;
		int i=0;
		static unsigned char check_sum = 0;	
		uint8_t *temp = (uint8_t *)&_send;
		for(i=0;i<6;i++)
		{
				_send.data[i] = (uint8_t)(Motor_Angle_Start[i]>>4);
				check_sum ^= _send.data[i];
		}
	
		for(i=6;i<11;i++)
		{
				_send.data[i] = 0x00;
				check_sum ^= _send.data[i];
		}
		_send.data[11] = check_sum;
		protocol_uart_send_msg(UARTCMD_OLD_Flag, temp, 12);
}
*/
/*
static void app_robot_send_chk(void)
{ 
		PackData _send;
		int i=0;
		static unsigned char check_sum = 0;	
		uint8_t *temp = (uint8_t *)&_send;
		for(i=0;i<6;i++)
		{
				_send.data[i] = (uint8_t)(Motor_Angle_receive[i]);
				check_sum ^= _send.data[i];
		}
	
		for(i=6;i<11;i++)
		{
				_send.data[i] = (uint8_t)(Pressure_On_receive[i-6]);
				check_sum ^= _send.data[i];
		}
		_send.data[11] = check_sum;
		protocol_uart_send_msg(UARTCMD_CHK_Flag, temp, 12);
}
*/
void Main_Struct(void)
{
		unsigned char Fingersel;
	
		if (sys_main_start != 0x01)     //set by TIMER per 0.1ms
				return ;
		else
		{
		    sys_main_start = 0x00;
				if(Receive_Flag == 1)
				{
						
						Receive_Flag = 0x00;
						New_Reiceive_Flag = 0x01;		//标志位，代表接收到新的命令
				}
				
				for(Fingersel= THUMB;Fingersel < LITTLE +1; Fingersel ++)
				{
					
						if(Pressure_On_receive[Fingersel] != 0 && Motor_Angle_receive[Fingersel] == 0 )
						{
								Pressure_Control(Fingersel);
						}
						else if(Motor_Angle_receive[Fingersel] != 0 && Pressure_On_receive[Fingersel] == 0)
						{
					//Motor_Angle_PID(fingers_choose);
								Angle_Control(Fingersel);
						}
						else
						{
								Set_motor_direction(Fingersel, SERVO_DIR_STOP);
						}
						
				
				}
						
		}
	
}


void hand_message_loop(void)
{
		hand_send_now_parameter();
		hand_send_old_parameter();
		hand_send_chk_parameter();

}


void hand_send_now_parameter(void)
{
		static unsigned long speed_record_time = 0;
		if(millis() - speed_record_time > 50)
		{
				
				PackData _send;
				int i=0;
				static unsigned char check_sum = 0;	
				PackInterface *pack_interface = get_pack_interface_handle();
				speed_record_time = millis();		//延时50 才会触发这个函数
				
				if(pack_interface->pack_line_state[Now_Pack] == LINE_READY)
				{
					uint8_t *temp = (uint8_t *)&_send;
					for(i=0;i<6;i++)
					{
							_send.data[i] = (uint8_t)(AD_Angle_average[i]>>4);
							check_sum ^= _send.data[i];
					}
	
					for(i=6;i<11;i++)
					{
							_send.data[i] = (uint8_t)(AD_Pressure_Average[i-6]);
							check_sum ^= _send.data[i];
					}
					_send.data[11] = check_sum;
				//	_send.data[0] = (int32_t)speed_odometry.total_distance[0];
				//	_send.data[1] = (int32_t)speed_odometry.total_distance[1];
					protocol_uart_send_msg(UARTCMD_NOW_Flag, temp, 12);
					pack_interface->pack_line_state[Now_Pack] = LINE_IDLE;
				}
		}
}


void hand_send_old_parameter(void)
{
		static unsigned long speed_record_time = 0;
		if(millis() - speed_record_time > 50)
		{
				
				PackData _send;
				int i=0;
				static unsigned char check_sum = 0;	
				PackInterface *pack_interface = get_pack_interface_handle();
				speed_record_time = millis();		//延时50 才会触发这个函数
				
				if(pack_interface->pack_line_state[Old_Pack] == LINE_READY)
				{
					uint8_t *temp = (uint8_t *)&_send;
					for(i=0;i<6;i++)
					{
							_send.data[i] = (uint8_t)(Motor_Angle_Start[i]>>4);//发送其实的角度值
							check_sum ^= _send.data[i];
					}
	
					for(i=6;i<11;i++)
					{
							_send.data[i] = 0x00;
							check_sum ^= _send.data[i];
					}
					_send.data[11] = check_sum;
				
					protocol_uart_send_msg(UARTCMD_OLD_Flag, temp, 12);//发送数据到PC
					pack_interface->pack_line_state[Old_Pack] = LINE_IDLE;
				}
		}
}
void hand_send_chk_parameter(void)
{
		static unsigned long speed_record_time = 0;
		if(millis() - speed_record_time > 50)
		{
				
				PackData _send;
				int i=0;
				static unsigned char check_sum = 0;	
				PackInterface *pack_interface = get_pack_interface_handle();
				speed_record_time = millis();		//延时50 才会触发这个函数
				
				if(pack_interface->pack_line_state[Chk_Pack] == LINE_READY)
				{
					uint8_t *temp = (uint8_t *)&_send;
					for(i=0;i<6;i++)
					{
							_send.data[i] = (uint8_t)(Motor_Angle_receive[i]);//将接收到的数据赋值到data数据中并发送到PC端检测发送的对不对
							check_sum ^= _send.data[i];
					}
	
					for(i=6;i<11;i++)
					{
							_send.data[i] = (uint8_t)(Pressure_On_receive[i-6]);
							check_sum ^= _send.data[i];
					}		
					_send.data[11] = check_sum;
					
					protocol_uart_send_msg(UARTCMD_CHK_Flag, temp, 12);//发送数据到PC
					pack_interface->pack_line_state[Chk_Pack] = LINE_IDLE;
				}
		}
}

void hand_receive_data_parameter(void)
{
		static unsigned long speed_record_time = 0;
		if(millis() - speed_record_time > 50)
		{
				
				PackData _send;
				int i=0;
				static unsigned char check_sum = 0;	
				PackInterface *pack_interface = get_pack_interface_handle();
				speed_record_time = millis();		//延时50 才会触发这个函数
				
				if(pack_interface->pack_line_state[Receive_Pack] == LINE_READY)
				{
						for(i=0;i<11;i++)
						{
								check_sum ^= pack_interface->pack_protocol->pack_data[i];//如果数据不对则不赋值
		
						}
						if(check_sum==pack_interface->pack_protocol->pack_data[11])
						{
								for(i=0;i<6;i++)
								{
										Motor_Angle_receive[i] =  pack_interface->pack_protocol->pack_data[i];
										//check_sum ^=  Motor_Angle_receive[i];
								}
								for(i=6;i<11;i++)
								{
										Pressure_On_receive[i-6] =  pack_interface->pack_protocol->pack_data[i];
										//check_sum ^=  Pressure_On_receive[i];
								}
	
								Receive_Flag=0x01;//接收标志位置1，指导手指运动
						}
						else
						{
						Receive_Flag=0x00;
						}
				
				//	protocol_uart_send_msg(UARTCMD_NOW_Flag, temp, 8);
						pack_interface->pack_line_state[Receive_Pack] = LINE_IDLE;
				}
		}
}















