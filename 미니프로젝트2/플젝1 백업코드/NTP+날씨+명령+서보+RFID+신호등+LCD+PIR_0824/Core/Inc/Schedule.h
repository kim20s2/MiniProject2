/*
 * Schedule.h
 *
 *  Created on: Aug 24, 2025
 *      Author: kim20
 */

#ifndef INC_SCHEDULE_H_
#define INC_SCHEDULE_H_

// Core/Inc/Schedule.h
#pragma once
#include <stdint.h>

typedef struct {
    uint8_t  enabled;     // 1=예약 있음
    uint8_t  hh;          // 시(0..23)
    uint8_t  mm;          // 분(0..59)
} schedule_t;

void Schedule_Init(void);
void Schedule_Set(uint8_t hh, uint8_t mm);
void Schedule_Clear(void);
void Schedule_Task(void *argument);


#endif /* INC_SCHEDULE_H_ */
