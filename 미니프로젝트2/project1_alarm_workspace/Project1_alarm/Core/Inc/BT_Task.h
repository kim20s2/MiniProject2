/*
 * BT_Task.h
 *
 *  Created on: Aug 25, 2025
 *      Author: kim20
 */

#ifndef INC_BT_TASK_H_
#define INC_BT_TASK_H_

#pragma once
void BT_Task(void *argument);

// 커널 시작 전에 1회 호출: USART6 1바이트 수신 시작
void BT_UART6_StartIT(void);


#endif /* INC_BT_TASK_H_ */
