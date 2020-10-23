/*
 * usci_b0_i2c.c
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
                            P3SEL |= BIT0 + BIT1;\
                        }

static unsigned char *i2c_tx_buff;
static unsigned char *i2c_rx_buff;

static unsigned char i2c_tx_num = 0;
static unsigned char i2c_rx_num = 0;

unsigned char usci_b0_i2c_transmit_frame(unsigned char slave_addr, unsigned char *p_buff,unsigned char num)
{
    if (UCB0STAT & UCBUSY) return 0;
    __disable_interrupt();
    UCB0IE |= UCTXIE;
    UCB0IE &=~UCRXIE;
    __enable_interrupt();
    i2c_tx_buff = p_buff;
    i2c_tx_num  = num;
    UCB0I2CSA = slave_addr;
    UCB0CTL1 |= UCTR + UCTXSTT;
    while (UCB0STAT & UCBUSY);
    return 1;
}

unsigned char usci_b0_i2c_receive_frame(unsigned char slave_addr, unsigned char *p_buff, unsigned char num)
{
    if (UCB0STAT & UCBUSY) return 0;
    __disable_interrupt();
    UCB0IE &=~UCTXIE;
    UCB0IE |= UCRXIE;
    __enable_interrupt();
    i2c_rx_buff = p_buff;
    i2c_rx_num  = num;
    UCB0I2CSA = slave_addr;
    UCB0CTL1 &=~UCTR;
    UCB0CTL1 |= UCTXSTT;
    while (UCB0STAT & UCBUSY);
    return 1;
 }

inline void usci_b0_i2c_rx_isr_handle(void)
{
    *i2c_rx_buff++ = UCB0RXBUF;
    i2c_rx_num--;
    if (i2c_rx_num) {
        if (i2c_rx_num == 1) {
            UCB0CTL1 |= UCTXSTP;
        }
    } else {
        UCB0IFG &=~UCRXIFG;
        UCB0IE  &=~UCRXIE;
    }
}

inline void usci_b0_i2c_tx_isr_handle(void)
{
    if (i2c_tx_num) {
        UCB0TXBUF = *i2c_tx_buff++;
        i2c_tx_num--;
    } else {
        UCB0CTL1 |= UCTXSTP;
        UCB0IFG  &=~UCTXIFG;
        UCB0IE   &=~UCTXIE;
    }
}

void usci_b0_i2c_init(void)
{
    I2C_SET_PIN();

    UCB0CTL1 |= UCSWRST;
    UCB0CTL0 |= UCMST + UCMODE_3 + UCSYNC;
    UCB0CTL1 |= UCSSEL_2;

    UCB0BR0 = 250;
    UCB0BR1 = 0;

    UCB0CTL1 &=~UCSWRST;
    UCB0IFG  &=~(UCTXIFG + UCRXIFG);
}
