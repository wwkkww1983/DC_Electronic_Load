/*
 * measure.h
 *
 *  Created on: 2018-05-17 14:59
 *      Author: Jack Chen <redchenjs@live.com>
 */

#ifndef INC_USER_MEASURE_H_
#define INC_USER_MEASURE_H_

#include <stdint.h>

typedef struct {
    uint8_t mode;
    int16_t adc0_data;
    int16_t adc1_data;
    double adc0_voltage;
    double adc1_voltage;
} measure_data_t;

extern measure_data_t *measure;

extern void measure_update(void);

#endif /* INC_USER_MEASURE_H_ */
