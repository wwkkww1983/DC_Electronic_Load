/*
 * dac8571.h
 *
 *  Created on: 2018-05-17 20:43
 *      Author: Jack Chen <redchenjs@live.com>
 */

/*#ifndef INC_DRIVER_DAC8571_H_
#define INC_DRIVER_DAC8571_H_

#include <stdint.h>

extern void dac8571_set_output(uint16_t value);
extern void dac8571_init(void);

#endif  INC_DRIVER_DAC8571_H_ */


#ifndef DAC8571_H_
#define DAC8571_H_

void DAC8571_Init(void);
void DAC8571_OUT( float num );

#endif /* DAC8571_H_ */
