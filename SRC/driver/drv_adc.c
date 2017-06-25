#include "drv_adc.h"
#include "hand_define.h"

ADC_SENSORS adc_sensor;

uint16_t* _ADCC_start_sample(void);
uint16_t* _ADCP_start_sample(void);

void ADCC_Configuration()
{    
	GPIO_InitTypeDef  GPIO_InitStructure;

	ADC_CommonInitTypeDef	ADC_CommonInitStructure;
	ADC_InitTypeDef	ADC_InitStructure;
	DMA_InitTypeDef	DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_DMA2, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); 

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1| GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* DMA2 Stream0 channel0 configuration **************************************/

	DMA_InitStructure.DMA_Channel = DMA_Channel_2;  
	DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)&ADC3->DR;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t )adc_sensor.adc_value_c;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = ADCC_CH_NUM;//
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;   
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream1, &DMA_InitStructure);
	
	DMA_ITConfig(DMA2_Stream1, DMA_IT_TC, ENABLE); 
	
	DMA_Cmd(DMA2_Stream1, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_DMAAccessMode= ADC_DMAAccessMode_1;
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion =ADCC_CH_NUM;  
	ADC_Init(ADC3, &ADC_InitStructure);
		
	ADC_RegularChannelConfig(ADC3, ADC_Channel_0, 1, ADC_SampleTime_480Cycles );	 		    
	ADC_RegularChannelConfig(ADC3, ADC_Channel_2, 2, ADC_SampleTime_480Cycles );
	ADC_RegularChannelConfig(ADC3, ADC_Channel_10, 3, ADC_SampleTime_480Cycles );

	ADC_RegularChannelConfig(ADC3, ADC_Channel_11,4, ADC_SampleTime_480Cycles );	 		    
	ADC_RegularChannelConfig(ADC3, ADC_Channel_12,5, ADC_SampleTime_480Cycles );	 		    
	ADC_RegularChannelConfig(ADC3, ADC_Channel_13,6, ADC_SampleTime_480Cycles );

	ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE); 

	ADC_DMACmd(ADC3, ENABLE);
	ADC_Cmd(ADC3, ENABLE);	

}	

void ADCP_Configuration(void)
{    
	GPIO_InitTypeDef  GPIO_InitStructure;

	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef       DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_DMA2, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ADC1

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	/* DMA2 Stream0 channel0 configuration **************************************/
	
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
	DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)&ADC1->DR;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)adc_sensor.adc_value_p;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = ADCP_CH_NUM;//
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;   
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;// 
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_DMAAccessMode= ADC_DMAAccessMode_1;
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	
	ADC_InitStructure.ADC_NbrOfConversion =ADCP_CH_NUM;  
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_1,1, ADC_SampleTime_480Cycles );	 		    
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3,2, ADC_SampleTime_480Cycles );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5,3, ADC_SampleTime_480Cycles );	 		    
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6,4, ADC_SampleTime_480Cycles );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7,5, ADC_SampleTime_480Cycles );	 	
	  
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE); 
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA2_Stream0, ENABLE);
	
}	

uint16_t* _ADCP_start_sample(void)
{
	ADC_SoftwareStartConv(ADC1); 
	//while(RESET == DMA_GetFlagStatus(DMA2_Stream0,DMA_FLAG_TCIF0));
	return adc_sensor.adc_value_p;
}

uint16_t* _ADCC_start_sample(void)
{
	ADC_SoftwareStartConv(ADC3); 
	//while(RESET == DMA_GetFlagStatus(DMA2_Stream1,DMA_FLAG_TCIF2));
	return adc_sensor.adc_value_c;
}

ADC_SENSORS* get_adc_sensor_handle(void)
{
	return &adc_sensor;
}

void DMA2_Stream1_IRQHandler(void)
{
		if(DMA_GetITStatus(DMA2_Stream1, DMA_IT_TCIF1) != RESET)
		{
				DMA_ClearITPendingBit(DMA2_Stream1, DMA_IT_TCIF1);
		}
    //DMA2_Stream1->M0AR = (uint32_t )adc_sensor.adc_text;//DMA_InitStruct->DMA_Memory0BaseAddr;
}

void DMA2_Stream0_IRQHandler(void)
{
		if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) != RESET)
		{
				DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
		}
    //DMA2_Stream1->M0AR = (uint32_t )adc_sensor.adc_text;//DMA_InitStruct->DMA_Memory0BaseAddr;
}




