#include "angle_loop.h"
#include "drv_spi.h"
#include "drv_timer.h"
#include "plat_algorithm.h"
#include  <string.h>
static void filter_arrry_init_angle(void);

static ReadBuffA filter_angle_value[SPIA_CH_NUM];
static ReadBuffA *filter_angle_Value = filter_angle_value;

static uint8_t buff_index3 = 0;
SPI_finish  spi_finish;
void angle_loop_main(void);

void angle_loop_init(void)
{
    angle_sys_callback(angle_loop_main);
	  filter_arrry_init_angle();
}


static void filter_angle_fuction(void)
{
		SPI_SENSORS *spi_sensor;
		spi_sensor = get_spi_sensor_handle();
		memcpy(filter_angle_Value,spi_sensor->spi_value_a,12);//copy value
		filter_angle_Value = filter_angle_Value->next;//loop assignment
		if(buff_index3 < 6)
		{
				filter_buffer(filter_angle_Value,SPIA_CH_NUM,buff_index3,spi_finish.spi_average_a[buff_index3]);
				buff_index3 ++;	  
		}
		else
				buff_index3 = 0;

}


void angle_loop_main(void)
{
		
	 _SPI_start_sample();
	filter_angle_fuction();
	 
}



static void filter_arrry_init_angle(void)
{
		init_circular_list(filter_angle_value,SPIA_CH_NUM);
}