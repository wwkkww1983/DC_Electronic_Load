/*
 * usci_b1_spi.c
 *
 *  Created on: 2016-10-20
 *      Author: Jack Chen <redchenjs@live.com>
 * 
 * ---------SPI---------
 * PORT     TYPE    PIN
 * MOSI     OUT     P4.1
 * MISO     IN      P4.2
 * SCK      OUT     P4.3
 * ---------------------
 */

#include <msp430.h>

#define SPI_SET_PIN()   {\
                            P4SEL |= BIT1 + BIT2 + BIT3;\
                        }

static unsigned char *spi_tx_buff;
static unsigned char *spi_rx_buff;

static unsigned char spi_tx_num = 0;

void usci_b1_spi_transmit_frame(unsigned char *tx_buff, unsigned char *rx_buff, unsigned char num)
{
    __disable_interrupt();
    UCB1IFG &=~(UCTXIFG + UCRXIFG);
    UCB1IE  |= (UCTXIE + UCRXIE);
    __enable_interrupt();
    spi_tx_buff = tx_buff;
    spi_rx_buff = rx_buff;
    spi_tx_num  = num;
    UCB1TXBUF = *spi_tx_buff++;
    while (UCB1STAT & UCBUSY);
}

inline void usci_b1_spi_rx_isr_handle(void)
{
    *spi_rx_buff++ = UCB1RXBUF;
    UCB1IFG &=~UCRXIFG;
}

inline void usci_b1_spi_tx_isr_handle(void)
{
    spi_tx_num--;
    if (spi_tx_num) {
        UCB1TXBUF = *spi_tx_buff++;
    } else {
        UCB1IFG &=~UCTXIFG;
        UCB1IE &=~(UCTXIE + UCRXIE);
    }
}

void usci_b1_spi_init(void)
{
    SPI_SET_PIN();

    UCB1CTL1 |= UCSWRST;
    UCB1CTL0 |= UCMST + UCMODE_0 + UCSYNC + UCMSB;
    UCB1CTL1 |= UCSSEL_2;

    UCB1BR0  = 10;
    UCB1BR1  = 0;

    UCB1CTL1 &=~UCSWRST;
    UCB1IFG  &=~(UCTXIFG + UCRXIFG);
}
