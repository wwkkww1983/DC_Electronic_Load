/*
 * TCA6416A.c
 *
 *  Created on: 2013-4-6
 *      Author: Administrator
 */
#include "I2C.h"

//-----控制寄存器定义-----
#define		In_CMD0			0x00	//读取管脚输入状态寄存器；只读
#define		In_CMD1			0x01
#define		Out_CMD0		0x02	//控制管脚输出状态寄存器；R/W
#define		Out_CMD1		0x03
#define		PIVS_CMD0		0x04	//反向控制管脚输出状态寄存器；R/W
#define		PIVS_CMD1		0x05
#define		CFG_CMD0		0x06	//管脚方向控制：1：In；0:：Out。
#define		CFG_CMD1		0x07
volatile unsigned int TCA6416A_InputBuffer=0;

/******************************************************************************************************
 * 名       称：TCA6416_Tx_Frame()
 * 功       能：给TCA6416发送一帧数据
 * 入口参数：*p_Tx：待发送数据的头指针
 * 					 num：数据字节数
 * 出口参数：无
 * 说       明：调用底层驱动实现。发成功为止。
 * 范       例：无
 ******************************************************************************************************/
void TCA6416_Tx_Frame(unsigned char *p_Tx,unsigned char num)
{
	unsigned char temp=0;
	do{
			temp=I2C_TxFrame(p_Tx, num);
		}while(temp==0);
}
/******************************************************************************************************
 * 名       称：TCA6416_Rx_Frame()
 * 功       能：从TCA6416A接收一帧函数
 * 入口参数： *p_Rx：待存放数据的头指针
 * 					 num：待接收字节数
 * 出口参数：无
 * 说       明：调用底层驱动函数实现。收成功为止。
 * 范       例：无
 ******************************************************************************************************/
void TCA6416_Rx_Frame(unsigned char *p_Rx,unsigned char num)
{
	unsigned char temp=0;
	do {
		temp=I2C_RxFrame(p_Rx, num);
	}while(temp==0);
}
/******************************************************************************************************
 * 名       称：TCA6416A_Init()
 * 功       能：TCA6416A初始化，
 * 入口参数：无
 * 出口参数：无
 * 说       明：实际是调用I2C初始化程序，将TCA6416A的输入输出口做定义
 * 范       例：无
 ******************************************************************************************************/
void TCA6416A_Init(void)
{
	unsigned char conf[3]={0};
	 __delay_cycles(100000);			//TCA6416的复位时间比单片机长，延迟确保可靠复位
	 I2C_Init();
	 I2C_Tx_Init();				//永远默认发模式
	//----根据扩展板的引脚使用，将按键所在管脚初始化为输入，其余管脚初始化为输出
	conf[0] = CFG_CMD0;					//TCA6416控制寄存器
	conf[1] = 0x00;						//  0 0 0 0_0 0 0 0  （LED0~LED7）
	TCA6416_Tx_Frame(conf,2);			//  写入命令字

	conf[0] = CFG_CMD1;
	conf[1] = 0x0f;						//  0 0 0 0_1 1 1 1 (按键)
	TCA6416_Tx_Frame(conf,2);			//  写入命令字

	//----上电先将管脚输出为高（此操作对输入管脚无效）
	conf[0] = Out_CMD0;
	conf[1] = 0xff;						// 某位置1，输出为高，0为低
	TCA6416_Tx_Frame(conf,2);			//  写入命令字

	conf[0] = Out_CMD1;
	conf[1] = 0xff;
	TCA6416_Tx_Frame(conf,2);			//  写入命令字
}
/******************************************************************************************************
 * 名       称：PinOUT()
 * 功       能：控制TCA6416指定port输出指定电平
 * 入口参数：无
 * 出口参数：无
 * 说       明：无
 * 范       例：无
 ******************************************************************************************************/
void PinOUT(unsigned char pin,unsigned char status)
{
	static unsigned char pinW0 = 0xff;		//用于缓存已写入相应管脚的状态信息，此操作避免读回TCA6416A中当前寄存器的值
	static unsigned char pinW1 = 0xff;		//用于缓存已写入相应管脚的状态信息，此操作避免读回TCA6416A中当前寄存器的值

	unsigned char out0_7[2]={0};				//管脚 pin0~pin7输出状态缓存
	unsigned char out10_17[2] = {0};			//管脚 pin10~pin17输出状态缓存
	if(pin<=7)												//所选管脚为pin0~pin7 ，刷新所要操作的输出缓存pinW0 状态
	{
		if(status == 0)
			pinW0 &= ~(1<<pin);
		else
			pinW0 |= 1<<pin;
		out0_7[0] = Out_CMD0;
		out0_7[1] = pinW0;
		 TCA6416_Tx_Frame(out0_7,2);					// 将更新后的数据包，写入芯片寄存器

	}
	else if(pin>=10 && pin<=17)				//所选管脚为pin10~pin17 ，刷新所要操作的输出缓存pinW1 状态
	{
		if(status == 0)
			pinW1 &= ~(1<<(pin%10));
		else
			pinW1 |= 1<<(pin%10);
		out10_17[0] = Out_CMD1;
		out10_17[1] = pinW1 ;
		 TCA6416_Tx_Frame(out10_17,2);				// 将更新后的数据包，写入芯片寄存器
	}
	else
	{
	    __no_operation();                       					// Set breakpoint >>here<<
	}

}
/******************************************************************************************************
 * 名       称：PinIN()
 * 功       能：读取TCA6416A指定port输入电平给全局变量TCA6416A_InputBuffer
 * 入口参数：无
 * 出口参数：无
 * 说       明：类似读FIFO操作，但指针无需循环
 * 范       例：无
 ******************************************************************************************************/
void PinIN()
{
	unsigned char temp[2]={0};
	unsigned char conf[1]={0};
	conf[0]=In_CMD1;
	TCA6416_Tx_Frame(conf,1);										//	写入要读取的寄存器地址命令
	I2C_Rx_Init();                 		 								// 将I2C切换到Rx模式 初始化
	TCA6416_Rx_Frame(&temp[0],1);								// 读取IO输入寄存器
	TCA6416_Rx_Frame(&temp[1],1);								// 读取IO输入寄存器，用不上也要读
	//----将最新键值，更新到输入缓存----
    TCA6416A_InputBuffer = TCA6416A_InputBuffer&0x00ff;
	TCA6416A_InputBuffer |=(((unsigned int)temp[0])<<8 )&0xff00;
	I2C_Tx_Init();
}
