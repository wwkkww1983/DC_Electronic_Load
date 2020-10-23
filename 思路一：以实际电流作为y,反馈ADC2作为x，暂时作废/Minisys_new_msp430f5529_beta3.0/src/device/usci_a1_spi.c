/*
 * usci_a1_spi.c
 *
 *  Created on: 2016-10-20
 *      Author: Jack Chen <redchenjs@live.com>
 * 
 * ---------SPI---------
 * PORT		TYPE	PIN
 * MOSI		OUT		P4.4
 * MISO		IN		P4.5
 * SCK      OUT     P4.0
 * ---------------------
 */

#include <msp430.h>

#define SPI_SET_PIN()   {\
                            P4SEL |= BIT0 + BIT4 + BIT5;\
                        }

static unsigned char *spi_tx_buff;
static unsigned char *spi_rx_buff;

static unsigned char spi_tx_num = 0;
static unsigned char spi_rx_num = 0;

unsigned char usci_a1_spi_transmit_frame(unsigned char *p_buff, unsigned char num)
{
	if (UCA1STAT & UCBUSY) return 0;
	__disable_interrupt();
    UCA1IE |= UCTXIE;
	UCA1IE &=~UCRXIE;
	__enable_interrupt();
    spi_tx_buff = p_buff;
    spi_tx_num  = num;
	UCA1TXBUF = *spi_tx_buff++;
    while (UCA1STAT & UCBUSY);
	return 1;
}

unsigned char usci_a1_spi_receive_frame(unsigned char *p_buff, unsigned char num)
{
    if (UCA1STAT & UCBUSY) return 0;
	__disable_interrupt();
    UCA1IE &=~UCTXIE;
    UCA1IE |= UCRXIE;
    __enable_interrupt();
    spi_rx_buff = p_buff;
    spi_rx_num  = num;
    UCA1TXBUF = 0xff;
    while (UCA1STAT & UCBUSY);
	return 1;
}

void usci_a1_spi_init(void)
{
    SPI_SET_PIN();

    UCA1CTL1 |= UCSWRST;
    UCA1CTL0 |= UCMST + UCMODE_0 + UCSYNC + UCCKPL + UCMSB;
    UCA1CTL1 |= UCSSEL_2;

    UCA1BR0  = 1;
    UCA1BR1  = 0;
    UCA1MCTL = 0;

    UCA1CTL1 &=~UCSWRST;
    UCA1IFG  &=~(UCTXIFG + UCRXIFG);
}

inline void usci_a1_spi_rx_isr_handle(void)
{
    *spi_rx_buff++ = UCA1RXBUF;
    spi_rx_num--;
    if (spi_rx_num) {
        UCA1TXBUF = 0xff;
    } else {
        UCA1IFG &=~UCRXIFG;
        UCA1IE  &=~UCRXIE;
    }
}

inline void usci_a1_spi_tx_isr_handle(void)
{
    UCA1RXBUF;
    spi_tx_num--;
    if (spi_tx_num) {
        UCA1TXBUF = *spi_tx_buff++;
    } else {
        UCA1IFG &=~UCTXIFG;
        UCA1IE  &=~UCTXIE;
    }
}
