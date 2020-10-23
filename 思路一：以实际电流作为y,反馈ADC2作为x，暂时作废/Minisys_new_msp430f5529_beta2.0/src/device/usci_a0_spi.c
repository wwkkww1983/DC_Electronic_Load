/*
 * usci_a0_spi.c
 *
 *  Created on: 2016-10-20
 *      Author: Jack Chen <redchenjs@live.com>
 * 
 * ---------SPI---------
 * PORT		TYPE	PIN
 * MOSI		OUT		P3.3
 * MISO		IN		P3.4
 * SCK      OUT     P2.7
 * ---------------------
 */

#include <msp430.h>

#define SPI_SET_PIN()   {\
                            P3SEL |= BIT3 + BIT4;\
                            P2SEL |= BIT7;\
                        }

static unsigned char *spi_tx_buff;
static unsigned char *spi_rx_buff;

static unsigned char spi_tx_num = 0;
static unsigned char spi_rx_num = 0;

unsigned char usci_a0_spi_transmit_frame(unsigned char *p_buff, unsigned char num)
{
	if (UCA0STAT & UCBUSY) return 0;
	__disable_interrupt();
    UCA0IE |= UCTXIE;
	UCA0IE &=~UCRXIE;
	__enable_interrupt();
    spi_tx_buff = p_buff;
    spi_tx_num  = num;
	UCA0TXBUF = *spi_tx_buff++;
    while (UCA0STAT & UCBUSY);
	return 1;
}

unsigned char usci_a0_spi_receive_frame(unsigned char *p_buff, unsigned char num)
{
    if (UCA0STAT & UCBUSY) return 0;
	__disable_interrupt();
    UCA0IE &=~UCTXIE;
    UCA0IE |= UCRXIE;
    __enable_interrupt();
    spi_rx_buff = p_buff;
    spi_rx_num  = num;
    UCA0TXBUF = 0xff;
    while (UCA0STAT & UCBUSY);
	return 1;
}

void usci_a0_spi_init(void)
{
    SPI_SET_PIN();

    UCA0CTL1 |= UCSWRST;
    UCA0CTL0 |= UCMST + UCMODE_0 + UCSYNC + UCCKPL + UCMSB;
    UCA0CTL1 |= UCSSEL_2;

    UCA0BR0  = 2;
    UCA0BR1  = 0;
    UCA0MCTL = 0;

    UCA0CTL1 &=~UCSWRST;
    UCA0IFG  &=~(UCTXIFG + UCRXIFG);
}

inline void usci_a0_spi_rx_isr_handle(void)
{
    *spi_rx_buff++ = UCA0RXBUF;
    spi_rx_num--;
    if (spi_rx_num) {
        UCA0TXBUF = 0xff;
    } else {
        UCA0IFG &=~UCRXIFG;
        UCA0IE  &=~UCRXIE;
    }
}

inline void usci_a0_spi_tx_isr_handle(void)
{
    UCA0RXBUF;
    spi_tx_num--;
    if (spi_tx_num) {
        UCA0TXBUF = *spi_tx_buff++;
    } else {
        UCA0IFG &=~UCTXIFG;
        UCA0IE  &=~UCTXIE;
    }
}
