#ifndef DRV_ADC_H
#define DRV_ADC_H
#include "stm32f4xx.h"
#include "drv_spi.h"

#define ADCC_CH_NUM  6
#define ADCP_CH_NUM  5
#define ADCC_FILTER_NUM 6
#define ADCP_FILTER_NUM 5


typedef uint16_t* (*ADC_start_sample)(void);

typedef struct read_buff_c
{
	uint16_t data[ADCC_CH_NUM];
	struct read_buff_c *next;
}ReadBuffC, *pReadBuffC;

typedef struct para_filter_c
{
  uint32_t filter_len;
  ReadBuffC *filter_buf;
}FilterParaC, *pFilterParaC;

typedef struct read_buff_p {
	uint16_t data[ADCP_CH_NUM];
	struct read_buff_p *next;
}ReadBuffP, *pReadBuffP;


typedef struct{
	uint16_t adc_value_c[ADCC_CH_NUM];//   sample value ?
	uint16_t adc_value_p[ADCP_CH_NUM];
	
	//fuction handler
	ADC_start_sample ADCP_start_sample;
	ADC_start_sample ADCC_start_sample;
}ADC_SENSORS, *pADC_SENSORS;


typedef struct{
	uint16_t adc_average_c[ADCC_CH_NUM];//   sample value ?
	uint16_t adc_average_p[ADCP_CH_NUM];
}ADC_finish, *pADC_finish;



extern void ADCC_Configuration(void);
extern void ADCP_Configuration(void);

extern ADC_SENSORS* get_adc_sensor_handle(void);
extern ADC_finish * get_adc_finish_handle(void);


extern uint16_t* _ADCC_start_sample(void);
extern uint16_t* _ADCP_start_sample(void);

extern void get_readbuff_current(FilterParaC *temp_filter);
#endif
