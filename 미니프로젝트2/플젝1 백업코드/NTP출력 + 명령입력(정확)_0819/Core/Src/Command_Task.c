/*
 * Command_Task.c
 *
 *  Created on: Aug 19, 2025
 *      Author: kim20
 */

// =============================
// command_task.c
// =============================
#include "cmsis_os.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "esp.h"
#include "main.h"

extern osMutexId_t ESP_MutexHandle;
extern cb_data_t cb_data;
void esp_event(const char *recvBuf); // already defined in your main.c (move that to a common .c if you like)

// LED/FAN 제어 매크로 (없다면 직접 HAL_GPIO_WritePin으로 대체)
#define LED_ON()   HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET)
#define LED_OFF()  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET)
#define FAN_ON()   HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, GPIO_PIN_SET)
#define FAN_OFF()  HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, GPIO_PIN_RESET)

/* ★ 중요: +IPD,<len>:<payload> 를 안정적으로 파싱
   - 개행을 기다리지 말고 길이 기반으로 정확히 잘라내기
*/


void esp_event(const char *recvBuf)
{
    char strBuff[128];
    char *pArray[4] = {0,};

    // 1) 문자열 복사 (안전하게 처리)
    strncpy(strBuff, recvBuf, sizeof(strBuff)-1);
    strBuff[sizeof(strBuff)-1] = '\0';

    // 2) 개행문자 제거
    char *newline = strchr(strBuff, '\n');
    if (newline) *newline = '\0';
    newline = strchr(strBuff, '\r');
    if (newline) *newline = '\0';

    // 3) 파싱 (구분자 @, 공백, [, ] 처리)
    int idx = 0;
    char *token = strtok(strBuff, "[]@ ");
    while (token != NULL && idx < 4)
    {
        pArray[idx++] = token;
        token = strtok(NULL, "[]@ ");
    }

    // 디버깅 출력
    /*
    printf("Parsed: pArray[0]=%s, pArray[1]=%s, pArray[2]=%s\r\n",
           pArray[0] ? pArray[0] : "NULL",
           pArray[1] ? pArray[1] : "NULL",
           pArray[2] ? pArray[2] : "NULL");
    */
    // 4) 명령 실행
    if (pArray[1] && pArray[2])
    {
        if (strcmp(pArray[1], "LED") == 0)
        {
            if (strcmp(pArray[2], "ON") == 0) {
                LED_ON();
                printf("LED turned ON\r\n");
            }
            else if (strcmp(pArray[2], "OFF") == 0) {
                LED_OFF();
                printf("LED turned OFF\r\n");
            }
        }
        else if (strcmp(pArray[1], "FAN") == 0)
        {
            if (strcmp(pArray[2], "ON") == 0) {
                FAN_ON();
                printf("FAN turned ON\r\n");
            }
            else if (strcmp(pArray[2], "OFF") == 0) {
                FAN_OFF();
                printf("FAN turned OFF\r\n");
            }
        }
        else
        {
            printf("Unknown device: %s\r\n", pArray[1]);
        }
    }
    else
    {
        printf("Invalid command format: %s\r\n", recvBuf);
    }
}

// ---- Robust +IPD,<len>:<payload> extractor (length-framed, not newline-based) ----
static int try_extract_ipd_payload(char *dst, int dst_sz)
{
    int copied = 0;
    int consumed = 0;

    // Minimal critical section: snapshot + partial consume
    taskENTER_CRITICAL();
    int n = cb_data.length;
    if (n > 0) {
        // find "+IPD,"
        int start = -1;
        for (int i = 0; i + 4 < n; ++i) {
            if (cb_data.buf[i] == '+' && i + 4 < n &&
                cb_data.buf[i+1]=='I' && cb_data.buf[i+2]=='P' && cb_data.buf[i+3]=='D' && cb_data.buf[i+4]==',') {
                start = i; break;
            }
        }
        if (start >= 0) {
            // parse length until ':'
            int len_start = start + 5;
            int colon = -1;
            for (int i = len_start; i < n && i < len_start + 10; ++i) {
                if (cb_data.buf[i] == ':') { colon = i; break; }
                if (cb_data.buf[i] < '0' || cb_data.buf[i] > '9') { start = -1; break; }
            }
            if (start >= 0 && colon > 0) {
                int ipd_len = atoi((const char*)&cb_data.buf[len_start]);
                int payload_start = colon + 1;
                int bytes_avail = n - payload_start;
                if (ipd_len > 0 && bytes_avail >= ipd_len) {
                    int cpy = (ipd_len < (dst_sz-1)) ? ipd_len : (dst_sz-1);
                    memcpy(dst, &cb_data.buf[payload_start], cpy);
                    dst[cpy] = 0;
                    copied = cpy;
                    consumed = payload_start + ipd_len; // consume through end of payload
                }
            }
        }

        if (consumed > 0) {
            int left = n - consumed;
            if (left > 0) memmove((void*)cb_data.buf, (void*)&cb_data.buf[consumed], left);
            cb_data.length = left;
        }
    }
    taskEXIT_CRITICAL();

    return copied; // 0 => need more bytes
}

void Command_Task(void *argument)
{
    char payload[256];
    for(;;)
    {
        // No need to require a trailing \n anymore
        int got = try_extract_ipd_payload(payload, sizeof(payload));
        if (got > 0) {
            // strip CR/LF
            payload[strcspn(payload, "\r\n")] = '\0';
            esp_event(payload);
        }

        // UART2 console (already in your code) can stay elsewhere
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
