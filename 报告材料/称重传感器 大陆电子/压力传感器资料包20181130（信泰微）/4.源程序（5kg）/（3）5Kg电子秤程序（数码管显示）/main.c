/**********************************************************************
----------------1.开发环境:Keil v4					   ----------------
----------------2.使用单片机型号：STC89C52RC		   ----------------

***********************************************************************/
#include "main.h"
#include "HX711.h"

unsigned long HX711_Buffer = 0;
unsigned long Weight_Maopi = 0;
long Weight_Shiwu = 0;

unsigned int Temp_Buffer = 0;
//code unsigned char table[]={ 0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xff,0x00};			//0,1,2,3,4,5,6,7,8,9,全暗，全亮			//共阳
code unsigned char table[] = { 0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0X00,0XFF,0x6f,0x79,0x77,0x3f,0x76,0x38};			//0,1,2,3,4,5,6,7,8,9,全暗，全亮,g,e,r,o,h,l			//共阴
unsigned char COM1_DATA = 0,COM2_DATA = 0,COM3_DATA = 0,COM4_DATA = 0,COM5_DATA = 0,COM6_DATA = 0;
unsigned char flag = 0;
bit Flag_ERROR = 0;


//校准参数
//因为不同的传感器特性曲线不是很一致，因此，每一个传感器需要矫正这里这个参数才能使测量值很准确。
//当发现测试出来的重量偏大时，增加该数值。
//如果测试出来的重量偏小时，减小改数值。
//该值可以为小数
#define GapValue 430


//****************************************************
//主函数
//****************************************************
void main()
{
	Init_Timer0();

	COM1_DATA =	16;				//H
	COM2_DATA = 13;				//E		
	COM3_DATA = 17;				//L		
	COM4_DATA = 17;				//L		
	COM5_DATA = 15;				//0		
	COM6_DATA = 10;				//空
				
	Delay_ms(3000);		 //延时,等待传感器稳定

	Get_Maopi();				//称毛皮重量
	
	while(1)
	{
		EA = 0;
		Get_Weight();			//称重
		EA = 1;

		Scan_Key();

		//显示当前重量
		if( Flag_ERROR == 1)
		{
			COM1_DATA = 13;			//E
			COM2_DATA = 14;			//R
			COM3_DATA = 14;			//R
			COM4_DATA = 15;			//O
			COM5_DATA = 14;			//R				
			COM6_DATA = 10;			//空	
		}
		else
		{
			COM1_DATA = Weight_Shiwu/1000;		
			COM2_DATA = Weight_Shiwu%1000/100;		
			COM3_DATA = Weight_Shiwu%100/10;		
			COM4_DATA = Weight_Shiwu%10;
			COM5_DATA = 10;			//空				
			COM6_DATA = 12;			//g	
		}
		
	}
}

//扫描按键
void Scan_Key()
{
	if(KEY1 == 0)
	{
		Delay_ms(5);
		if(KEY1 == 0)
		{
			while(KEY1 == 0);
			Get_Maopi();			//去皮
		}	
	}
}

//****************************************************
//称重
//****************************************************
void Get_Weight()
{
	Weight_Shiwu = HX711_Read();
	Weight_Shiwu = Weight_Shiwu - Weight_Maopi;		//获取净重
	if(Weight_Shiwu > 0)			
	{	
		Weight_Shiwu = (unsigned int)((float)Weight_Shiwu/GapValue); 	//计算实物的实际重量
																		
																		
		if(Weight_Shiwu > 5000)		//超重报警
		{
			Flag_ERROR = 1;	
		}
		else
		{
			Flag_ERROR = 0;
		}
	}
	else
	{
		Weight_Shiwu = 0;
		Flag_ERROR = 1;				//负重报警
	}
	
}

//****************************************************
//获取毛皮重量
//****************************************************
void Get_Maopi()
{
	Weight_Maopi = HX711_Read();	
} 


//****************************************************
//初始化定时器0
//****************************************************
void Init_Timer0()
{
	TMOD = 0X01;			//T0, 工作模式1

	TH0 = (65536 - 2000)/256;
	TL0 = (65536 - 2000)%256;

	TR0 = 1;				//开启定时器
	ET0 = 1;			    //开启定时器中断
	EA = 1;					//开启总中断		
}


//中断函数
//****************************************************
void Timer0() interrupt 1
{
	TH0 = (65536 - 2000)/256;
	TL0 = (65536 - 2000)%256;
	
	TF0 = 0;

	flag++;
	if(flag >= 6)
	{
		flag = 0;
	}


	switch(flag)
	{
		case 0:
				COM1 = 0;
				COM2 = 1;
				COM3 = 1;
				COM4 = 1;
				COM5 = 1;
				COM6 = 1;

//				if(COM1_DATA < 10)
//				{
//					SEG_DATA = table[COM1_DATA]|0x80;		////显示第一位值加小数点
//				}
//				else
//				{
					SEG_DATA = table[COM1_DATA];	
//				}
				
				break;

		case 1:
				COM1 = 1;
				COM2 = 0;
				COM3 = 1;
				COM4 = 1;
				COM5 = 1;
				COM6 = 1;
				SEG_DATA = table[COM2_DATA];		//显示第二位值
				
				break;

							  
		case 2:
				COM1 = 1;
				COM2 = 1;
				COM3 = 0;
				COM4 = 1;
				COM5 = 1;
				COM6 = 1;
				SEG_DATA = table[COM3_DATA];		////显示第三位值
				
				break;


		case 3:
				COM1 = 1;
				COM2 = 1;
				COM3 = 1;
				COM4 = 0;
				COM5 = 1;
				COM6 = 1;
				SEG_DATA = table[COM4_DATA];			  //显示第四位值
				break;

		case 4:
				COM1 = 1;
				COM2 = 1;
				COM3 = 1;
				COM4 = 1;
				COM5 = 0;
				COM6 = 1;
				SEG_DATA = table[COM5_DATA];			  //显示第四位值
				break;

		case 5:
				COM1 = 1;
				COM2 = 1;
				COM3 = 1;
				COM4 = 1;
				COM5 = 1;
				COM6 = 0;
				SEG_DATA = table[COM6_DATA];			  //显示第四位值
				break;

		default : break;
	}

} 

//****************************************************
//MS延时函数(12M晶振下测试)
//****************************************************
void Delay_ms(unsigned int n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<123;j++);
}