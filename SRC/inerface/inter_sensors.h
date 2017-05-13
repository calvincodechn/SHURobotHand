#ifndef INTER_SENSORS_H
#define INTER_SENSORS_H

#include <stdint.h>
#include "drv_adc.h"
#include "drv_spi.h"




typedef struct{
	//the sensors of fingers
	uint16_t *finger_pressure_value;
	
	//the sensors of servo system
	uint16_t *servo_current_value;
	uint16_t *servo_angle_value;
	
	SPI_Start_Sample start_angle_sample;
	ADC_start_sample start_pressure_sample;
	ADC_start_sample start_current_sample;
	
}SENSOR_INTERFACE, *pSENSOR_ALL;




//================================================================================//

extern void init_sensor_interface(void);
extern SENSOR_INTERFACE* get_sensor_interface_handle(void);


#endif

