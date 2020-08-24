#include "uart.h"

//unsigned char RX_Buffer[30];
unsigned char Num = 0;
bit Flag_Receive_OK = 0;


void Uart_Init()					  			   
{
	SCON = 0X50;  //UART方式1；8位UART
	REN  = 0;     //不允许串行口接收数据
	PCON = 0x00;  //SMOD=0;波特率不加倍
	TMOD = 0x21;  //T1方式2，用于产生波特率
	TH1  = 0xFD;  //装初值
	TL1  = 0xFD;
	TR1  = 1;     //启动定时器1
	EA   = 1;     //打开全局中断控制
	ES   = 0;     //串行口中断	
}

void Send_Word(unsigned char *p)				//发送字符串
{	
 	while(*p)
 	{
	 	SBUF=*(p++);
		while(TI==0)
		{
			;
			;
			;
		};
		TI=0;
	}   
}

void Send_ASCII(unsigned char c)				//发送一个字符
{ 
    TI=0;   
    SBUF=c;   
    while(TI==0);   
    TI=0;   
}























