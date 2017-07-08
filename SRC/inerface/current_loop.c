#include "current_loop.h"
#include "plat_algorithm.h"
#include "drv_timer.h"
#include "drv_adc.h"

static CurrentLoopPara current_loop_para[MOTOR_SERVO_TOTAL];
static FilterParaC c_filter_para;

void curent_loop_main(void);

void current_loop_init(void)
{
  MOTOR_SERVO_SELECT list_cur;
  PIDPara pid_para[MOTOR_SERVO_TOTAL] = {
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0}
};
  // init the  struct of current para.
  init_circular_list(current_loop_para, FINGER_SEL_TOTAL);
  // init the data of current para
  for(list_cur = 0; list_cur < MOTOR_SERVO_TOTAL; list_cur++)
  {
    current_loop_para[list_cur].motor_index = list_cur;
    current_loop_para[list_cur].direction = SERVO_DIR_STOP;
    current_loop_para[list_cur].pwm_duty = 0;
    current_loop_para[list_cur].control_pid = pid_para[list_cur];
  }
  // init the callback current function
  current_sys_callback(curent_loop_main);
  // filter_arrry_init();
  get_readbuff_current(&c_filter_para);
}

void curent_loop_main(void)
{
  static CurrentLoopPara *loop_cur = current_loop_para;

  _ADCC_start_sample();
  filter_buffer(c_filter_para.filter_buf, c_filter_para.filter_len, loop_cur->motor_index, loop_cur->current_value);
  
  loop_cur->control_pid.err = loop_cur->current_input - loop_cur->current_value;
  loop_cur->control_pid.intergral += loop_cur->control_pid.err;
  loop_cur->pwm_duty = loop_cur->control_pid.ctl_kp * loop_cur->control_pid.err + 
                       loop_cur->control_pid.ctl_ki * loop_cur->control_pid.intergral;
  loop_cur->control_pid.err_last = loop_cur->control_pid.err;

  set_motor_direction(loop_cur->motor_index, loop_cur->direction);
  timer_pwm_fresh(loop_cur->motor_index, loop_cur->pwm_duty);
 
  loop_cur = loop_cur->next;
}

void current_loop_set(MOTOR_SERVO_SELECT motor_sel, uint32_t current_input)
{
  // add assert here.
  current_loop_para[motor_sel].current_input = current_input;
}

