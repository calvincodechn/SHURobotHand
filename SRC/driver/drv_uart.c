#include "drv_uart.h"
#include "arm_math.h"
//static proc_on_rx_t  _rx_proc = NULL;
//hand_uart_rx_proc_t   rx_port;
//unsigned char SendFlag = 0x01;
static uart_process_on_rx  _rx_process_hand = NULL;
Uarts_Data                 uarts_data;

static void _drv_uart_control_init(uint32_t baudrate)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9 , GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStructure);
  
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART1 ,ENABLE);
}

static void _drv_uart_system_init(uint32_t baudrate)
{
	
		DMA_InitTypeDef DMA_InitStructure;
	
	
		GPIO_InitTypeDef   GPIO_InitStructure;
		USART_InitTypeDef  USART_InitStructure;
		NVIC_InitTypeDef   NVIC_InitStructure;
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART1时钟
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2);
   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	
		USART_InitStructure.USART_BaudRate=115200;
		USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
		USART_InitStructure.USART_Parity=USART_Parity_No;
		USART_InitStructure.USART_StopBits=USART_StopBits_1;
		USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	
		USART_Init(USART2,&USART_InitStructure);
		USART_Cmd(USART2 ,ENABLE);
  
		USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
		USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
		
		USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
	
		NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
		NVIC_Init(&NVIC_InitStructure);
		//USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
		//DMA_DeInit(DMA1_Stream5); 

		DMA_InitStructure.DMA_Channel = DMA_Channel_4; 
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR; 
		DMA_InitStructure.DMA_Memory0BaseAddr = (u32)uarts_data.Uarts_value;  
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;  
		DMA_InitStructure.DMA_BufferSize = DMA_Rec_Len;  
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  
		DMA_InitStructure.DMA_Priority = DMA_Priority_High; 
		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;   
		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; 
		DMA_Init(DMA1_Stream5, &DMA_InitStructure); 
		
		DMA_ITConfig(DMA1_Stream5, DMA_IT_TC, ENABLE);
		
		DMA_Cmd(DMA1_Stream5, ENABLE); 
		
		NVIC_InitTypeDef NVIC_InitStructure1;

		NVIC_InitStructure1.NVIC_IRQChannel = DMA1_Stream5_IRQn;
		NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 2;
		NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure1);
}






void drv_uart_init(void)
{
		_drv_uart_control_init(115200);
		_drv_uart_system_init(115200);
}


void 	uart_hand_backcall(uart_process_on_rx process_func)
{
		_rx_process_hand = process_func;
}



void drv_put_byte(USART_TypeDef* USARTx, uint8_t data)
{
     USART_SendData(USARTx, data);
	 while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)!=SET);
}
	

void drv_put_bytes(USART_TypeDef* USARTx, const uint8_t *data, uint16_t len)
{
    while(len--)
    {
				drv_put_byte(USARTx, *data++);
    }
}

/**********uart to communicate with the control port***************************/
/***********************TTL<->232*************************/
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(UartControl, USART_IT_RXNE) != RESET)
	{	
		uint8_t temp;
		temp = USART_ReceiveData(UartControl)& 0xFF;
		//if(_rx_process_hand)
		// _rx_process_hand(temp);
		//drv_put_byte(UartControl, temp);
		//USART_SendData(UartControl, temp);
	}
}

/**********uart to communicate with the system debug port***************************/
/******************************TTL***************************************/
/*
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(UartSysterm, USART_IT_RXNE) != RESET)
	{
		uint8_t temp;
		temp = USART_ReceiveData(UartSysterm) & 0xFF;
		if(_rx_process_hand)
			_rx_process_hand(temp);
		 //USART_SendData(UartSysterm, temp);
	}
}
*/


void USART2_IRQHandler(void)                //??1??????
{
	
     if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) 
      {
         USART_ReceiveData(USART2);
			
         USART_ClearITPendingBit(USART2,USART_IT_IDLE);    
     } 
}

void DMA1_Stream5_IRQHandler(void)
{
		if(DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5) != RESET)
		{
				uarts_data.DMA_Transfer_finish = LINE_DMA_READY;
				DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);
		}
}


Uarts_Data* get_Uarts_data_handle(void)
{
		return &uarts_data;
}
