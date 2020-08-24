/*
 * I2C.c
 *
 *  Created on: 2013-4-6
 *      Author: Administrator
 */
#include"MSP430G2553.h"
#include"I2C.h"

#ifdef HARD_I2C		//Begin of Hard I2C

#define TX_STATE 						0					/*I2C发送状态*/
#define RX_STATE						1					/*I2C接收状态*/
//-----对SMCLK分频产生I2C通信波特率-----
#define I2C_BAUDRATE_DIV 	14						/*I2C波特率控制*/
#define SLAVE_ADDR 				0x20				/*从机TCA6416A的地址*/
static unsigned char  TxByteCnt=0;				//剩余发送数据
static unsigned char  RxByteCnt=0;				//剩余接收数据
static unsigned char *pTxData;                     // 待发送TX 数据的指针
static unsigned char *pRxData;                     // Rx接收存放数据的指针
unsigned char I2C_State =  0;						//收发状态指示变量
/******************************************************************************************************
 * 名       称：I2C_Init()
 * 功       能：初始化USCI_B0模块为I2C模式
 * 入口参数：无
 * 出口参数：无
 * 说       明：I2C设为3线制主机状态，暂不使能Tx和Rx中断
 * 范       例：无
 ******************************************************************************************************/
void I2C_Init()
{
	 _disable_interrupts();
	  P1SEL |= BIT6 + BIT7;                      						// GPIO 配置为USCI_B0功能
	  P1SEL2|= BIT6 + BIT7;                    						 // GPIO 配置为USCI_B0功能
	  UCB0CTL1 |= UCSWRST;                      					 // 软件复位状态
	  UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;   // 同步通信I2C主机状态
	  UCB0CTL1 = UCSSEL_2 + UCSWRST;            		 // 使用SMCLK，软件复位状态
	  UCB0BR0 =I2C_BAUDRATE_DIV ;                          // 除了分频系数，实际波特率还与SMCLK有关
	  UCB0BR1 = 0;														//这一级别的分频一般不启用
	  UCB0I2CSA = SLAVE_ADDR;                    			// I2C从机地址，可在宏定义中修改
	  UCB0CTL1 &= ~UCSWRST;                     				// 开启I2C
	  _enable_interrupts();
}
/******************************************************************************************************
 * 名       称：I2C_Tx_Init()
 * 功       能：仅使能I2C的Tx中断
 * 入口参数：无
 * 出口参数：无
 * 说       明：I2C通信只能半双工，只使能一个中断，可靠
 * 范       例：无
 ******************************************************************************************************/
void I2C_Tx_Init()
{
	  _disable_interrupts();
	  while ((UCB0STAT & UCBUSY)||UCB0CTL1 & UCTXSTP);       // 确保总线空闲
	  IE2 &= ~UCB0RXIE;							//关闭Rx中断
	  I2C_State=TX_STATE;
	  IE2 |= UCB0TXIE;                          	    //允许Tx中断
	  _enable_interrupts();						// 开总中断
}
/******************************************************************************************************
 * 名       称：I2C_Rx_Init()
 * 功       能：仅使能I2C的Rx中断
 * 入口参数：无
 * 出口参数：无
 * 说       明：I2C通信只能半双工，只使能一个中断，可靠
 * 范       例：无
 ******************************************************************************************************/
void I2C_Rx_Init()
{
	 _disable_interrupts();
	  while ((UCB0STAT & UCBUSY)||UCB0CTL1 & UCTXSTP);       // 确保总线空闲
	  IE2 &= ~UCB0TXIE;							//关闭Rx中断
	  I2C_State=RX_STATE;
	  IE2 |= UCB0RXIE;                          	    //允许Tx中断
	  _enable_interrupts();						// 开总中断
}
/******************************************************************************************************
 * 名       称：I2C_TxFrame()
 * 功       能：新发送1帧数据
 * 入口参数：*p_Tx ：待发送数据的首地址
 * 					   num： 待发送数据的个数
 * 出口参数：无
 * 说       明：只有不BUSY且STOP已复位的情况下才允许发送新的帧
 * 范       例：无
 ******************************************************************************************************/
