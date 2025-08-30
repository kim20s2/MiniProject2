/*
 * dht11.h
 *
 *  Created on: Aug 24, 2025
 *      Author: kim20
 */

#ifndef INC_DHT11_H_
#define INC_DHT11_H_

// Core/Inc/dht11.h
#pragma once
#include "stm32f4xx_hal.h"
#include <stdint.h>

// ── 핀 설정 (요청: PA1) ─────────────────────────
#define DHT_GPIO_Port   GPIOA
#define DHT_Pin         GPIO_PIN_1

// API
void     DWT_Delay_Init(void);
void     DWT_Delay_us(uint32_t us);

int      DHT11_Read(uint8_t *temp_c, uint8_t *rh);


#endif /* INC_DHT11_H_ */
