/*
 * spi.h
 *
 *  Created on: 2018-02-10 16:37
 *      Author: Jack Chen <redchenjs@live.com>
 */

#ifndef INC_DEVICE_SPI_H_
#define INC_DEVICE_SPI_H_

extern void spi_init(void);

extern unsigned char spi_transmit_frame(unsigned char *p_buff, unsigned int num);
extern unsigned char spi_receive_frame(unsigned char *p_buff, unsigned int num);

#endif /* INC_DEVICE_SPI_H_ */
