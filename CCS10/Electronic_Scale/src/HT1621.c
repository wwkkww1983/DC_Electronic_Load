/*
 * HT1621.c
 *
 *  Created on: 2013-3-30
 *      Author: Administrator
 */

#include "msp430g2553.h"
#include "HT1621.h"
#include "TCA6416A.h"
#include"LCD_128.h"

//-----控制信号线宏定义-----
#define HT1621_CS_LOW       	PinOUT(14,0)
#define HT1621_CS_HIGH  		PinOUT(14,1)
#define HT1621_RD_LOW       	PinOUT(15,0)
#define HT1621_RD_HIGH  		PinOUT(15,1)
#define HT1621_WR_LOW   		PinOUT(16,0)
#define HT1621_WR_HIGH      	PinOUT(16,1)
#define HT1621_DATA_LOW     PinOUT(17,0)
#define HT1621_DATA_HIGH 	PinOUT(17,1)
//-----Ht1621命令宏定义-----
#define HT1621_COMMAND 		4			//100
#define HT1621_WRITEDISBUF		5			//101
#define HT1621_INIT						0x29 	//开启LCD驱动+1/3bias+4coms
#define HT1621_ON						0x03 	//开启LCD驱动+1/3bias+4coms
#define HT1621_OSC						0x01		//开启内部振荡器
/******************************************************************************************************
 * 名       称：HT1621_SendBit()
 * 功       能：向HT1621发送一位
 * 入口参数：Code：所有位都是0则数据线置低，反之置高
 * 出口参数：无
 * 说       明：无
 * 范       例：无
 ******************************************************************************************************/
void HT1621_SendBit(unsigned int Code)
{
	HT1621_WR_LOW;
	if (Code==0)
	{
		HT1621_DATA_LOW;
	}
	else
	{
		HT1621_DATA_HIGH;
	}
	HT1621_WR_HIGH;
}
/******************************************************************************************************
 * 名       称：HT1621_init()
 * 功       能：HT1621的初始化函数
 * 入口参数：无
 * 出口参数：无
 * 说       明：初始化为开启LCD驱动，1/3Bias，4COM
 * 范       例：无
 ******************************************************************************************************/
void HT1621_init()
{
   unsigned char  i =0;
   unsigned char Temp=0;
   // ----将全部信号线拉高，再CS使能----
   HT1621_CS_HIGH;
   HT1621_WR_HIGH;
   HT1621_DATA_HIGH;
   HT1621_CS_LOW;
   //----发送初始化命令字100，表明是写命令-------
   Temp=HT1621_COMMAND;
   for(i=0;i<3;i++)
   {
	 HT1621_SendBit(Temp&BIT2);
	 Temp=Temp<<1;
   }
   //-----开启LCD驱动-----
   Temp=HT1621_ON;
   for(i=0;i<8;i++)
   {
	 HT1621_SendBit(Temp&BIT7);
	 Temp=Temp<<1;
   }
   	HT1621_WR_LOW;	//-----空发1位X-----
	HT1621_WR_HIGH;
   //-----开启设为1/3bias和4com-----
   Temp=HT1621_INIT;
   for(i=0;i<8;i++)
   {
	 HT1621_SendBit(Temp&BIT7);
	 Temp=Temp<<1;
   }
	HT1621_WR_LOW;	  //-----空发1位X-----
	HT1621_WR_HIGH;
	//-----开启内部振荡器-----
   Temp=HT1621_OSC;
   for(i=0;i<8;i++)
   {
	 HT1621_SendBit(Temp&BIT7);
	 Temp=Temp<<1;
   }
	HT1621_WR_LOW;	//-----空发1位X-----
	HT1621_WR_HIGH;
    HT1621_CS_HIGH;	//----CS置高，片选禁止
}
/******************************************************************************************************
 * 名       称：HT1621_Reflash()
 * 功       能：更新HT1621的显示缓存
 * 入口参数： *p：显存的映射指针
 * 出口参数：无
 * 说       明：无
 * 范       例：无
 ******************************************************************************************************/
void HT1621_Reflash(unsigned int *p)
{
   unsigned char i =0,j=0;
   unsigned int Temp=0;
   HT1621_CS_LOW;		//使能
   //----发送命令字101，表明是写显存-------
   for(i=0;i<3;i++)
   {
	 HT1621_SendBit((HT1621_WRITEDISBUF<<i)&BIT2);
   }
   //-----发送显存首地址000000----
   for(i=0;i<=5;i++)
   {
	   HT1621_SendBit(0);
   }
   //----发送128位显存，分8字每字16位-------
   for(i=0;i<8;i++)
   {
	   Temp= *p++;
	   for(j=0;j<16;j++)
	     {
	  	 HT1621_SendBit(Temp&BIT0);
	  	 Temp=Temp>>1;
	     }
   }
     HT1621_CS_HIGH;		//使能禁止
}
/******************************************************************************************************
 * 名       称：HT1621_Reflash_Digit()
 * 功       能：只更新8字数字所在的段
 * 入口参数：Position：8字段的位置
 * 出口参数：无
 * 说       明：
 * 范       例：无
 ******************************************************************************************************/
void HT1621_Reflash_Digit(unsigned char Position)
{
  unsigned char Num_Buffer=0;
  unsigned char Addr=0;
   unsigned char i =0;
   Calculate_NumBuff( Position,&Num_Buffer,&Addr);

   HT1621_CS_LOW;		//使能
   //-----发送命令字101，表明是写显存-------
   for(i=0;i<3;i++)
   {
	 HT1621_SendBit((HT1621_WRITEDISBUF<<i)&BIT2);
   }
   //----发送6位待写显存首地址------
   for(i=0;i<6;i++)
   {
	 HT1621_SendBit((Addr<<i)&BIT5);
   }

    //----发送1字节-------
   for(i=0;i<8;i++)
	 {
	 HT1621_SendBit(Num_Buffer&BIT0);
	 Num_Buffer=Num_Buffer>>1;
	 }
     HT1621_CS_HIGH;		//使能禁止
}
