#ifndef __MAIN_H__
#define __MAIN_H__


#include <reg52.h>

//IO设置

sbit KEY1 = P3^2;
sbit KEY2 = P3^3;
sbit KEY3 = P3^4;


//函数或者变量声明
extern void Delay_ms(unsigned int n);
extern void Get_Maopi();
extern void Get_Weight();
extern void Scan_Key();


#endif