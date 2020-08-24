/*
 * HT1621.h
 *
 *  Created on: 2013-3-30
 *      Author: Administrator
 */

#ifndef HT1621_H_
#define HT1621_H_

extern unsigned int LCD_Buffer[8];	//全局变量，显存

extern void HT1621_SendBit();
extern void HT1621_init();
extern void HT1621_Reflash();
extern void HT1621_Reflash_Digit(unsigned char Position);


#endif /* HT1621_H_ */
