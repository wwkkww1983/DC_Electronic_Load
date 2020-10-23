/*
 * usci_a0_spi.h
 *
 *  Created on: 2016-06-25
 *      Author: Jack Chen <redchenjs@live.com>
 */

#ifndef USCI_A0_SPI_H_
#define USCI_A0_SPI_H_

extern unsigned char usci_a0_spi_transmit_frame(unsigned char *p_buff, unsigned int num);
extern unsigned char usci_a0_spi_receive_frame(unsigned char *p_buff, unsigned int num);

extern void usci_a0_spi_tx_isr_handle(void);
extern void usci_a0_spi_rx_isr_handle(void);

extern void usci_a0_spi_init(void);

#endif /* USCI_A0_SPI_H_ */
