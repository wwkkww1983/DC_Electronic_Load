/**********************************************************************
----------------1.开发环境:Keil v4					   ----------------
----------------2.使用单片机型号：STC89C52RC		   ----------------

***********************************************************************/

#include "main.h"
#include "HX711.h"
#include "uart.h"

unsigned long HX711_Buffer = 0;
unsigned long Weight_Maopi = 0;
long Weight_Shiwu = 0;

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
    Uart_Init();
    Send_Word("Welcome to use!\n");
    Send_Word("Made by Beetle Electronic Technology!\n");
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
            Send_Word("ERROR\n");
		}		
		else
		{
            Send_ASCII(Weight_Shiwu/1000 + 0X30);
            Send_ASCII(Weight_Shiwu%1000/100 + 0X30);
            Send_ASCII(Weight_Shiwu%100/10 + 0X30);
            Send_ASCII(Weight_Shiwu%10 + 0X30);
            Send_Word(" g\n");
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
//MS延时函数(12M晶振下测试)
//****************************************************
void Delay_ms(unsigned int n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<123;j++);
}