#include "drv_spi.h"
#include <string.h>
#include "plat_algorithm.h"


SPI_SENSORS spi_sensors;
//SPI_finish  spi_finish;

//static ReadBuffA filter_angle_value[SPIA_CH_NUM];
//static ReadBuffA *filter_angle_Value = filter_angle_value;



//static void filter_arrry_init_angle(void);

static void SPI2_Configureation(void)	
{
  
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//Ê¹ÄÜGPIOBÊ±ÖÓ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//Ê¹ÄÜSPI1Ê±ÖÓ
 

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;//PB3~5¸´ÓÃ¹¦ÄÜÊä³ö	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//¸´ÓÃ¹¦ÄÜ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//ÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//ÉÏÀ­
	GPIO_Init(GPIOB, &GPIO_InitStructure);//³õÊ¼»¯
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2); //PB13¸´ÓÃÎª SPI2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2); //PB14¸´ÓÃÎª SPI2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2); //PB15¸´ÓÃÎª SPI2
	
	//ÕâÀïÖ»Õë¶ÔSPI¿Ú³õÊ¼»¯
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,ENABLE);//¸´Î»SPI1
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,DISABLE);//Í£Ö¹¸´Î»SPI1


	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //ÉèÖÃSPIµ¥Ïò»òÕßË«ÏòµÄÊý¾ÝÄ£Ê½:SPIÉèÖÃÎªË«ÏßË«ÏòÈ«Ë«¹¤
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//ÉèÖÃSPI¹¤×÷Ä£Ê½:ÉèÖÃÎªÖ÷SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;		//ÉèÖÃSPIµÄÊý¾Ý´óÐ¡:SPI·¢ËÍ½ÓÊÕ8Î»Ö¡½á¹¹
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//´®ÐÐÍ¬²½Ê±ÖÓµÄ¿ÕÏÐ×´Ì¬Îª¸ßµçÆ½
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//´®ÐÐÍ¬²½Ê±ÖÓµÄµÚ¶þ¸öÌø±äÑØ£¨ÉÏÉý»òÏÂ½µ£©Êý¾Ý±»²ÉÑù
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSSÐÅºÅÓÉÓ²¼þ£¨NSS¹Ü½Å£©»¹ÊÇÈí¼þ£¨Ê¹ÓÃSSIÎ»£©¹ÜÀí:ÄÚ²¿NSSÐÅºÅÓÐSSIÎ»¿ØÖÆ
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;		//¶¨Òå²¨ÌØÂÊÔ¤·ÖÆµµÄÖµ:²¨ÌØÂÊÔ¤·ÖÆµÖµÎª256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//Ö¸¶¨Êý¾Ý´«Êä´ÓMSBÎ»»¹ÊÇLSBÎ»¿ªÊ¼:Êý¾Ý´«Êä´ÓMSBÎ»¿ªÊ¼
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCÖµ¼ÆËãµÄ¶àÏîÊ½
	SPI_Init(SPI2, &SPI_InitStructure);  //¸ù¾ÝSPI_InitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯ÍâÉèSPIx¼Ä´æÆ÷
 
	SPI_Cmd(SPI2, ENABLE); //Ê¹ÄÜSPIÍâÉè
}
/*
static void SPI_DMA_Configureation(void)	
{
  DMA_InitTypeDef DMA_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//Ê¹ÄÜGPIOBÊ±ÖÓ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//Ê¹ÄÜSPI1Ê±ÖÓ
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);	

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;//PB3~5¸´ÓÃ¹¦ÄÜÊä³ö	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//¸´ÓÃ¹¦ÄÜ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//ÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//ÉÏÀ­
	GPIO_Init(GPIOB, &GPIO_InitStructure);//³õÊ¼»¯
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2); //PB13¸´ÓÃÎª SPI2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2); //PB14¸´ÓÃÎª SPI2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2); //PB15¸´ÓÃÎª SPI2
	
	//ÕâÀïÖ»Õë¶ÔSPI¿Ú³õÊ¼»¯
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,ENABLE);//¸´Î»SPI1
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,DISABLE);//Í£Ö¹¸´Î»SPI1


	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //ÉèÖÃSPIµ¥Ïò»òÕßË«ÏòµÄÊý¾ÝÄ£Ê½:SPIÉèÖÃÎªË«ÏßË«ÏòÈ«Ë«¹¤
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//ÉèÖÃSPI¹¤×÷Ä£Ê½:ÉèÖÃÎªÖ÷SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;		//ÉèÖÃSPIµÄÊý¾Ý´óÐ¡:SPI·¢ËÍ½ÓÊÕ8Î»Ö¡½á¹¹
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//´®ÐÐÍ¬²½Ê±ÖÓµÄ¿ÕÏÐ×´Ì¬Îª¸ßµçÆ½
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//´®ÐÐÍ¬²½Ê±ÖÓµÄµÚ¶þ¸öÌø±äÑØ£¨ÉÏÉý»òÏÂ½µ£©Êý¾Ý±»²ÉÑù
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSSÐÅºÅÓÉÓ²¼þ£¨NSS¹Ü½Å£©»¹ÊÇÈí¼þ£¨Ê¹ÓÃSSIÎ»£©¹ÜÀí:ÄÚ²¿NSSÐÅºÅÓÐSSIÎ»¿ØÖÆ
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;		//¶¨Òå²¨ÌØÂÊÔ¤·ÖÆµµÄÖµ:²¨ÌØÂÊÔ¤·ÖÆµÖµÎª256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//Ö¸¶¨Êý¾Ý´«Êä´ÓMSBÎ»»¹ÊÇLSBÎ»¿ªÊ¼:Êý¾Ý´«Êä´ÓMSBÎ»¿ªÊ¼
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCÖµ¼ÆËãµÄ¶àÏîÊ½
	SPI_Init(SPI2, &SPI_InitStructure);  //¸ù¾ÝSPI_InitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯ÍâÉèSPIx¼Ä´æÆ÷
	
	SPI_Cmd(SPI2, ENABLE); //Ê¹ÄÜSPIÍâÉè
	
	
	DMA_InitStructure.DMA_Channel = DMA_Channel_0; 
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&SPI2->DR;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)spi_sensors1->spi_value_a;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = SPIA_CH_NUM;
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
	DMA_Init(DMA1_Stream3, &DMA_InitStructure);
	 
	DMA_ITConfig(DMA1_Stream3, DMA_IT_TC, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Rx,ENABLE);
	//SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Tx,ENABLE);	
	DMA_Cmd(DMA1_Stream3,ENABLE);
	
	
}
*/


 void gpio_cs_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;   
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//ÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//ÉÏÀ­
	GPIO_Init(GPIOE, &GPIO_InitStructure);//³õÊ¼»¯   PE11ÎªADSÐ¾Æ¬µÄÊ¹ÄÜ½Å
}



