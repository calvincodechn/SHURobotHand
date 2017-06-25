#ifndef HAND_DEFINE_H
#define HAND_DEFINE_H

#include  "drv_uart.h"

#define NULL 0

typedef enum{
	FINGER_SEL_THUMB				= 0,
	FINGER_SEL_INDEX,
	FINGER_SEL_MIDDLE,
	FINGER_SEL_RING,
	FINGER_SEL_LITTLE,
	FINGER_SEL_ROOT,
	FINGER_SEL_TOTAL
}FINGEL_SELECT;

typedef enum{
	MOTOR_SERVO_THUMB				= 0,
	MOTOR_SERVO_INDEX,
	MOTOR_SERVO_MIDDLE,
	MOTOR_SERVO_RING,
	MOTOR_SERVO_LITTLE,
	MOTOR_SERVO_ROOT,
	MOTOR_SERVO_TOTAL
}MOTOR_SERVO_SELECT;


typedef enum{
	SERVO_DIR_UP				= 0,
	SERVO_DIR_DOWN,
	SERVO_DIR_STOP
}SERVO_DIR_SEL;

typedef enum{
	Hand_value_new      = 0,
	Hand_value_old1,				
	Hand_value_old2,
	Hand_value_old3,
	Hand_value_average,
	Hand_value_total
}hand_value;



typedef struct{
      uint8_t     Angle_rec_state;                       //一个包在读取过程中的状态
      uint16_t    Angle_Value_rec[FINGER_SEL_TOTAL];
			uint16_t    Pressure_Value_rec[FINGER_SEL_ROOT];
			uint16_t    Current_Value_rec[FINGER_SEL_TOTAL];
}HandValue_Rec;

typedef struct{
      //uint8_t     Angle_rec_state;                       //一个包在读取过程中的状态
      uint16_t    Angle_Value_start[FINGER_SEL_TOTAL];
			
}Angle_Value_Start;


typedef struct{
      
			int16_t    Hand_Angle_value[FINGER_SEL_TOTAL][Hand_value_total];
			int16_t    Hand_Pressure_value[FINGER_SEL_TOTAL][Hand_value_total];
			int16_t    Hand_Current_value[FINGER_SEL_TOTAL][Hand_value_total];
	
}Hand_Value;




#endif

