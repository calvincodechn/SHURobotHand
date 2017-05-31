#include "drv_uart.h"
#include "arm_math.h"
//static proc_on_rx_t  _rx_proc = NULL;
//hand_uart_rx_proc_t   rx_port;
//unsigned char SendFlag = 0x01;
static uart_process_on_rx _rx_process_hand = NULL;


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
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5 | GPIO_PinSource6,GPIO_AF_USART2);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate=baudrate;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART2,&USART_InitStructure);

	USART_Cmd(USART2 ,ENABLE);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
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

	