unsigned char I2C_TxFrame(unsigned char *p_Tx,unsigned char num)
{
	if ((UCB0STAT & UCBUSY)||(UCB0CTL1 & UCTXSTP))		return(0);
 	pTxData = (unsigned char *)p_Tx;     	// 更新数据指针
    TxByteCnt = num;                  					// 更新剩余发送数据个数
    UCB0CTL1 |= UCTR + UCTXSTT;            // I2C Tx位, 软件start condition
    _bis_SR_register(CPUOFF+GIE);       		// 进LPM0模式，开总中断
    return(1);
}
/******************************************************************************************************
 * 名       称：I2C_RxFrame()
 * 功       能：新接收1帧数据
 * 入口参数：*p_Tx ：数据待存放的首地址
 * 					   num： 待接收数据的个数
 * 说       明：只有不BUSY且STOP已复位的情况下才允许接收新的帧
 * 范       例：无
 ******************************************************************************************************/
unsigned char I2C_RxFrame(unsigned char *p_Rx,unsigned char num)
{
	if ((UCB0STAT & UCBUSY)||(UCB0CTL1 & UCTXSTP))		return(0);
	pRxData = (unsigned char *)p_Rx;     	// 更新数据指针
	 RxByteCnt= num;                  				// 更新剩余接收数据个数
	 UCB0CTL1 &= ~UCTR;
     UCB0CTL1 |= UCTXSTT;                    		// I2C Rx位, 软件start condition
    _bis_SR_register(CPUOFF+GIE);       	    //  进LPM0模式，开总中断
    return(1);
 }
void I2C_TxFrame_ISR(void);
void I2C_RxFrame_ISR(void);
/******************************************************************************************************
 * 名       称：USCIAB0TX_ISR()
 * 功       能：响应I2C的收发中断服务
 * 入口参数：无
 * 出口参数：无
 * 说       明：I2C的Tx和Rx共用中断入口
 * 范       例：无
 ******************************************************************************************************/
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
	 _disable_interrupts();							//等同_DINT
  if(I2C_State==TX_STATE)							//判断是收状态还是发状态
    I2C_TxFrame_ISR();                        			// 事件：发送帧
  else
	I2C_RxFrame_ISR();                     			// 事件：接收帧
  	//-------预留给主循环中唤醒CPU用途-------
  	  if(RxByteCnt == 0 || TxByteCnt == 0)	//如果没有待发送或待接收数据
  	    __bic_SR_register_on_exit(CPUOFF);   // Exit LPM0
  	 _enable_interrupts();							//等同_ENIT
}
/******************************************************************************************************
 * 名       称：I2C_TxFrame_ISR()
 * 功       能：I2C的Tx事件处理函数，发送缓存数组中的数据
 * 入口参数：无
 * 出口参数：无
 * 说       明：类似FIFO操作，但指针无需循环
 * 范       例：无
 ******************************************************************************************************/
void I2C_TxFrame_ISR(void)
{
   if (TxByteCnt)                            			// 检查数据是否发完
  {
    UCB0TXBUF = *pTxData;                   // 装填待发送数据
    pTxData++;											//数据指针移位
    TxByteCnt--;                            			// 待发送数据个数递减
  }
  else														//数据发送完毕
  {
    UCB0CTL1 |= UCTXSTP;                   	// 置停止位
    IFG2 &= ~UCB0TXIFG;                     	// 人工清除标志位（由于没有写Buffer，MCU不会自动清除）
  }
}
/******************************************************************************************************
 * 名       称：I2C_RxFrame_ISR()
 * 功       能：I2C的Rx事件处理函数，读取UCB0RXBUF写入指定缓存数组中
 * 入口参数：无
 * 出口参数：无
 * 说       明：类似读FIFO操作，但指针无需循环
 * 范       例：无
 ******************************************************************************************************/
