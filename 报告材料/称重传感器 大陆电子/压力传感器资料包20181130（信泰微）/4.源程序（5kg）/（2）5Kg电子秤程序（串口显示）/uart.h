#ifndef __UART_H__
#define __UART_H__

#include <reg52.h>



//函数或者变量声明
extern void Uart_Init();
extern void Send_Word(unsigned char *p);
extern void Send_ASCII(unsigned char c);



extern unsigned char  RX_Buffer[30];
extern unsigned char Num;
extern bit Flag_Receive_OK;




#endif