/*
 * usci_a1_spi.h
 *
 *  Created on: 2016-06-25
 *      Author: Jack Chen <redchenjs@live.com>
 */

#ifndef USCI_A1_SPI_H_
#define USCI_A1_SPI_H_

extern unsigned char usci_a1_transmit_frame(unsigned char *p_buff, unsigned int num);
extern unsigned char usci_a1_receive_frame(unsigned char *p_buff, unsigned int num);

extern void usci_a1_spi_tx_isr_handle(void);
extern void usci_a1_spi_rx_isr_handle(void);

extern void usci_a1_init(void);

#endif /* USCI_A1_SPI_H_ */
