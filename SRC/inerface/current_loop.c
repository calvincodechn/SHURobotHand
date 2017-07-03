#include "current_loop.h"


static CurrentLoopPara current_loop_para[FINGER_SEL_TOTAL];

void curent_loop_main(void);





void current_loop_init(void)
{
  //init the  struct of current para.
  init_circular_list(current_loop_para, FINGER_SEL_TOTAL);
  // init the current function
  current_sys_callback(curent_loop_main);
	//filter_arrry_init();
}

void curent_loop_main(void)
{ //enter it per 166us
  //static CurrentLoopPara *loop_cur = current_loop_para;
  //SENSOR_INTERFACE *sensor_interface = get_sensor_interface_handle();

  //loop_cur->current_input;

  // _ADCC_start_sample();

  //loop_cur = loop_cur->next;
}



