/**/

#ifndef _CURRENT_LOOP_H
#define _CURRENT_LOOP_H

#include "drv_timer.h"
#include "hand_define.h"
#include "plat_algorithm.h"
#include "drv_adc.h"

#define CURRENT_FILTER_LEN 5
#define CURRENT_VEC_LEN 4




typedef struct pid_para
{
	float ctl_kp;
	float ctl_ki;
	float ctl_kd;
	int32_t err;
	int32_t err_last;
	int32_t intergral;
}PIDPara, *pPIDPara;

typedef struct motor_para{
  uint8_t motor_index;
  uint32_t pwm_duty;
  SERVO_DIR_SEL direction;
  uint32_t current_input;
  uint32_t current_value;
  uint32_t current_alarm_time;  // the time that current keeps large.
  PIDPara control_pid;
  struct motor_para *next;
}CurrentLoopPara, *pCurrentLoopPara;


extern void current_loop_init(void);

#endif
