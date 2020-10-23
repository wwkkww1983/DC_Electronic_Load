/*
 * key.c
 *
 *  Created on: 2018-05-19 13:29
 *      Author: Jack Chen <redchenjs@live.com>
 *  Fixed on :  2018-05-25 11:14
 *      Author: art-j <jjw903427521@gmail.com>
 */

#include <msp430.h>

void key_init(void)
{
    P2DIR &=~(BIT0 + BIT2 + BIT4 + BIT5);//将P2.0 P2.2 P2.4 P2.5设置为输入
    P2REN |=  BIT0 + BIT2 + BIT4 + BIT5;
    P2OUT |=  BIT0 + BIT2 + BIT4 + BIT5;
}
