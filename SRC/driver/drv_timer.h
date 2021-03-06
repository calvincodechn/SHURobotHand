#ifndef _MOTOR_CONTROL_H
#define _MOTOR_CONTROL_H

#include "stm32f4xx.h"
#include <stdint.h>
#include "hand_define.h"

#define TIM2_PRESCALER         13   //14-1
#define TIM2_PERIOD            999  //1000-1 (psc)  Tclk = 84Mhz  // Tout = ((arr+1)*(psc+1))/Tclk
#define PWM_PRESCALER          9    //10-1
#define PWM_PERIOD             559  //560-1

typedef void (*current_callback)(void);
typedef void (*pressure_callback)(void);
typedef void (*angle_callback)(void);


void timer_Int_Init(void);
void timer_pwm_init(uint16_t timer_prescaler, uint32_t timer_period);
void gpio_motor_dir_init(void); 
void TIM2_IRQHandler(void);
void TIM5_IRQHandler(void);


extern void set_motor_direction(MOTOR_SERVO_SELECT f_select, SERVO_DIR_SEL servo_dir);
extern void timer_pwm_fresh(FINGEL_SELECT f_select, uint16_t pwmval);
extern void Set_motor_direction(unsigned char Fingersel, SERVO_DIR_SEL servo_dir);
extern void current_sys_callback(current_callback current_func_in);
extern void pressure_sys_callback(pressure_callback pressure_callback_in);
extern void angle_sys_callback(angle_callback angle_callback_in);
#endif


