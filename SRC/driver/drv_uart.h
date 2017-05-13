#ifndef _DRV_UART_H
#define _DRV_UART_H

#include "stm32f4xx.h" 


#define 	UartControl   USART1
#define 	UartSysterm   USART2

typedef void (*uart_process_on_rx)(uint8_t data);



extern void drv_uart_init(void);
extern void drv_put_byte(USART_TypeDef* USARTx, uint8_t data);
extern void drv_put_bytes(USART_TypeDef* USARTx, const uint8_t *data, uint16_t len);

//extern void SendPackage(unsigned char ucIsSend);
//typedef void (*proc_on_rx_t)(u8 data);
//typedef void (*hand_uart_rx_proc_t)( void * data, u32 size);
//extern void erro_message(unsigned char ucIsSend);
//extern void uart_hand_backcall(uart_process_on_rx process_func);


/*
extern signed int AD_Angle_average[6];
extern unsigned int AD_Pressure_Average[5];
extern unsigned char Pressure_On_receive[5];
extern unsigned char Motor_Angle_receive[6];
extern unsigned char ucControl_Order ;
extern unsigned char New_Reiceive_Flag ;
extern unsigned char New_Message_Flag ;*/
#endif

