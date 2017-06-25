#ifndef DRV_ADC_H
#define DRV_ADC_H
#include "stm32f4xx.h"


#define ADCC_CH_NUM  6
#define ADCP_CH_NUM  5
#define ADCC_FILTER_NUM 6
#define ADCP_FILTER_NUM 5


typedef uint16_t* (*ADC_start_sample)(void);

typedef struct read_buff_c {
	uint32_t data[ADCC_CH_NUM];
	struct read_buff_c *next;
}ReadBuffC, *pReadBuffC;
typedef struct read_buff_p {
	uint32_t data[ADCC_CH_NUM];
	struct read_buff_p *next;
}ReadBuffP, *pReadBuffP;


typedef struct{
	uint16_t adc_value_c[ADCC_CH_NUM];//   sample value ?
	uint16_t adc_value_p[ADCP_CH_NUM];
	
	//fuction handler
	ADC_start_sample ADCP_start_sample;
	ADC_start_sample ADCC_start_sample;
}ADC_SENSORS, *pADC_SENSORS;

extern void ADCC_Configuration(void);
extern void ADCP_Configuration(void);

extern ADC_SENSORS* get_adc_sensor_handle(void);

extern uint16_t* _ADCC_start_sample(void);
extern uint16_t* _ADCP_start_sample(void);
#endif
