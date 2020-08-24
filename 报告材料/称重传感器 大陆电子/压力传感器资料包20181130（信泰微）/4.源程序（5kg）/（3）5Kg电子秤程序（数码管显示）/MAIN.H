#ifndef __MAIN_H__
#define __MAIN_H__


#include <reg52.h>

//IO设置

#define SEG_DATA P0	

sbit COM1 = P2^7;
sbit COM2 = P2^6;
sbit COM3 = P2^5;
sbit COM4 = P2^4;
sbit COM5 = P2^3;
sbit COM6 = P2^2;

sbit KEY1 = P3^2;


//函数或者变量声明
extern void Delay_ms(unsigned int n);
extern void Get_Maopi();
extern void Get_Weight();
extern void Init_Timer0();
extern void Scan_Key();


#endif