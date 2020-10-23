/*
 * init.c
 *
 *  Created on: 2018-02-10 16:37
 *      Author: Jack Chen <redchenjs@live.com>
*   Fixed on :  2018-05-25 11:14
*      Author: art-j <jjw903427521@gmail.com>
 */

#include <inc/device/wdt.h>
#include <inc/device/ucs.h>
#include <inc/device/soft_i2c.h>
#include <inc/device/usci_a0_spi.h>
#include <inc/device/usci_b0_i2c.h>
#include <inc/device/usci_b1_spi.h>

#include <inc/driver/led.h>
#include <inc/driver/key.h>
#include <inc/driver/ssd1351.h>
#include <inc/driver/ads1118.h>
#include <inc/driver/dac8571.h>

void device_init(void)
{
    ucs_init();//统一时钟初始化
    wdt_init();//看门狗初始化

    usci_a0_spi_init();//USCI_A0_SPI初始化，对应ADS1118的引脚P2.7和P3.3
//    usci_b0_i2c_init();
    soft_i2c_init();//DAC8571的I2C通信引脚，dui
//    usci_b1_spi_init();
}

void driver_init(void)
{
    //led_init();//LED1.0初始化
    key_init();//外部按键初始化
    ssd1351_init();//OLED驱动芯片初始化
    DAC8571_Init();//DAC85871初始化
    ADS1118_Init();//ADS1118初始化
}
