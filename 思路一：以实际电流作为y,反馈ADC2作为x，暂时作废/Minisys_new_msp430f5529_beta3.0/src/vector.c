/*
 * vector.c
 *
 *  Created on: 2016-10-26
 *      Author: Jack Chen <redchenjs@live.com>
 *  Fixed on :  2018-05-25 11:14
 *      Author: art-j <jjw903427521@gmail.com>
 */

#include <msp430.h>

#include <inc/device/wdt.h>
#include <inc/device/usci_a0_spi.h>
#include <inc/device/usci_a1_spi.h>
#include <inc/device/usci_b0_spi.h>
#include <inc/device/usci_b1_spi.h>
#include <inc/device/usci_b0_i2c.h>
#include <inc/device/usci_b1_i2c.h>
#include <inc/system/user.h>

#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
/*    static unsigned long cnt = 0;

    if (cnt++ > 20) {
        cnt = 0;
        __bic_SR_register_on_exit(LPM0_bits);
    }*/
    P2_IODect();
    //__bic_SR_register_on_exit(LPM0_bits);
}

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
    switch (__even_in_range(UCA0IV, 4)) {
    case  0: break;                           // Vector  0: No interrupts
    case  2:                                  // Vector  2: RXIFG
        usci_a0_spi_rx_isr_handle();
        break;
    case  4:                                  // Vector  4: TXIFG
        usci_a0_spi_tx_isr_handle();
        break;
    default: break;
    }
}

#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
    switch (__even_in_range(UCB0IV,12)) {
    case  0: break;                           // Vector  0: No interrupts
    case  2: break;                           // Vector  2: ALIFG
    case  4: break;                           // Vector  4: NACKIFG
    case  6: break;                           // Vector  6: STTIFG
    case  8: break;                           // Vector  8: STPIFG
    case 10:                                  // Vector 10: RXIFG
        usci_b0_i2c_rx_isr_handle();
        break;
    case 12:                                  // Vector 12: TXIFG
        usci_b0_i2c_tx_isr_handle();
        break;
    default: break;
    }
}

#pragma vector=USCI_B1_VECTOR
__interrupt void USCI_B1_ISR(void)
{
    switch (__even_in_range(UCB1IV, 4)) {
    case  0: break;                           // Vector  0: No interrupts
    case  2:                                  // Vector  2: RXIFG
        usci_b1_spi_rx_isr_handle();
        break;
    case  4:                                  // Vector  4: TXIFG
        usci_b1_spi_tx_isr_handle();
        break;
    default: break;
    }
}
