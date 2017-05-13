#include "stm32f4xx.h"
#include "drv_sysclk.h"
#include "drv_timer.h"
#include "drv_spi.h"
#include "inter_sensors.h"
#include "sys_fuction.h"
#include "sol_uart.h"
#include "led_blink.h"
#include "drv_uart.h"

//SENSOR_INTERFACE* Angle_sensor;



void init_dev(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	systick_init();
	drv_uart_init();
	timer_pwm_init(12,467);
	timer_Int_Init();
	gpio_motor_dir_init();
	//My_USART2_Init();
	drv_ads_anglular_init();
	//timer_Int_Init();
	//init_sensor_interface();
	//init_hand_com();
	//Hand_measurement_init();
	//Sys_Init_Value();
	drv_led_init();
	ADCC_Configuration();
	ADCP_Configuration();
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
		/*static uint64_t time_test = 0;
		if(millis() - time_test > 100)
		{
			time_test = millis();
			_SPI_start_sample();
		}*/
		
		static uint64_t time_test = 0;
		static int32_t period = 0;
		static uint8_t dir = 0;
		USART_SendData(USART1, 0x99);
		if(millis() - time_test > 10)
		{
			ADC_SENSORS* adc_sensors;
			time_test = millis();
			timer_pwm_fresh(FINGER_SEL_THUMB, period);
			timer_pwm_fresh(FINGER_SEL_INDEX, period);
			timer_pwm_fresh(FINGER_SEL_MIDDLE, period);
			timer_pwm_fresh(FINGER_SEL_RING, period);
			timer_pwm_fresh(FINGER_SEL_LITTLE, period);
			timer_pwm_fresh(FINGER_SEL_ROOT, period);	
			period++;
			if(period > 467)
			{
				period = 0;
				dir ++;
			}
			if(dir % 2)
			{
				set_motor_direction(FINGER_SEL_THUMB, SERVO_DIR_UP);
				set_motor_direction(FINGER_SEL_INDEX, SERVO_DIR_UP);
				set_motor_direction(FINGER_SEL_MIDDLE, SERVO_DIR_UP);
				set_motor_direction(FINGER_SEL_RING, SERVO_DIR_UP);
				set_motor_direction(FINGER_SEL_LITTLE, SERVO_DIR_UP);
				set_motor_direction(FINGER_SEL_ROOT, SERVO_DIR_UP);
			}
			else
			{
				set_motor_direction(FINGER_SEL_THUMB, SERVO_DIR_DOWN);
				set_motor_direction(FINGER_SEL_INDEX, SERVO_DIR_DOWN);
				set_motor_direction(FINGER_SEL_MIDDLE, SERVO_DIR_DOWN);
				set_motor_direction(FINGER_SEL_RING, SERVO_DIR_DOWN);
				set_motor_direction(FINGER_SEL_LITTLE, SERVO_DIR_DOWN);
				set_motor_direction(FINGER_SEL_ROOT, SERVO_DIR_DOWN);	
			}
			///adc_test

			_ADCC_start_sample();
			//_ADCP_start_sample();
		
		}
		
		
	
	}
}
