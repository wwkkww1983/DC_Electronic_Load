/*
 * usci_b1_i2c.c
 *
 *  Created on: 2016-06-25
 *      Author: Jack Chen <redchenjs@live.com>
 *
 * ---------I2C---------
 * PORT		TYPE	PIN
 * SDA      INOUT   P4.1
 * SCL		OUT		P4.2
 * ---------------------
 */

#include <msp430.h>

#define I2C_SET_PIN()   {\
                            P4SEL |= BIT1 + BIT2;\
                        }

static unsigned char *i2c_tx_buff;
static unsigned char *i2c_rx_buff;

static unsigned char i2c_tx_num = 0;
static unsigned char i2c_rx_num = 0;

unsigned char usci_b1_i2c_transmit_frame(unsigned char slave_addr, unsigned char *p_buff,unsigned char num)
{
    if (UCB1STAT & UCBUSY) return 0;
    __disable_interrupt();
    UCB1IE |= UCTXIE;
    UCB1IE &=~UCRXIE;
    __enable_interrupt();
    i2c_tx_buff = p_buff;
    i2c_tx_num  = num;
    UCB1I2CSA = slave_addr;
    UCB1CTL1 |= UCTR + UCTXSTT;
    while (UCB1STAT & UCBUSY);
    return 1;
}

unsigned char usci_b1_i2c_receive_frame(unsigned char slave_addr, unsigned char *p_buff, unsigned char num)
{
    if (UCB1STAT & UCBUSY) return 0;
    __disable_interrupt();
    UCB1IE &=~UCTXIE;
    UCB1IE |= UCRXIE;
    __enable_interrupt();
    i2c_rx_buff = p_buff;
    i2c_rx_num  = num;
    UCB1I2CSA = slave_addr;
    UCB1CTL1 &=~UCTR;
    UCB1CTL1 |= UCTXSTT;
    while (UCB1STAT & UCBUSY);
    return 1;
 }

inline void usci_b1_i2c_tx_isr_handle(void)
{
    if (i2c_tx_num) {
        UCB1TXBUF = *i2c_tx_buff++;
        i2c_tx_num--;
    }
    else {
        UCB1CTL1 |= UCTXSTP;
        UCB0IFG  &=~UCTXIFG;
        UCB0IE   &=~UCTXIE;
    }
}

inline void usci_b1_i2c_rx_isr_handle(void)
{
    *i2c_rx_buff++ = UCB1RXBUF;
    i2c_rx_num--;
    if (i2c_rx_num) {
        if (i2c_rx_num == 1) {
            UCB1CTL1 |= UCTXSTP;
        }
    } else {
        UCB0IFG &=~UCRXIFG;
        UCB0IE  &=~UCRXIE;
    }
}

void usci_b1_i2c_init(void)
{
    I2C_SET_PIN();

    UCB1CTL1 |= UCSWRST;
    UCB1CTL0 |= UCMST + UCMODE_3 + UCSYNC;
    UCB1CTL1 |= UCSSEL_2;

    UCB1BR0 = 16;
    UCB1BR1 = 0;

    UCB1CTL1 &=~UCSWRST;
    UCB1IFG  &=~(UCTXIFG + UCRXIFG);
}
