/*
 * dac8571.c
 *
 *  Created on: 2018-05-17 20:43
 *      Author: Jack Chen <redchenjs@live.com>
 *  Fixed on :  2018-05-25 11:14
 *      Author: art-j <jjw903427521@gmail.com>
 */


/*#include <stdint.h>

#include <inc/device/soft_i2c.h>
#include <inc/device/usci_b0_i2c.h>

#define DAC8571_I2C_ADDRESS 0x4c
#define DAC8571_UPDATE_REG  0x10

#define TAG "dac8571"

void dac8571_set_output(uint16_t value)
{
    unsigned char temp[3] = {0};
    temp[0] = DAC8571_UPDATE_REG;
    temp[1] = value >> 8;
    temp[2] = value & 0xff;
//    usci_b0_i2c_transmit_frame(DAC8571_I2C_ADDRESS, temp, 3);
    soft_i2c_transmit_frame(DAC8571_I2C_ADDRESS, temp, 3);
}

void dac8571_init(void)
{
    dac8571_set_output(0x0000);
}*/






/*
 * DAC8571.c
 *
 *  Created on: 2015-7-8
 *      Author: Administrator
 */
#include <msp430f5529.h>
#include <inc/driver/dac8571.h>
#include <src/user/display_value.h>
#define VREF 3.28//实际的基准电压

#define SCL_H P3OUT |= BIT1 //IIC
#define SDA_H P3OUT |=BIT0 ;  //IIC
#define SCL_L P3OUT &= ~BIT1 ; //IIC
#define SDA_L P3OUT &= ~BIT0 ;  //IIC
#define SDA_DIRIN  P3DIR &= ~BIT0
#define SDA_DIROUT P3DIR |= BIT0
#define SDA_IN  P3IN&=BIT0

#define CPU_F ((double)16000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define Delay_Nms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

#define uint  unsigned int
#define uchar unsigned char

uint i, n;
uchar ack=0;

void DAC8571_Init(void)
{
    P3DIR |=BIT0+BIT1;//P8_2 SDA   P8_1  SCL
    DAC8571_OUT(CC_voltage);//不要忘记初始化是DAC的输出
}

void IIC_START(void)
{
    SCL_H;                   //SCL
    delay_us( 200);
    SDA_H;
    delay_us( 200);
    SDA_L;                    //SCL
    delay_us( 200);
 }

void IIC_OVER( void )
{
    SCL_L;
    SDA_H ;
    delay_us(400);
}

void IIC_STOP(void)
{
    SCL_L;
    SDA_L;
    delay_us( 400);
    SCL_H;
    delay_us( 100);
    SDA_H;
    delay_us( 400);
}

uchar IIC_TR(uint dat )
{
    uchar ack=0;
    SDA_DIROUT;
    for( i=0; i<8; i++ )
    {
        n = 7-i;
        SCL_L;
        delay_us( 200);
        if( (( dat & (1 << n) ) >> n) )
        {
            SDA_H;
        }
        else
            SDA_L;
        delay_us( 200);
        SCL_H;
        delay_us( 200);
    }
    SCL_L;
    SDA_DIRIN;
    delay_us( 200);
    ack = SDA_IN;
    delay_us( 200);
    SCL_H;
    delay_us( 200);
    return ack;
 }

uchar DAC_Init(void)
{
    uchar ack=0;
    IIC_START();   //IIC
    ack=IIC_TR( 0x98 );//·¢ËÍµØÖ·
    if( ack != 0 )
    {
        return ack;
    }
    IIC_OVER();
    IIC_TR( 0x10 );//¿ØÖÆ¼Ä´æÆ÷Write temporary register and load DAC with data
    if( ack != 0 )
    {
        return ack;
    }
    IIC_OVER();
    return 0;
}


void DAC8571_OUT( float num )
{
    uchar ack=0,error=0;
    uint dat = 0x98;
    uint dat0, dat1;
    if( (num - VREF) > 0.0 )
    {
        error=1;
    }
    DAC_Init();
    dat = ( uint )( ( num/VREF )*65535 );
    dat1 = ( ( dat & 0xff00 ) >> 8 );
    ack =IIC_TR( dat1 );
    if( ack != 0 )
    {
        error=1;
    }
    IIC_OVER();
    dat0 = ( dat & 0x00ff );
    ack =IIC_TR( dat0 );
    if( ack != 0 )
    {
        error=1;
    }
    IIC_STOP();

    while(error==1);    //ÅÐ¶ÏÊÇ·ñÕýÈ·Êä³ö
}



