#ifndef _SOL_UART_H
#define _SOL_UART_H




//#include "uart.h"
//#include "sys.h"
//#include "delay.h"
//#include "usart.h"
//#include "sys_fuction.h"



#define Rec_Buffer_Size                  50


#define MOTOR_DIRVER_ADDRESS                            0x1
#define MOTOR_DRIVER_DEVICE_TYPE                        0x40
// The flag of protocol package with vehicle
#define UART_MSG_PACKAGE_STARTCHAR       0xAA        // start byte
#define UART_MSG_PACKAGE_ENDCHAR         0x0D        // end byte
//#define Rec_Buffer_Size                  50



typedef enum{
    LINE_IDLE                          =0,
    LINE_WAIT                          =1,
    LINE_READY                         =2
}LineState;     //The line of read and fetching process

typedef struct{
	int8_t data[12];
}__attribute__((packed)) PackData;


typedef enum {
	UARTCMD_MOVE_Flag			=0x50,
	UARTCMD_NOW_Flag     	=0x51,
	UARTCMD_OLD_Flag     	=0x52,
	UARTCMD_CHK_Flag      =0x53
}UART_CMD_Type;

typedef void (*send_pack_fetch)(void);
typedef void (*send_pack_Fetch)(void);
//typedef void (*receive_pack_fetch)(void);
typedef void (*check_pack_fetch)(void);



typedef enum{
	Receive_Pack					=0,
	Now_Pack,
	Old_Pack,
	Chk_Pack,
	Line_Number,
	Error_Pack						=0xFF
}PackLineNumber;

typedef struct{
      uint8_t pack_head;
      uint8_t pack_device_type;
      uint8_t pack_motor_addr;
      uint8_t pack_cmd;
      uint8_t pack_len;
      uint8_t pack_data[Rec_Buffer_Size];
      uint8_t pack_crc_l;
      uint8_t pack_crc_h;
}PackProtocol;




typedef struct{
      uint8_t pack_state;                       //一个包在读取过程中的状态
      uint8_t pack_line_state[Line_Number];                  //请求->wait; 包读来->Ready; 包取走->IDLE;     index refer to struct PackLineNumber
      PackProtocol* pack_protocol;  
}PackInterface;





extern PackInterface *get_pack_interface_handle(void);

//extern unsigned char Receive_Flag ;
//extern void  Parameter_receive_callback(receive_pack_fetch _receive_pack_Temp);
extern void  protocol_uart_send_msg( UART_CMD_Type Cmd, uint8_t *Dat, uint8_t Len );

//extern void  OLD_send_callback(send_pack_Fetch _send_pack_Temp);
//extern void  NOW_send_callback(send_pack_fetch _send_pack_temp );
//extern void  CHK_send_callback(check_pack_fetch _check_pack_Temp);
//extern void  hand_uart_on_rx(void * data_arr, u32 size);
//extern void  hand_uart_sol();

//extern unsigned char WaitForReceive;
extern uint8_t init_hand_com(void);
extern u8 *rxbuffer;


#endif
