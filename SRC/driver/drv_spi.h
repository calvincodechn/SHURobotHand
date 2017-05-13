#ifndef __DRV_SPI_H
#define __DRV_SPI_H

#include "stm32f4xx.h"


#define SPIA_CH_NUM 6

typedef uint16_t* (*SPI_Start_Sample)(void);


typedef struct{

	uint16_t spi_value_a[SPIA_CH_NUM];
	
	SPI_Start_Sample SPI_start_sample;
}SPI_SENSORS, *pSPI_SENSORS;




//==================================================================================
extern void drv_ads_anglular_init(void);
extern uint16_t* _SPI_start_sample(void);
extern SPI_SENSORS * get_spi_sensor_handle(void);



#endif

