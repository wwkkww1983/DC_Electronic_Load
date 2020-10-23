/*
 * soft_i2c.h
 *
 *  Created on: 2016-06-25
 *      Author: Jack Chen <redchenjs@live.com>
 */

#ifndef INC_DEVICE_SOFT_I2C_H_
#define INC_DEVICE_SOFT_I2C_H_

extern unsigned char soft_i2c_transmit_frame(unsigned char slave_addr, unsigned char *p_buff, unsigned char num);
extern unsigned char soft_i2c_receive_frame(unsigned char slave_addr, unsigned char *p_buff, unsigned char num);

extern void soft_i2c_init(void);

#endif /* INC_DEVICE_SOFT_I2C_H_ */
