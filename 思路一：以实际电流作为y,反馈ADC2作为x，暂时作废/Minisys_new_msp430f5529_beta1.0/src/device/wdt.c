/*
 * wdt.c
 *
 *  Created on: 2016-10-20
 *      Author: Jack Chen <redchenjs@live.com>
 */

#include <msp430.h>

void wdt_init(void)
{
    WDTCTL = WDT_ADLY_16;
    SFRIE1 |= WDTIE;
}
