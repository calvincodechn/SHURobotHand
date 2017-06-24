/**/

#ifndef _CURRENT_LOOP_H
#define _CURRENT_LOOP_H

#include"drv_timer.h"

#define CURRENT_FILTER_LEN 5
#define CURRENT_VEC_LEN 4


typedef struct current_filter
{
  uint32_t current_vector[CURRENT_FILTER_LEN];
	uint32_t current_value;
	uint8_t current_cur;
}CurrentFilter, *pCurrentFilter;

typedef struct pid_para
{
	int32_t ctl_kp;
	int32_t ctl_ki;
	int32_t ctl_kd;
}PIDPara, *pPIDPara;

typedef struct motor_para{
  uint32_t pwm_duty;
  uint32_t current_input;
  uint32_t current_alarm_time; //the time that current keeps large.
  CurrentFilter current_filter;  //remember to init
  PIDPara current_pid;
  struct motor_para *next;
}CurrentLoopPara, *pCurrentLoopPara;


extern void current_loop_init(void);

#endif
