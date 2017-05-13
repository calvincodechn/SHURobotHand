

#ifndef _SYS_FUCTION_H
#define _SYS_FUCTION_H


#include "stm32f4xx.h"
#include <stdint.h>
#include "hand_define.h"
#include "drv_sysclk.h"
#include "sol_uart.h"

extern unsigned char WaitForReceive;

extern unsigned char sys_main_start ;

//extern void receive_parameter(PackData temp);

//extern unsigned char Motor_Angle_receive[6];
//extern unsigned char Pressure_On_receive[5];


extern void   hand_message_loop(void);
extern void 	Angle_Control(unsigned char Fingersel);
extern void 	Angle_Get(unsigned char Fingersel);
extern void 	Main_Struct(void);
extern void 	Pressure_Control(unsigned char Fingersel);
extern void 	Pressure_Get(unsigned char Fingersel);
extern void 	Current_Get(unsigned char Fingersel);
extern void 	Current_Protect(void);
extern void 	Hand_Parameter_collect(void);
//extern void 	Specical_Function(unsigned char Fingersel);
//extern void 	Demonstration(unsigned int puiActionSel);
//extern void 	Set_Reiceive_Array(unsigned char* pucArray );
extern void 	Hand_measurement_init(void);


#endif
