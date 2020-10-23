/*
 * control.h
 *
 *  Created on: 2018-05-17 14:59
 *      Author: Jack Chen <redchenjs@live.com>
 */

#ifndef INC_USER_CONTROL_H_
#define INC_USER_CONTROL_H_

#include <stdint.h>

typedef struct {
    uint8_t mode;
    uint16_t dac0_data;
    double dac0_voltage;
    int16_t min_temp;
    int16_t max_temp;
    double resistance;
    double temperature;
} control_data_t;

extern control_data_t *control;

extern void control_update(void);

#endif /* INC_USER_CONTROL_H_ */
