/*
 * usci_b1_spi.h
 *
 *  Created on: 2016-06-25
 *      Author: Jack Chen <redchenjs@live.com>
 */

#ifndef USCI_B1_SPI_H_
#define USCI_B1_SPI_H_

extern void usci_b1_spi_transmit_frame(unsigned char *tx_buff, unsigned char *rx_buff, unsigned int num);

extern void usci_b1_spi_rx_isr_handle(void);
extern void usci_b1_spi_tx_isr_handle(void);

extern void usci_b1_spi_init(void);

#endif /* USCI_B1_SPI_H_ */
