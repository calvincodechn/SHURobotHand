#ifndef _TRANSFER_PROTOCOL_UART_H
#define _TRANSFER_PROTOCOL_UART_H

#include  "drv_uart.h"
//#include "sys.h"
//#include "delay.h"



#define Rec_Buffer_Size                  50

#define UART_MSG_PACKAGE_STARTCHAR       0xAA        // start byte
#define UART_MSG_PACKAGE_ENDCHAR         0x0D        // end byte
#define HAND_DIRVER_ADR                 0x01


typedef struct{
      uint8_t pack_head;
      uint8_t pack_device_type;
      uint8_t pack_motor_addr;
      uint8_t pack_cmd;
      uint8_t pack_len;
      uint8_t pack_data[Rec_Buffer_Size];
			uint8_t pack_cheak;
}PackProtocol;


typedef struct{
	int8_t data[12];
}__attribute__((packed)) PackData;

typedef enum{
	Heart_Getflag             =0,
	Rressure_Getflag,
	Curent_Getflag,
	Angle_Getflag,
	Rressure_Pack,
	Curent_Pack,
	Angle_Pack,
	Chk_Pack,
	Line_Number,
	Error_Pack						=0xFF
}PackLineNumber;

typedef enum{
    LINE_IDLE                          =0,
    LINE_WAIT                          =1,
    LINE_READY                         =2
}LineState; 

typedef enum {
	
	UARTCMD_HEART_BIT          =0x10,  //ͬ��
	UARTCMD_Get_Pressure       =0x30,  //��ȡѹ��
	UARTCMD_Get_Current        =0x31,  //��ȡ����
	UARTCMD_Get_Angle          =0x32,  //��ȡ�Ƕ�
	
	UARTCMD_Set_Pressure 			 =0x50,  //����ѹ��
	UARTCMD_Set_Current     	 =0x51,  //���õ���
	UARTCMD_Set_Angle          =0x52,  //���ýǶ�
	UARTCMD_CHK_Flag           =0x53
}UART_CMD_Type;


typedef struct{
      uint8_t pack_state;                       //һ�����ڶ�ȡ�����е�״̬
      uint8_t pack_line_state[Line_Number];                  //����->wait; ������->Ready; ��ȡ��->IDLE;     index refer to struct PackLineNumber
      PackProtocol* pack_Protocol;  
}PackInterface;


extern uint8_t init_hand_com(void);
extern PackInterface *get_pack_interface_handle(void);
extern void protocol_uart_send_pc_msg( uint8_t *Dat, uint8_t Len, UART_CMD_Type cmd);


#endif