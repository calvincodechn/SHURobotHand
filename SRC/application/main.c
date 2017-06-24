#include "stm32f4xx.h"
#include "drv_sysclk.h"
#include "drv_timer.h"
#include "drv_spi.h"
#include "inter_sensors.h"
//#include "sys_fuction.h"
#include "transfer_protocol_uart.h"
#include "Hand_Main_Fuction.h"

#include "led_blink.h"
#include "drv_uart.h"
#include "current_loop.h"

//SENSOR_INTERFACE* Angle_sensor;



void init_dev(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	systick_init();
	drv_uart_init();
	timer_pwm_init(12,467);
	timer_Int_Init();
	gpio_motor_dir_init();
	drv_ads_anglular_init();
	init_sensor_interface();
	//init_hand_com();

	drv_led_init();
	current_loop_init();
}

static int32_t loop(void)
{
	led_blink(500);	
	return 1;
}

int main(void)
{
	init_dev();

	while(loop())
	{		
	  //_ADCC_start_sample();
	}
}
