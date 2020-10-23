/*

 * ads1118.h
 *
 *  Created on: 2018-05-17 11:23
 *      Author: Jack Chen <redchenjs@live.com>
*/

/*
#ifndef INC_DRIVER_ADS1118_H_
#define INC_DRIVER_ADS1118_H_

#include <stdint.h>

typedef struct {
    uint8_t reserved:1;
    uint8_t nop:2;
    uint8_t pull_up_en:1;
    uint8_t ts_mode:1;
    uint8_t dr:3;
    uint8_t mode:1;
    uint8_t pga:3;
    uint8_t mux:3;
    uint8_t ss:1;
} ads1118_reg_t;

typedef union {
    uint16_t all;
    ads1118_reg_t bits;
} ads1118_t;

extern ads1118_t *ads1118;

extern void ads1118_set_channel(uint8_t channel);
extern int16_t ads1118_convert(void);
extern void ads1118_init(void);

#endif  //INC_DRIVER_ADS1118_H_
*/

#ifndef __ADS1118_H__
#define __ADS1118_H__

#define uint unsigned int
#define uchar unsigned char

void ADS1118_Init();
float ADS1118(uchar port, uchar scale);
extern void get_ads1118_sample_voltage(void);
extern void cc_mode_adc2_feedback(float v);
extern  float a0, a1, a2;
#endif