void drv_ads_anglular_init(void)
{
	gpio_cs_init();
	SPI2_Configureation();
	//filter_arrry_init_angle();
	//SPI_DMA_Configureation();
}

uint16_t SPI1_ReadWriteByte(u16 TxData)
{		 			 
	uint16_t temp_rev; 
	GPIO_ResetBits(GPIOE,GPIO_Pin_11);       
	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){}//µÈ´ý·¢ËÍÇø¿Õ  
	SPI_I2S_SendData(SPI2, TxData); //Í¨¹ýÍâÉèSPIx·¢ËÍÊý¾Ý
	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){} //µÈ´ý½ÓÊÕÍê   
	temp_rev = SPI_I2S_ReceiveData(SPI2); //·µ»ØÍ¨¹ýSPIx×î½ü½ÓÊÕµÄÊý¾Ý
 
	GPIO_SetBits(GPIOE,GPIO_Pin_11);
	return temp_rev;
}


uint16_t* _SPI_start_sample(void)
{
  SPI1_ReadWriteByte(0X0000);
  spi_sensors.spi_value_a[0]=SPI1_ReadWriteByte(0X1800);
	
  SPI1_ReadWriteByte(0X0000);
  spi_sensors.spi_value_a[0]=SPI1_ReadWriteByte(0X0800);
  spi_sensors.spi_value_a[1]=SPI1_ReadWriteByte(0X1000);
  spi_sensors.spi_value_a[2]=SPI1_ReadWriteByte(0X1800);
  spi_sensors.spi_value_a[3]=SPI1_ReadWriteByte(0X2000);
  spi_sensors.spi_value_a[4]=SPI1_ReadWriteByte(0X2800);
  spi_sensors.spi_value_a[5]=SPI1_ReadWriteByte(0X0000);		//	
  return spi_sensors.spi_value_a;
}
/*
void SPI2_ReadWriteByte(u16 TxData)
{
	GPIO_ResetBits(GPIOE,GPIO_Pin_11);  
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){}//µÈ´ý·¢ËÍÇø¿Õ  
	SPI_I2S_SendData(SPI2, TxData); //Í¨¹ýÍâÉèSPIx·¢ËÍÊý¾Ý
	
 	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){} //µÈ´ý½ÓÊÕÍ
	SPI_I2S_ReceiveData(SPI2);
  GPIO_SetBits(GPIOE,GPIO_Pin_11);
}
*/
/*
void _SPI_Start_sample(void)
{
		SPI2_ReadWriteByte(0X0000);
		//SPI2_ReadWriteByte(0X0800);
		//SPI2_ReadWriteByte(0X1000);
		//SPI2_ReadWriteByte(0X1800);
		//SPI2_ReadWriteByte(0X2000);
		SPI2_ReadWriteByte(0X2800);
		//SPI2_ReadWriteByte(0X0000);
}
*/



SPI_SENSORS * get_spi_sensor_handle(void)
{
	return &spi_sensors;
}

/*
void DMA1_Stream3_IRQHandler(void)
{
		if(DMA_GetITStatus(DMA1_Stream3, DMA_IT_TCIF3) != RESET)
		{			
				memcpy(filter_angle_Value,spi_sensors.spi_value_a,12);//copy value
				filter_angle_Value = filter_angle_Value->next;//loop assignment
				if(buff_index3 < 6)
				{
						filter_buffer(filter_angle_Value,SPIA_CH_NUM,buff_index3,spi_finish.spi_average_a[buff_index3]);
						buff_index3 ++;	  
				}
				else
					 buff_index3 = 0;
				DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_TCIF3);
		}
}
*/


/*
static void filter_arrry_init_angle(void)
{
		init_circular_list(filter_angle_value,SPIA_CH_NUM);
}
*/


