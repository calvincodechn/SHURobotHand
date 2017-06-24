#include"current_loop.h"
#include"hand_define.h"

static CurrentLoopPara current_loop_para[FINGER_SEL_TOTAL];

void curent_loop_main(void);

void current_loop_init(void)
{
  int32_t motor_cur;
  //init the  struct of current para.
	for (motor_cur = 0; motor_cur < FINGER_SEL_TOTAL - 1; motor_cur++)
	{
		int filter_cur;
    current_loop_para[motor_cur].next = current_loop_para + motor_cur + 1;
  }
  current_loop_para[motor_cur].next = current_loop_para;
  
	//init the current function 
	current_loop_callback(curent_loop_main);

}

void curent_loop_main(void)
{
  static CurrentLoopPara *loop_cur = current_loop_para;
}
