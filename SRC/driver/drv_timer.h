#ifndef _MOTOR_CONTROL_H
#define _MOTOR_CONTROL_H

#include "stm32f4xx.h"
#include <stdint.h>
#include "hand_define.h"

void timer_Int_Init(void);
void timer_pwm_init(uint16_t timer_prescaler, uint32_t timer_period);
void gpio_motor_dir_init(void); 
void TIM2_IRQHandler(void);
void TIM5_IRQHandler(void);


extern void set_motor_direction(MOTOR_SERVO_SELECT f_select, SERVO_DIR_SEL servo_dir);
extern void timer_pwm_fresh(FINGEL_SELECT f_select, uint16_t pwmval);
extern void Set_motor_direction(unsigned char Fingersel, SERVO_DIR_SEL servo_dir);

#endif


