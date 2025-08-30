/*
 * FanHold.h
 *
 *  Created on: Aug 24, 2025
 *      Author: kim20
 */

#ifndef INC_FANHOLD_H_
#define INC_FANHOLD_H_

#pragma once
#include "stm32f4xx_hal.h"
#include <stdint.h>

extern volatile uint8_t fan_hold;   // 0: 자유, 1: 온습도 태스크가 OFF 금지

#endif /* INC_FANHOLD_H_ */
