/*
 * I2C.h
 *
 *  Created on: 2013-4-6
 *      Author: Administrator
 */

#ifndef I2C_H_
#define I2C_H_
//#define HARD_I2C			//±àÒëÓ²¼şI2C´úÂë
#define SOFT_I2C				//±àÒëÈí¼şI2C´úÂë
extern void I2C_Init();
extern void I2C_Tx_Init();
extern void I2C_Rx_Init();
extern unsigned char I2C_TxFrame(unsigned char *p_Tx,unsigned char num);
extern unsigned char I2C_RxFrame(unsigned char *p_Tx,unsigned char num);

#endif /* I2C_H_ */
