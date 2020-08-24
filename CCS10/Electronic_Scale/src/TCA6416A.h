/*
 * TCA6416A.h
 *
 *  Created on: 2013-4-6
 *      Author: Administrator
 */

#ifndef TCA6416A_H_
#define TCA6416A_H_

extern void PinIN();
extern void PinOUT(unsigned char pin,unsigned char status);
extern void TCA6416A_Init();
extern volatile unsigned int TCA6416A_InputBuffer;

#endif /* TCA6416A_H_ */
