/*
 * soft_i2c.c
 *
 *  Created on: 2016-06-25
 *      Author: Jack Chen <redchenjs@live.com>
 *
 * ---------I2C---------
 * PORT		TYPE	PIN
 * SDA      INOUT   P3.0
 * SCL		OUT		P3.1
 * ---------------------
 */

#include <msp430.h>

#define I2C_SET_PIN()   {\
                            P3DIR &=~BIT0;\
                            P3OUT &=~BIT0;\
                            P3DIR &=~BIT1;\
                            P3OUT &=~BIT1;\
                         }
#define I2C_SDA_SET()   P3DIR &=~BIT0
#define I2C_SDA_CLR()   P3DIR |= BIT0
#define I2C_SDA_IN()    P3IN  &  BIT0
#define I2C_SCL_SET()   P3DIR &=~BIT1
#define I2C_SCL_CLR()   P3DIR |= BIT1

#define CPU_FREQ  25000000

static inline void delay_us(void)
{
	__delay_cycles(CPU_FREQ / 2000000);
}

static void i2c_start(void)
{
	I2C_SDA_SET();
	delay_us();
	I2C_SCL_SET();
	delay_us();
	I2C_SDA_CLR();
	delay_us();
	I2C_SCL_CLR();
}

static void i2c_stop(void)
{
	I2C_SDA_CLR();
	delay_us();
	I2C_SCL_SET();
	delay_us();
	I2C_SDA_SET();
}

static void transmit_ack(unsigned char ack)
{
	if (ack) {
		I2C_SDA_CLR();
	} else {
		I2C_SDA_SET();
	}
	delay_us();
	I2C_SCL_SET();
	delay_us();
	I2C_SCL_CLR();
}

static unsigned char receive_ack(void)
{
	unsigned char temp;

	I2C_SDA_SET();
	delay_us();
	I2C_SCL_SET();
	temp = I2C_SDA_IN();
	delay_us();
	I2C_SCL_CLR();

	return temp;
}

static void transmit_char(unsigned char data)
{
	unsigned char i=0;
	for (i=8; i>0; i--) {
		if (data & BIT7) {
			I2C_SDA_SET();
		} else {
			I2C_SDA_CLR();
		}
		data <<= 1;
		delay_us();
		I2C_SCL_SET();
		delay_us();
		I2C_SCL_CLR();
	}
	receive_ack();
}

static unsigned char receive_char(unsigned char ack)
{
	unsigned char i=0;
	unsigned char temp=0;
	I2C_SDA_SET();
	for (i=8; i>0; i--) {
		delay_us();
		I2C_SCL_SET();
		temp = temp<<1;
		if (I2C_SDA_IN()) {
			temp |= BIT0;
		}
		delay_us();
		I2C_SCL_CLR();
	}
	transmit_ack(ack);
	return temp;
}

unsigned char soft_i2c_transmit_frame(unsigned char slave_addr, unsigned char *p_buff, unsigned char num)
{
	unsigned char i=0;
	__disable_interrupt();
	i2c_start();
	transmit_char(slave_addr<<1);
	for (i=num; i>0; i--)
	{
		transmit_char(*p_buff);
		p_buff++;
	}
	i2c_stop();
	__enable_interrupt();
	return 1;
}

unsigned char soft_i2c_receive_frame(unsigned char slave_addr, unsigned char *p_buff, unsigned char num)
{
	unsigned char i=0;
	__disable_interrupt();
	i2c_start();
	transmit_char((slave_addr<<1) + 0x01);
	for (i=num; i>0; i--) {
		*p_buff=receive_char(i-1);
		p_buff++;
	}
	i2c_stop();
	__enable_interrupt();
	return 1;
}

void soft_i2c_init(void)
{
    I2C_SET_PIN();//I2C的通信协议初始化
}
