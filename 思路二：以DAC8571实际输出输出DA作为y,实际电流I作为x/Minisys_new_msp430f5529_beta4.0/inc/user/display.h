/*
 * display.h
 *
 *  Created on: 2018-02-13 22:57
 *      Author: Jack Chen <redchenjs@live.com>
 */

#ifndef INC_USER_DISPLAY_H_
#define INC_USER_DISPLAY_H_

#include <stdint.h>

typedef struct {
    uint8_t mode;
    uint8_t flag;
    uint8_t index;
    uint8_t selected;
    uint8_t position;
} display_data_t;

extern display_data_t *display;

extern void display_update(void);

#endif /* INC_USER_DISPLAY_H_ */
