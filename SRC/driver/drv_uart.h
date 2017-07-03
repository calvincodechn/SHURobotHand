#ifndef _DRV_UART_H
#define _DRV_UART_H

#include "stm32f4xx.h" 
#include "transfer_protocol_uart.h"

#define 	UartControl   USART1
#define 	UartSysterm   USART2
#define   DMA_Rec_Len   16

typedef void (*uart_process_on_rx)(uint8_t data);

typedef enum{
    LINE_DMA_IDLE                          =0,
    //LINE_WAIT                          =1,
    LINE_DMA_READY                         =1
}Line_MDA_Flag; 


typedef struct {
		uint8_t       Uarts_value[16];
		Line_MDA_Flag  DMA_Transfer_finish;
}Uarts_Data,*pUarts_Data;

extern void drv_uart_init(void); //串口初始化

extern void drv_put_byte(USART_TypeDef* USARTx, uint8_t data); //传输8位字节

extern void drv_put_bytes(USART_TypeDef* USARTx, const uint8_t *data, uint16_t len); //传输字符串

extern void 	uart_hand_backcall(uart_process_on_rx process_func);

extern Uarts_Data* get_Uarts_data_handle(void);

#endif