void I2C_RxFrame_ISR(void)
{
	  if (RxByteCnt == 1)                       			// 只剩1字节没接收时（实际已经在RxBuff里了）
	      UCB0CTL1 |= UCTXSTP;                  		// 软件产生停止位
	  RxByteCnt--;                              				// 待接收字节数递减
	  *pRxData = UCB0RXBUF;                  		// 存储已接收的数据
	  *pRxData++;												//数据指针移位
}
#endif			//End of Hard I2C


#ifdef SOFT_I2C		//Begin of Soft I2C
#define MCLK_FREQ			1000000						// 此处填写实际时钟频率
//-----屏蔽硬件差异，对I2C的IO操作进行宏定义-----
#define I2C_CLK_HIGH 			P1DIR &=~BIT6;
#define I2C_CLK_LOW 			P1DIR |= BIT6; P1OUT &=~BIT6
#define I2C_DATA_HIGH 		P1DIR &=~BIT7
#define I2C_DATA_LOW 		P1DIR |= BIT7; P1OUT &=~BIT7
#define I2C_DATA_IN			P1IN&BIT7
#define I2C_START 				Start()
#define I2C_STOP 				 	Stop()
//-----从机地址宏定义
#define SLAVE_ADDR			0x20				//填写实际的从机地址
#define SLAVE_ADDR_W	 	SLAVE_ADDR<<1			//自动生成，不用修改
#define SLAVE_ADDR_R		(SLAVE_ADDR<<1)+1	//自动生成，不用修改

//-----为兼容硬件I2C编写的空函数
void I2C_Tx_Init(){}
void I2C_Rx_Init(){}
/******************************************************************************************************
 * 名       称：delay_us()
 * 功       能：us级精确延时
 * 入口参数：无
 * 出口参数：无
 * 说       明：实际延时值与CPU时钟频率有关，所以使用了MCLK_FREQ宏定义实现自动调整
 * 					不同的I2C设备的速度不一样，并不一定遵守100kHz和400kHz的规范。所以延时
 * 					参数可视情况添加。
 * 范       例：无
 ******************************************************************************************************/
static void delay_us()
{
//	_delay_cycles(MCLK_FREQ/1000);				//1000us延时
//	_delay_cycles(MCLK_FREQ/10000);			//100us延时
//	_delay_cycles(MCLK_FREQ/100000);			//10us延时
//	_delay_cycles(MCLK_FREQ/1000000);		//1us延时
}
/******************************************************************************************************
 * 名       称：Start()
 * 功       能：模拟I2C的起始位
 * 入口参数：无
 * 出口参数：无
 * 说       明：无
 * 范       例：无
 ******************************************************************************************************/
void Start()
{
	I2C_DATA_HIGH;
	delay_us();
	I2C_CLK_HIGH;
	delay_us();
	I2C_DATA_LOW;
	delay_us();
	I2C_CLK_LOW;
	delay_us();
}
/******************************************************************************************************
 * 名       称：Stop()
 * 功       能：模拟I2C的停止位
 * 入口参数：无
 * 出口参数：无
 * 说       明：无
 * 范       例：无
 ******************************************************************************************************/
void Stop()
{
	I2C_CLK_LOW;
	delay_us();
	I2C_DATA_LOW;
	delay_us();
	I2C_CLK_HIGH;
	delay_us();
	I2C_DATA_HIGH;
	delay_us();
}
/******************************************************************************************************
 * 名       称：I2C_Init()
 * 功       能：兼容硬件I2C的格式，给一个初始化函数
 * 入口参数：无
 * 出口参数：无
 * 说       明：无
 * 范       例：无
 ******************************************************************************************************/
