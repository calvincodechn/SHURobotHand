#include "drv_timer.h"
#include "plat_algorithm.h"
#include "drv_adc.h"

void pressure_loop_main(void);

void pressure_loop_init(void)
{
  
   pressure_sys_callback(pressure_loop_main);

}

void pressure_loop_main(void)
{ 
   //_ADCP_start_sample();


}