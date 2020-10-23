/*
 * soft_spi.h
 *
 *  Created on: 2016-06-25
 *      Author: Jack Chen <redchenjs@live.com>
 */

#ifndef SOFT_SPI_H_
#define SOFT_SPI_H_

extern unsigned char soft_spi_transmit_frame(unsigned char *p_buff, unsigned int num);
extern unsigned char soft_spi_receive_frame(unsigned char *p_buff, unsigned int num);

extern void soft_spi_init(void);

#endif /* SOFT_SPI_H_ */