void I2C_Init()
{
	P1DIR |= BIT6;  		//SCL管脚为输出
	P1DIR &= ~BIT7;	 //SDA管脚为输入
	I2C_CLK_LOW;
	I2C_STOP;
}
/******************************************************************************************************
 * 名       称：Send_Char()
 * 功       能：主机向从机发送1个字节数据
 * 入口参数：无
 * 出口参数：无
 * 说       明：发完了8位后，从机有一个应答位。
 * 范       例：无
 ******************************************************************************************************/
void Send_Char(unsigned char data)
{
	unsigned char i=0;
	for(i=0;i<8;i++)
	{
		if((data<<i)&BIT7)
			I2C_DATA_HIGH;
		else
			I2C_DATA_LOW;
		I2C_CLK_HIGH;
		delay_us();
		I2C_CLK_LOW ;
		delay_us();
	}
	//----最后1个CLK，接收从机应答位，但不作判断----
	I2C_CLK_HIGH  ;
	delay_us();
	I2C_CLK_LOW;
	delay_us();
}
/******************************************************************************************************
 * 名       称： Master_Ack()
 * 功       能：主机的接收应答
 * 入口参数：无
 * 出口参数：无
 * 说       明：当主机接收完从机8个字节数据后，主机要控制数据线发出0应答信号。之后再释放总线。
 * 范       例：无
 ******************************************************************************************************/
void Master_Ack()
{
	I2C_DATA_LOW;	//主机控制数据线，给0信号
	delay_us();
	I2C_CLK_HIGH;	//主机发出应答位0
	delay_us();
	//-----释放总线-----
	I2C_CLK_LOW;
	delay_us();
	I2C_DATA_HIGH;
}
/******************************************************************************************************
 * 名       称：Get_Char()
 * 功       能：主机接收从机1个字节数据
 * 入口参数：无
 * 出口参数：无
 * 说       明：接受完最后一位数据后，主机要给出应答位
 * 范       例：无
 ******************************************************************************************************/
unsigned char Get_Char()
{
	unsigned char i=0;
	unsigned char Temp=0;
	I2C_DATA_HIGH;
	for(i=0;i<8;i++)
	{
		I2C_CLK_HIGH;
		delay_us();
		Temp=Temp<<1;
		if((I2C_DATA_IN )== BIT7)		//先收高位
			Temp |=BIT0;
		delay_us();
		I2C_CLK_LOW ;
		delay_us();
	}
	//-----应答位-----
	Master_Ack();
	return(Temp);
}
/******************************************************************************************************
 * 名       称：I2C_TxFrame(unsigned char *p_Tx,unsigned char num)
 * 功       能：主机发送若干字节的1帧数据
 * 入口参数：无
 * 出口参数：无
 * 说       明：无
 * 范       例：无
 ******************************************************************************************************/
unsigned char I2C_TxFrame(unsigned char *p_Tx,unsigned char num)
{
	_disable_interrupts();
	unsigned char i=0;
	 I2C_START;
	 Send_Char(SLAVE_ADDR_W);		//先发送器件地址和写标志
	for(i=0;i<num;i++)						//然后依次发送各字节数据
	{
		Send_Char(*p_Tx);
		p_Tx++;
	}
	I2C_STOP;
	 _enable_interrupts();
	return 1;
}
/******************************************************************************************************
 * 名       称：I2C_RxFrame(unsigned char *p_Tx,unsigned char num)
 * 功       能：主机接收若干字节的1帧数据
 * 入口参数：无
 * 出口参数：无
 * 说       明：无
 * 范       例：无
 ******************************************************************************************************/
unsigned char I2C_RxFrame(unsigned char *p_Tx,unsigned char num)
{
	unsigned char i=0;
	_disable_interrupts();;
	 I2C_START;
	 Send_Char(SLAVE_ADDR_R);		//先发送器件地址和读标志
	for(i=0;i<num;i++)						//然后依次接收各个字节数据
	{
		*p_Tx=Get_Char();
		p_Tx++;
	}
	I2C_STOP;
	 _enable_interrupts();
	return 1;
}
#endif		//End of Soft I2C



