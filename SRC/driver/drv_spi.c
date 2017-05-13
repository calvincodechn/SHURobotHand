#include "drv_spi.h"
 
 
SPI_SENSORS spi_sensors;
 

 
 
static void SPI2_Configureation(void)	
{
  
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//ʹ��SPI1ʱ��
 

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;//PB3~5���ù������	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2); //PB13����Ϊ SPI2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2); //PB14����Ϊ SPI2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2); //PB15����Ϊ SPI2
	
	//����ֻ���SPI�ڳ�ʼ��
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,ENABLE);//��λSPI1
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,DISABLE);//ֹͣ��λSPI1


	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����
}   

 void gpio_cs_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;   
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��   PE11ΪADSоƬ��ʹ�ܽ�*/
}

void drv_ads_anglular_init(void)
{
	gpio_cs_init();
	SPI2_Configureation();
}


uint16_t SPI1_ReadWriteByte(u16 TxData)
{		 			 
	uint16_t temp_rev; 
	GPIO_ResetBits(GPIOE,GPIO_Pin_11);       
	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������  
	SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx��������
	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){} //�ȴ�������   
	temp_rev = SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����
 
	GPIO_SetBits(GPIOE,GPIO_Pin_11);
	return temp_rev;
}


uint16_t* _SPI_start_sample(void)
{
		
		//SPI1_ReadWriteByte(0X0000);
		//spi_sensors.spi_value_a[0]=SPI1_ReadWriteByte(0X1800);
		
		SPI1_ReadWriteByte(0X0000);
		spi_sensors.spi_value_a[0]=SPI1_ReadWriteByte(0X0800);
		spi_sensors.spi_value_a[1]=SPI1_ReadWriteByte(0X1000);
		spi_sensors.spi_value_a[2]=SPI1_ReadWriteByte(0X1800);
 		spi_sensors.spi_value_a[3]=SPI1_ReadWriteByte(0X2000);
 		spi_sensors.spi_value_a[4]=SPI1_ReadWriteByte(0X2800);
 		spi_sensors.spi_value_a[5]=SPI1_ReadWriteByte(0X0000);		//*/
		
		return spi_sensors.spi_value_a;
}


SPI_SENSORS * get_spi_sensor_handle(void)
{
	return &spi_sensors;
}

