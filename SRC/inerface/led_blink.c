#include"drv_sysclk.h"
#include"led_blink.h"

void led_blink(int32_t blink_frequ)	
{
	static uint64_t led_blink_time = 0;
	if( millis() - led_blink_time > blink_frequ )
	{
		led_blink_time = millis();
		drv_led_toggle();
	}
}





