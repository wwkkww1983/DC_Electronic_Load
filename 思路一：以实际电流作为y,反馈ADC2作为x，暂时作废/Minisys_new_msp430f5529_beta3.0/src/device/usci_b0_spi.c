/*
 * usci_b0_spi.c
 *
 *  Created on: 2016-10-20
 *      Author: Jack Chen <redchenjs@live.com>
 * 
 * ---------SPI---------
 * PORT		TYPE	PIN
 * MOSI		OUT		P3.0
 * MISO		IN		P3.1
 * SCK      OUT     P3.2
 * ---------------------
 */

#include <msp430.h>

#define SPI_SET_PIN()   {\
                            P3SEL |= BIT0 + BIT1 + BIT2;\
                        }

static unsigned char *spi_tx_buff;
static unsigned char *spi_rx_buff;

static unsigned char spi_tx_num = 0;
static unsigned char spi_rx_num = 0;

unsigned char usci_b0_spi_transmit_frame(unsigned char *p_buff, unsigned char num)
{
	if (UCB0STAT & UCBUSY) return 0;
	__disable_interrupt();
    UCB0IE |= UCTXIE;
	UCB0IE &=~UCRXIE;
	__enable_interrupt();
    spi_tx_buff = p_buff;
    spi_tx_num  = num;
	UCB0TXBUF = *spi_tx_buff++;
    while (UCB0STAT & UCBUSY);
	return 1;
}

unsigned char usci_b0_spi_receive_frame(unsigned char *p_buff, unsigned char num)
{
    if (UCB0STAT & UCBUSY) return 0;
	__disable_interrupt();
    UCB0IE &=~UCTXIE;
    UCB0IE |= UCRXIE;
    __enable_interrupt();
    spi_rx_buff = p_buff;
    spi_rx_num  = num;
    UCB0TXBUF = 0xff;
    while (UCB0STAT & UCBUSY);
	return 1;
}

void usci_b0_spi_init(void)
{
    SPI_SET_PIN();

    UCB0CTL1 |= UCSWRST;
    UCB0CTL0 |= UCMST + UCMODE_0 + UCSYNC + UCCKPL + UCMSB;
    UCB0CTL1 |= UCSSEL_2;

    UCB0BR0  = 1;
    UCB0BR1  = 0;

    UCB0CTL1 &=~UCSWRST;
    UCB0IFG  &=~(UCTXIFG + UCRXIFG);
}

inline void usci_b0_spi_tx_isr_handle(void)
{
    UCB0RXBUF;
    spi_tx_num--;
    if (spi_tx_num) {
        UCB0TXBUF = *spi_tx_buff++;
    } else {
        UCB0IFG &=~UCTXIFG;
        UCB0IE  &=~UCTXIE;
    }
}

inline void usci_b0_spi_rx_isr_handle(void)
{
    *spi_rx_buff++ = UCB0RXBUF;
    spi_rx_num--;
    if (spi_rx_num) {
        UCB0TXBUF = 0xff;
    } else {
        UCB0IFG &=~UCRXIFG;
        UCB0IE  &=~UCRXIE;
    }
}
